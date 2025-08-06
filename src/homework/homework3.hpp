#ifndef _homework3_hpp_
#define _homework3_hpp_

#include <cmath>

#include <QSize>
#include <QImage>
#include <QVector>
#include <QPolygonF>

#include <app/matrix_util.hpp>
#include <app/CameraCalib.hpp>
#include <app/TurntableCalib.hpp>

namespace hw3
{
  // estimate camera pose from
  // correspondences between pattern coordinates to image coordinates
  void estimateExtrinsics(Matrix3d const& K, /* intrinsic, input */
                          Vector5d const& kc, /* intrinsic, input */
                          QVector<Vector2d> const& worldCoord, /* input */
                          QVector<Vector2d> const& imageCoord, /* input */
                          Matrix3d& R, /* output */
                          Vector3d& T  /* output */);
  
  // from world coordinates to image coordinates
  void projectPoints
  (Matrix3d          const& K, /* input */
   Vector5d          const& kc, /* input */
   Matrix3d          const& R_world, /* input */
   Vector3d          const& T_world, /* input */
   QVector<Vector3d> const& worldPoints, /* input */  
   QVector<Vector2d>      & projectedPoints /* output */ );

  // from normalized camera coordinates to image coordinates
  void projectPoints
  (Matrix3d          const& K, /* input */
   Vector5d          const& kc, /* input */
   QVector<Vector3d> const& cameraPoints, /* input */  
   QVector<Vector2d>      & projectedPoints /* output */ );

  // Laser Plane Calibration

  // minor variation of hw2::detectLaser, where the output is a vector
  // of image points, and the detected points are restricted to those
  // falling inside a bounding rectangle
  void detectLaser(QImage const& inputImage, /* input */
                   QVector<Vector2d>& laserLineCoord, /* output */
                   int x0=0, int x1=0, int y0=0, int y1=0);

  // similar to previous method, but with a bounding polygon of
  // arbitrary shape
  void detectLaser(QImage const& inputImage, /* input */
                   QVector<Vector2d>& laserLineCoord, /* output */
                   const QPolygonF& boundingQuad);

  // compute equation of the checkerboard pattern plane from it pose
  void checkerboardPlane(Matrix3d const& R, /* input */
                         Vector3d const& T, /* input */
                         Vector4d& checkerboardPlane /* output */);

  // ray-plane triangulation
  void triangulate(Matrix3d          const& K,                 /* input */
                   Vector5d          const& kc,                /* input */
                   Vector4d               & planeCoefficients, /* input */
                   QVector<Vector2d> const& imagePoints,       /* input */
                   QVector<Vector3d>      & worldPoints        /* output */ );

  // least squares fitting using SVD or Eigen decomposition
  void estimatePlane(QVector<Vector3d> const& laserPoints, /* input */
                     Vector4d               & laserPlane /* output */);

  // Turntable Calibration

  void estimateTurntableCoordSystem
  (QVector<Matrix3d>  const& R,                      /* input */
   QVector<Vector3d>  const& T,                      /* input */
   Vector3d&                 patternCenter,          /* input/output */
   Vector3d&                 turntableWorldCenter,   /* output */
   QVector<Matrix3d>       & turntableWorldRotation  /* output */);

  void generateWireframeModel
  (const double height, const double radius, const int sides,
   /* outputs */
   QVector<Vector3d>& worldCoord, QVector<int>& edge);
};

#endif //_homework3_hpp_
