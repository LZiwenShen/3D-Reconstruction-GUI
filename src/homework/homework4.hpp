#ifndef _homework4_hpp_
#define _homework4_hpp_

#include <QSize>
#include <QImage>
#include <QVector>

// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <util/MatrixUtil.hpp>
// #include <app/CalibrationCamera.hpp>
#include <app/TurntableCalib.hpp>

namespace hw4 {

  // reference
  // https://docs.opencv.org/4.10.0/d9/d0c/group__calib3d.html

  // this function is already implemented in homework4.cpp using OpenCV functions
  int detectChessboardCorners
  (// inputs
   cv::Mat1b image,
   cv::Size2i cornerCount,
   cv::Size2f cornerSize,
   // outputs
   std::vector<cv::Point3f> & cornersWorld,
   std::vector<cv::Point2f> & cornersCamera);

  // this function is already implemented in homework4.cpp using OpenCV functions
  double calibrateCamera
  (// inputs
   std::vector<std::vector<cv::Point3f> > & worldPoints,
   std::vector<std::vector<cv::Point2f> > & imagePoints,
   cv::Size2i & imageSize,
   // outputs
   cv::Mat & K,
   cv::Mat & kc, 
   std::vector<cv::Mat> & R_vecs,
   std::vector<cv::Mat> & T_vecs);

  void projectPoint
  (Matrix3d const& K,
   Vector5d const& kc,
   Matrix3d const& R,
   Vector3d const& T,
   Vector3d const& p_world,
   Vector2d      & p_image);

  void calibrateTurntable
  (QVector<Matrix3d>  const& R_pose,                /* input */
   QVector<Vector3d>  const& T_pose,                /* input */
   Vector3d&                 patternCenter,         /* input/output */
   Vector3d&                 turntableWorldCenter,  /* output */
   QVector<Matrix3d>&        turntableWorldRotation /* output */);

  // this function is already implemented in homework4.cpp using OpenCV functions
  int estimatePose
  (// inputs
   cv::Mat & K,
   cv::Mat & kc, 
   std::vector<cv::Point3f> & worldPoints,
   std::vector<cv::Point2f> & imagePoints,
   cv::Size2i & imageSize,
   // outputs
   cv::Mat & R_vec,
   cv::Mat & T_vec);

  void detectLaserPoints
  (QImage const& imageLaserOn, /* input */
   QImage const& imageLaserOff, /* input */
   QVector<Vector2d>& laserLineCoord /* output */ );

  void detectLaserPoints
  (QImage const& imageLaserOn, /* input */
   QImage const& imageLaserOff, /* input */
   const QPolygonF& boundingQuad, /* input */
   QVector<Vector2d>& laserLineCoord, /* output */
   QVector<Vector3d>* color=nullptr /* output*/ ); // 0.0<=r,g,b<=1.0

  // - instead of implementing this new function
  //
  // void calibrateLaserPlane
  // (// inputs
  //  cv::Mat & K,
  //  cv::Mat & kc,
  //  QVector<QString> const& laserImageFileName,
  //  cv::Size2i cornerCount,
  //  cv::Size2f cornerSize,
  //  // outputs
  //  Vector4d& laserPlane);
  //
  //  - check how
  //    PanelCalibrate::on_laserFitPlanesButton_clicked();
  //    uses hw3::estimatePlane();

  // - we don't need a new function to triangulate the laser points here
  //
  // void triangulate
  // (// inputs
  //  Matrix3d const&    K,
  //  Vector5d const&    kc,
  //  Matrix3d const&    worldRotation,
  //  Vector3d const&    worldTranslation,
  //  float    const     angle,
  //  QImage   const&    imageLaserOff,
  //  QImage   const&    imageLaserOn,
  //  Vector4d const&    laserPlane,
  //  // outputs
  //  QVector<Vector3d>& worldCoord);
  //
  //  - instead, PanelCalibrate::on_laserTriangulateButton_clicked()
  //    uses hw3::checkerboardPlane() and hw3::triangulate();

};

#endif //_homework4_hpp_
