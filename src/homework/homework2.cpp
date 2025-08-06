#include "homework2.hpp"
#include "homework1.hpp"

//homework functions

double hw2::reprojectPoints(Matrix3d const& K, Vector5d const& kc, 
                            Matrix3d const& R, Vector3d const& T,
                            QVector<Vector3d> const& worldPoints, 
                            QVector<Vector2d> const& imagePoints, 
                            QVector<Vector2d> & reprojectedPoints)
{
  //  Description:
  //
  //  hw2::reprojectPoints() receives camera intrisics (K,kc) and
  //  extrinsics (R,T) parameters, as well as, the 3D coordinates of
  //  the checkerboard coordinates (worlPoints). Your task is to use
  //  these parameters to reproject each of the 3D points onto the
  //  image and save the reprojected 2D points in the output
  //  'reprojectedPoints'.  In addition, you must compare your
  //  reprojected points with the original 2D image points, given in
  //  'imagePoints' and compute the Root Mean Square Error (RMSE). The
  //  RMSE is the return value of this function.

  //  We use Eigen Lib for matrix and vector operations
  //  check this for help:
  //  http://eigen.tuxfamily.org/dox/group__QuickRefPage.html
  
  //  Check the Camera Calibration Toolbox webpage to learn how to project 
  //  a 3D point, including lens distortion:
  //  http://www.vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html

  double rmse = 0.0;
  auto nPoints = worldPoints.size();
  for (int i=0; i<nPoints; ++i) {
      auto const& p = worldPoints[i];
      auto pc = R * p + T;

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
      Vector3d pi = K * d;
      Vector2d u(pi[0], pi[1]);

      //save result
      reprojectedPoints.push_back(u);

      //compare with the original image point 'v'
      auto const& v = imagePoints[i];
      rmse += pow(u[0] - v[0], 2) + pow(u[1] - v[1], 2);
      //TODO ...
  }

  //RMSE
  rmse = std::sqrt(rmse/(2*nPoints));

  return rmse;
}

QVector<Vector3d> hw2::triangulate
(Matrix3d const& K, Vector5d const& kc, 
 Matrix3d const& R, Vector3d const& T,
 Vector4d const& laserPlane, double turntableAngle,
 QImage image,
 int const x0, int const x1, int const y0, int const y1) {

  // Description:
  // hw2::triangulate() receives the Camera and Turntable calibration
  // as input parameters, together with an image. The function will
  // call hw1::detectLaser() to find a vertical laser line, and it
  // will triangulate using plane-ray intersection each of the laser
  // line pixels. The function returns a vector of 3D points.

  // Coordinate systems:
  // The world coordinate system is at the center of the turntable,
  // with the xy-plane on the turntable and z-axis upwards. The camera
  // coordinate system is located at the center of projection and
  // following the camera orientation as usual.  At each image the
  // turntable was rotated by 'turntableAngle' radians, thus, after
  // triangulation each 3D point must be rotated backwards so that
  // points are located at the correct position in the unrotated
  // scanning target.

  // Laser Light Plane:
  // The laser plane is given as a 4-vector in the camera coordinate
  // systems. This is convenient because we can calculate the
  // intersection point of the camera ray with this plane directly in
  // camera coords and later transform the point to world coords.  If
  // we call the plane 4-vector 'n', then a 3D point in homogeneous
  // coordinates p = (X,Y,Z,1) belongs to the plane if and only if
  // n.transpose()*p = 0.
  
  // Camera Lens Distortion:
  // The model commonly used to compensate for lens distortion does
  // not have a closed form inverse. We will ignore the lens
  // distortion here to simplify the task.

  Q_UNUSED(kc)

  //detect laser
  QImage detectionImage = hw1::detectLaser(image);

  //output vector of triangulated 3D points
  QVector<Vector3d> points;

  //triangulate
  for (int h=0; h<detectionImage.height(); ++h) {
      for (int w=0; w<detectionImage.width(); ++w) {
          auto pixel = detectionImage.pixel(w, h);
          int value = qRed(pixel);
          if (!value) { /* skip */ continue; }

          //transform from pixel to camera coordinates
          //Vector3d u(w, h, 1);
          //Vector3d v = ...
          //TODO ...
          Vector3d u(w, h, 1);
          Vector3d v = K.inverse() * u;

          //ray-plane intersection (NO RADIAL DISTORTION)
          //double lambda = ...
          //Vector3d p = lambda * ...
          //TODO ...
          double lambda = laserPlane[3]/(v[0]*laserPlane[0]+v[1]*laserPlane[1]+v[2]*laserPlane[2]);
          Vector3d pt1 = lambda * v;

          //transform from camera to turntable coords
          //p = ...
          Vector3d pt2 = R.transpose() * (pt1 - T);

          //undo the turntable location to place the point in world coords
          //TODO ...
          Vector3d p(cos(turntableAngle)*pt2[0]-sin(turntableAngle)*pt2[1],cos(turntableAngle)*pt2[1]+sin(turntableAngle)*pt2[0],pt2[2]);

          //save the point
          points.push_back(p);

      }//for cols
  }//for rows

  return points;
}
