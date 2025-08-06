#include <iostream>

// TODO Sun Nov 10 2024
// - 

#include "homework1.hpp"
// #include "homework2.hpp"
// #include "homework3.hpp"
#include "homework4.hpp"

#include <Eigen/SVD>
#include <math.h>

using namespace Eigen;

#define USE_FIND_CHESSBOARD_CORNERS_SB

//////////////////////////////////////////////////////////////////////
// - already implemented using OpenCV functions

int hw4::detectChessboardCorners
(/*inputs*/
 cv::Mat1b imageGray,
 cv::Size2i cornerCount, // (cols,rows)
 cv::Size2f cornerSize,  // (width,height)
 /* outputs */
 std::vector<cv::Point3f> & cornersWorld,
 std::vector<cv::Point2f> & cornersCamera) {

  // returns number of detected corners is successful; 0 otherwise
 
  cornersWorld.clear();
  cornersCamera.clear();

#ifdef USE_FIND_CHESSBOARD_CORNERS_SB

  // CALIB_CB_NORMALIZE_IMAGE Normalize the image gamma with
  // equalizeHist before detection.
  //
  // CALIB_CB_EXHAUSTIVE Run an exhaustive search to improve detection
  // rate.
  //
  // CALIB_CB_ACCURACY Up sample input image to improve sub-pixel
  // accuracy due to aliasing effects.
  //
  // CALIB_CB_LARGER The detected pattern is allowed to be larger than
  // patternSize (see description).
  //
  // CALIB_CB_MARKER The detected pattern must have a marker (see
  // description). This should be used if an accurate camera
  // calibration is required.

  int retVal = cv::findChessboardCornersSB
    (imageGray, cornerCount, cornersCamera,
     cv::CALIB_CB_NORMALIZE_IMAGE +
     cv::CALIB_CB_EXHAUSTIVE +
     cv::CALIB_CB_ACCURACY
     /* OutputArray meta */
     );

  if(retVal==0) return 0; // chessboard not detected

#else // #ifndef USE_FIND_CHESSBOARD_CORNERS_SB

  // check image size
  int imageScale = 1;
  cv::Size imageSize = imageGray.size();
  if (imageSize.width>1024) {
    imageScale = cvRound(imageSize.width/1024.0);
  }
        
  // scale down image to speed up process
  cv::Mat1b imgSmall = imageGray;
  if (imageScale>1) {
    cv::resize(imageGray, imgSmall,
               cv::Size(imageSize.width/imageScale, imageSize.height/imageScale));
  }

  int retVal = cv::findChessboardCorners
    (imgSmall, cornerCount, cornersCamera,
     cv::CALIB_CB_ADAPTIVE_THRESH +
     cv::CALIB_CB_NORMALIZE_IMAGE /* + */
     /*cv::CALIB_CB_FILTER_QUADS*/
     );

  if(retVal==0) return 0; // chessboard not detected
  
  // adjust camera corners to original scale if necessary
  if(imageScale!=1) {
    for (auto it=cornersCamera.begin(); it!=cornersCamera.end(); ++it) {
      *it *= imageScale;
    }
  }
  
  // refine camera corners at subpixel resolution with respect to original image
  cv::TermCriteria termCriteria =
    cv::TermCriteria( cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.01 );
  
  cv::cornerSubPix
    (imageGray, cornersCamera, cv::Size(11, 11), cv::Size(-1, -1), termCriteria);

#endif // USE_FIND_CHESSBOARD_CORNERS_SB

  // generate chessboard coordinates array
  for (int h=0; h<cornerCount.height; h++) {
    for (int w=0; w<cornerCount.width; w++) {
      cornersWorld.push_back
        (cv::Point3f(cornerSize.width * w, cornerSize.height * h, 0.f));
    }
  }

  return cornersCamera.size();
}

//////////////////////////////////////////////////////////////////////
// - already implemented using OpenCV functions

double hw4::calibrateCamera
( // inputs
 std::vector<std::vector<cv::Point3f> > & worldPoints,
 std::vector<std::vector<cv::Point2f> > & imagePoints,
 cv::Size2i & imageSize,
 // outputs
 cv::Mat & K,
 cv::Mat & kc, 
 std::vector<cv::Mat> & R_vecs,
 std::vector<cv::Mat> & T_vecs) {

  double camError = -1.0;

  int calFlags = 0
    // + cv::CALIB_FIX_K1
    // + cv::CALIB_FIX_K2
    // + cv::CALIB_ZERO_TANGENT_DIST
    + cv::CALIB_FIX_K3
    ;

  camError =
    cv::calibrateCamera(worldPoints,imagePoints,
                        imageSize,K,kc,R_vecs,T_vecs,calFlags,
                        cv::TermCriteria(cv::TermCriteria::COUNT +
                                         cv::TermCriteria::EPS, 50, DBL_EPSILON));
  return camError;
}

//////////////////////////////////////////////////////////////////////
void hw4::projectPoint
  (Matrix3d const& K,
   Vector5d const& kc,
   Matrix3d const& R,
   Vector3d const& T,
   Vector3d const& p_world,
   Vector2d      & p_image) {

  // similar to hw3::projectPoints(); but this one projects a single point

  // 1) change of coordinates from world to camera
  // 2) perspective projection
  // 3) apply lens distortion
  // 4) convert from normalized homogeneous coordinates to pixel coordinates
  // 5) compute non-homogeneous coordinates
  auto pc = R * p_world + T;

  //project 'p' to the image plane
  //Vector2d u = ...
  //TODO ...
  Vector2d pn(pc.x()/pc.z(), pc.y()/pc.z());

  //apply lens distortion to 'u'
  //u = ...
  //TODO ...
  double r2 = pow(pn[0], 2) + pow(pn[1], 2);
  Vector2d dx(2*kc[2]*pn[0]*pn[1]+kc[3]*(r2+2*pow(pn[0],2)),kc[2]*(r2+2*pow(pn[1],2))+2*kc[3]*pn[0]*pn[1]);
  double f = 1+kc[0]*r2+kc[1]*pow(r2,2)+kc[4]*pow(r2,3);
  Vector2d pd = f * pn + dx;

  //convert 'u' to pixel coords
  // u = ...
  //TODO ...
  Vector3d d(pd[0], pd[1], 1.0);
  Vector3d p = K * d;
  p_image(0) = p[0];
  p_image(1) = p[1];
}

//////////////////////////////////////////////////////////////////////
//
// calibrate using the same images of vertical chessboards used for
// camera calibration
//
void hw4::calibrateTurntable
(QVector<Matrix3d>  const& R_pose,                /* input */
 QVector<Vector3d>  const& T_pose,                /* input */
 Vector3d&                 patternCenter,         /* input/output */
 Vector3d&                 turntableWorldCenter,  /* output */
 QVector<Matrix3d>&        turntableWorldRotation /* output */) {

  // - center of rotation on turntable surface
  // patternCenter(0) contains an input value, which should not be modfied
  // patternCenter(1) and patternCenter(2) are output values computed here

  // check for invalid arguments
  int nPoses = R_pose.size();
  if(nPoses< 2 || nPoses!=T_pose.size()) return;

  // allocate matrix A (3*nPoses)x(5), and vector b (3*nPoses)x(1)
  Matrix<double,Dynamic,Dynamic> A;
  A.resize(3*nPoses,5);
  Matrix<double,Dynamic,Dynamic> b;
  b.resize(3*nPoses,1);

  // fill matrix A and vector b
  for(int i=0;i<nPoses;i++) {

    // fill rows 3*i,3*i+1,3*i+2 of A and b

    const Matrix3d & R_pose_i = R_pose[i];
    const Vector3d & T_pose_i = T_pose[i];

    // R_pose_i = [ri0 ri1 ri2]
    // p = ri0*x+ri1*y+ri2*z+T_pose_i
    // X = (y,z,p[0],p[1],p[2])^t --- unknowns
    // Ai = [ri1 ri2 -I]
    // bi = x*ri0-T_pose_i
    Vector3d ri0 = R_pose_i.col(0);
    Vector3d ri1 = R_pose_i.col(1);
    Vector3d ri2 = R_pose_i.col(2);
    A.block<3, 2>(3 * i, 0) << ri1, ri2;
    A.block<3, 3>(3 * i, 2) << -Matrix3d::Identity();
    b.block<3, 1>(3 * i, 0) = -(patternCenter(0) * ri0 + T_pose_i);
    std::cout << ri0 << std::endl;
    std::cout << T_pose_i << std::endl;
  }
  //std::cout << A << std::endl;
  //std::cout << b << std::endl;
  std::cout << patternCenter(0) << std::endl;
  JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
  VectorXd X = svd.solve(b);

  // solve
  // X = argmin_X \| AX-b \|^2
  // using Eigen::JacobiSVD

  // Vector2d& centerCoord, /* output */
  // coordinates in the (y,z) plane, perpendicular to the (x,y) plane of the pattern 
  // turntablePlaneX == x
  //
  patternCenter(1) = X(0); // y
  patternCenter(2) = X(1); // z

  // Vector3d& turntableWorldCenter /* output */) {
  //
  turntableWorldCenter(0) = X(2);
  turntableWorldCenter(1) = X(3);
  turntableWorldCenter(2) = X(4);

  // turntablePoseRotations
  turntableWorldRotation.clear();
  for(int i=0;i<nPoses;i++) {
    const Matrix3d & R_pose_i = R_pose[i];
    Matrix3d turntableWorldRotation_i;
        turntableWorldRotation_i <<
          R_pose_i(0,1), R_pose_i(0,2), R_pose_i(0,0),
          R_pose_i(1,1), R_pose_i(1,2), R_pose_i(1,0),
          R_pose_i(2,1), R_pose_i(2,2), R_pose_i(2,0);
    turntableWorldRotation.push_back(turntableWorldRotation_i);
  }

}

//////////////////////////////////////////////////////////////////////
// - already implemented using OpenCV

int hw4::estimatePose
(// inputs
 cv::Mat & K,
 cv::Mat & kc, 
 std::vector<cv::Point3f> & worldPoints,
 std::vector<cv::Point2f> & imagePoints,
 cv::Size2i & imageSize,
 // outputs
 cv::Mat & R_vec,
 cv::Mat & T_vec) {

  // same as hw3::estimateExtrinsics()
  // with different data structures
  // already implemented in OpenCV (one of many available algorithms)
  
  int retVal = cv::solvePnP
      (worldPoints, imagePoints, K, kc, R_vec, T_vec);

  if(retVal==0) return 0;

  return worldPoints.size();
}

//////////////////////////////////////////////////////////////////////
void hw4::detectLaserPoints
(QImage const& imageLaserOn, /* input */
 QImage const& imageLaserOff, /* input */
 QVector<Vector2d>& laserLineCoord /* output */ ) {
  QPolygonF imgQuad;
  double w = static_cast<double>(imageLaserOn.width()-1);
  double h = static_cast<double>(imageLaserOn.height()-1);
  QPointF p00(0.0,0.0);
  QPointF p01(0.0,  h);
  QPointF p10(  w,0.0);
  QPointF p11(  w,  h);
  imgQuad << p00 << p01 << p11 << p10 << p00;
  detectLaserPoints(imageLaserOn, imageLaserOff, imgQuad, laserLineCoord);
}

//////////////////////////////////////////////////////////////////////
void hw4::detectLaserPoints
(QImage const& imageLaserOn, /* input */
 QImage const& imageLaserOff, /* input */
 const QPolygonF& boundingQuad, /* input */
 QVector<Vector2d>& laserLineCoord, /* output */
 QVector<Vector3d>* pColor /* output*/) {

  // - similar to
  //  
  // void hw3detectLaser(QImage const& inputImage, /* input */
  //                     QVector<Vector2d>& laserLineCoord, /* output */
  //                     const QPolygonF& boundingQuad);
  //
  //   but with two input images, one with the laser on, and the other
  //   with the laser off
  //
  // - if the argument pColor==nullptr you only have to determine the
  //   u,v coordinates of the pixels iluminated by the laser, but only
  //   within the boundingQuad;
  //
  // - you can use the function QPolygonF::containsPoint() to
  // - determine whether or not an illuminated pixel is insideor
  // - outside of the boundingQuad.
  //
  // - if the argument pColor!=nullptr you also have to generate an
  //   rgb color for each detected point, buy looking at the same
  //   pixel in the image with the laser off; the r,g,b components of
  //   this color vector should be double in the range [0.0:1.0]
  // 

  int rows = imageLaserOff.height();
  int cols = imageLaserOff.width();
  laserLineCoord.clear();
  if(pColor) pColor->clear();
  //QImage processedImage = imageLaserOn.convertToFormat(QImage::Format_RGB32);
  //QVector<int> filter(QVector<int>() << 1 << 4 << 6 << 4 << 1);
  //QImage processedImage = hw1::filterImage(imageLaserOn, filter);

  QImage processedImage(imageLaserOn.size(), imageLaserOn.format());
  for (int y = 0; y < rows; ++y) {
      for (int x = 0; x < cols; ++x) {
          QRgb pixelOn = imageLaserOn.pixel(x, y);
          QRgb pixelOff = imageLaserOff.pixel(x, y);

          int r = qBound(0, qRed(pixelOn) - qRed(pixelOff), 255);
          int g = qBound(0, qGreen(pixelOn) - qGreen(pixelOff), 255);
          int b = qBound(0, qBlue(pixelOn) - qBlue(pixelOff), 255);

          processedImage.setPixel(x, y, qRgb(r, g, b));
      }
  }

  //isolate red color
  for (int h = 0; h < rows; ++h)
  {
      for (int w = 0; w < cols; ++w)
      {
          QRgb pixel = processedImage.pixel(w, h);
          int r = qRed(pixel);
          int g = qGreen(pixel);
          int b = qBlue(pixel);

          int value = r - (g + b) / 2;
          if (value < 0)
              value = 0;

          processedImage.setPixel(w, h, qRgb(value, value, value));
      }
  }

  //search average maximum of each row
  int sum = 0;

  for (int h = 0; h < rows; ++h)
  {
      int max = 0;
      for (int w = 0; w < cols; ++w)
      {
          QRgb pixel = processedImage.pixel(w, h);
          if (qRed(pixel) > max)
              max = qRed(pixel);
      }
      sum += max;
  }
  double avg = sum / rows;

  //apply threshold
  for (int h = 0; h < rows; ++h)
  {
      for (int w = 0; w < cols; ++w)
      {
          QRgb pixel = processedImage.pixel(w, h);
          if (qRed(pixel) < 0.5 * avg)
              processedImage.setPixel(w, h, qRgb(0, 0, 0));
      }
  }

  //search the maximum of each row
  for (int h = 0; h < rows; ++h)
  {
      int max = 0;
      int w0 = -1;
      for (int w = 0; w < cols; ++w)
      {
          QRgb pixel = processedImage.pixel(w, h);
          if (qRed(pixel) > max)
          {
              w0 = w;
              max = qRed(pixel);
          }
      }
      if (w0 > -1)
      {
          QPointF point(w0, h);
          if (boundingQuad.containsPoint(point, Qt::OddEvenFill)) {
              Vector2d detectedPoint(w0, h);
              laserLineCoord.push_back(detectedPoint);

              if (pColor) {
                  QRgb pixelOff = imageLaserOff.pixel(w0, h);
                  double r = qRed(pixelOff) / 255.0;
                  double g = qGreen(pixelOff) / 255.0;
                  double b = qBlue(pixelOff) / 255.0;

                  Vector3d color(r, g, b);
                  pColor->push_back(color);
              }
          }
      }
  }

  // check input parameters;
  // return right away if you find any errors here

  // ...
}
