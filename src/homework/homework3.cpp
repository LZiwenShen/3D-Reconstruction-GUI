#include <iostream>

#include "homework3.hpp"
// #include "homework2.hpp"
#include "homework1.hpp"

#include <Eigen/SVD>
#include <math.h>

using namespace Eigen;

// Turntable Calibration

void hw3::estimateExtrinsics
(Matrix3d const& K, /* intrinsic */
 Vector5d const& kc, /* intrinsic */
 QVector<Vector2d> const& worldCoord, /* input */
 QVector<Vector2d> const& imageCoord, /* input, distorted */
 Matrix3d& R, /* output */
 Vector3d& T  /* output */) {

  (void)kc; // ignore lens distortion for this assignmet

  // (R,T) to be estimated
  //
  // [r0 r1 r2] = R
  // R*p+T = r0 * px + r1 * py + T
  //
  // for each p=(px,py,0)^t in worldCoord

  int nPoints = worldCoord.size();

  MatrixXd A(2 * nPoints, 9);

  for (int i = 0; i < nPoints; i++)
  {
      auto const& u_dist_pix = imageCoord[i];
      Vector3d ud(u_dist_pix[0], u_dist_pix[1], 1);
      Vector3d u = K.inverse() * ud;
      auto const& p = worldCoord[i];
      A.row(2 * i) << 0, -p[0], u.y() * p[0], 0, -p[1], u.y() * p[1], 0, -1, u.y();
      A.row(2 * i + 1) << p[0], 0, -u.x() * p[0], p[1], 0, -u.x() * p[1], 1, 0, -u.x();
  }

  // create a matrix A of dimension 2*nPoints rows X 9 columns
  // for each of the nPoints p=(x,y,0)^t in worldCoord  {
  //   - get the corresponding image point in pixel coords from imageCoordC
  //   - compute the normalized distorted image point coordinates by
  //     solving : K*u_dist_norm = u_dist_pix
  //
  //     [ k_00 k_01 k_02 ] [ u_dist_norm(0) ]          [ u_dist_pix(0) ]
  //     [   0  k_11 k_12 ] [ u_dist_norm(1) ] = lambda [ u_dist_pix(1) ]
  //     [   0    0  k_22 ] [       1        ]          [        1      ]
  //
  //   - remove distortion (ignored here)
  //     to obtain normailized camera coordinates (ux,uy)
  //
  //   - add the following two rows to the matrix A
  //     [  0  -px  uy*px  0  -py  uy*py  0 -1  uy ]
  //     [  px  0  -ux*px  py  0  -ux*py  1  0 -ux ]
  // }
  //
  // - compute
  //   X = argmin \| AX \|^2 under the cnstarint \|X\|^2=1
  //
  // - solution is right singular vector corresponding to the minimum
  //   singular value of A
  // - can use Eigen::JacobiSVD to perform this computation
  //
  // - except for a multiplicative normalization factor, the vector X
  //   should be equal to the concatenation of the vectors r0, r1, and
  //   t
  //
  // - the normalization factor can be determined by the fact that r0
  //   and r1, being the first two columns of a rotation matrix,
  //   should be unit length vectors and orthogonal to each other (how
  //   do we deal with the fact that here we have three constraints,
  //   but we can impose only one to determine the normalization factor?)
  //
  // - find a way to measure how far are the resulting matrix R is
  // - from an orthogonal matrix, and print the value
  //
  // - once the normalization factor is determined, X should be
  //   normalized before t is extracted
  //
  // - also the third column r2 of the matrix R can be computed as the
  //   cross product of r0 and r1
  JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
  MatrixXd V = svd.matrixV();
  VectorXd x = V.col(V.cols() - 1);
  Vector3d r1_hat(x(0), x(1), x(2));
  Vector3d r2_hat(x(3), x(4), x(5));
  Vector3d T_hat(x(6), x(7), x(8));
  double s = 2 / (r1_hat.norm() + r2_hat.norm());
  Vector3d r1 = s * r1_hat;
  Vector3d r3 = r1.cross(s * r2_hat);
  Vector3d r2 = r3.cross(r1);
  R.col(0) = r1;
  R.col(1) = r2;
  R.col(2) = r3;
  T= s * T_hat;
}

//////////////////////////////////////////////////////////////////////
void hw3::projectPoints
(Matrix3d          const& K,
 Vector5d          const& kc, 
 Matrix3d          const& R_world,
 Vector3d          const& T_world,
 QVector<Vector3d> const& worldPoints, 
 QVector<Vector2d>      & projectedPoints) {
  
  // - derived from hw2::reprojectPoints() by removing the image
  //   points detected by the calibration software used to measure the
  //   rms error
  
  // - remember to apply the lens distortion model and to convert to
  //    pixel coordinates
  auto nPoints = worldPoints.size();
  for (int i=0; i<nPoints; ++i) {
      auto const& p = worldPoints[i];
      auto pc = R_world * p + T_world;

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
      projectedPoints.push_back(u);
  }
}

//////////////////////////////////////////////////////////////////////
void hw3::projectPoints
(Matrix3d          const& K, /* input */
 Vector5d          const& kc, /* input */
 QVector<Vector3d> const& cameraPoints, /* input */  
 QVector<Vector2d>      & imagePoints /* output */ ) {

  // - same as the previous version when the camera coordinate system
  //   is the world coordinate system; i.e., when R_world ==
  //   Matrix3d::Identity() and T_world == Vector3d::Zero()
    auto nPoints = cameraPoints.size();
    for (int i=0; i<nPoints; ++i) {
        auto const& pc = cameraPoints[i];

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
        imagePoints.push_back(u);
    }
}

// Laser Plane Calibration

//////////////////////////////////////////////////////////////////////
void hw3::detectLaser
(QImage const& inputImage, /* input */
 QVector<Vector2d>& laserLineCoord, /* output */
 int x0, int x1, int y0, int y1) {

  // - closely related to
  //   hw1::detectLaser(QImage const& inputImage);
  //
  // - for each row in the range [y0:y1] of the input image
  //   if a laser point is detected on that row (assumed to be at most one)
  //     an the location of the detected point is in the the range [x0:x1]
  //     - add the (x,y) coordinates to the output vector
  //
  // - how can you estimate the x coordinate at subpixel resolution ?
/*
    for (auto& coord : laserLineCoord) {
      coord.setZero();
    }
*/
    laserLineCoord.clear();
    QVector<int> filter(QVector<int>() << 1 << 4 << 6 << 4 << 1);
    QImage filteredImage = hw1::filterImage(inputImage, filter);

    //isolate red color
    for (int h = y0; h < y1; ++h)
    {
        for (int w = x0; w < x1; ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            int value = r - (g + b) / 2;
            if (value < 0)
                value = 0;

            filteredImage.setPixel(w, h, qRgb(value, value, value));
        }
    }

    //search average maximum of each row
    int sum = 0;
    for (int h = y0; h < y1; ++h)
    {
        int max = 0;
        for (int w = x0; w < x1; ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) > max)
                max = qRed(pixel);
        }
        sum += max;
    }
    double avg = sum / (y1 - y0);

    //apply threshold
    for (int h = y0; h < y1; ++h)
    {
        for (int w = x0; w < x1; ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) < 0.8 * avg)
                filteredImage.setPixel(w, h, qRgb(0, 0, 0));
        }
    }

    //search the maximum of each row
    for (int h = y0; h < y1; ++h)
    {
        int max = 0;
        int w0 = x0-1;
        for (int w = x0; w < x1; ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) > max)
            {
                w0 = w;
                max = qRed(pixel);
            }
        }
        if (w0 > x0-1)
        {
            Vector2d detectedPoint(w0, h);
            laserLineCoord.push_back(detectedPoint);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void hw3::detectLaser
(QImage const& inputImage, /* input */
 QVector<Vector2d>& laserLineCoord, /* output */
 const QPolygonF& boundingQuad) {

  // - same as the previous version but using
  //   QPolygonF::containsPoint() function to decide whether or not to
  //   add the detected point to the output vector
    /*
    for (auto& coord : laserLineCoord) {
        coord.setZero();
    }
*/
    laserLineCoord.clear();
    QVector<int> filter(QVector<int>() << 1 << 4 << 6 << 4 << 1);
    QImage filteredImage = hw1::filterImage(inputImage, filter);

    //isolate red color
    for (int h = 0; h < filteredImage.height(); ++h)
    {
        for (int w = 0; w < filteredImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            int value = r - (g + b) / 2;
            if (value < 0)
                value = 0;

            filteredImage.setPixel(w, h, qRgb(value, value, value));
        }
    }

    //search average maximum of each row
    int sum = 0;
    for (int h = 0; h < inputImage.height(); ++h)
    {
        int max = 0;
        for (int w = 0; w < inputImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) > max)
                max = qRed(pixel);
        }
        sum += max;
    }
    double avg = sum / inputImage.height();

    //apply threshold
    for (int h = 0; h < inputImage.height(); ++h)
    {
        for (int w = 0; w < inputImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) < 0.8 * avg)
                filteredImage.setPixel(w, h, qRgb(0, 0, 0));
        }
    }

    //search the maximum of each row
    for (int h = 0; h < inputImage.height(); ++h)
    {
        int max = 0;
        int w0 = -1;
        for (int w = 0; w < inputImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) > max)
            {
                w0 = w;
                max = qRed(pixel);
            }
        }
        if (w0 > -1)
        {
            Vector2d detectedPoint(w0, h);
            QPointF qPoint(detectedPoint.x(), detectedPoint.y());
            if (boundingQuad.containsPoint(qPoint, Qt::OddEvenFill)) {
                laserLineCoord.push_back(detectedPoint);
            }
        }
    }

}

//////////////////////////////////////////////////////////////////////
void hw3::checkerboardPlane
(Matrix3d const& R, /* input */
 Vector3d const& T, /* input */
 Vector4d & checkerboardPlane /* output */) {

  // with [r0 r1 r2] = R
  //
  // the implicit equation of the plane is 
  // r2^t(p-T) = 0
  //
  // the first three coordinates of of checkerboardPlane vector must
  // be equal to r2
    Vector3d r2 = R.col(2);
    checkerboardPlane.head<3>() = r2;
    checkerboardPlane(3) = -r2.dot(T);
}

//////////////////////////////////////////////////////////////////////
void hw3::triangulate
(Matrix3d const& K, /* input */
 Vector5d const& kc, /* input */
 Vector4d& planeCoefficients,
 QVector<Vector2d> const& imagePoints, /* input */
 QVector<Vector3d> & worldPoints /* output */ ) {

  // - derived from hw2::triangulate() but all the computation is done
  //   in normalized coordinates; i.e., the world coordinate system is
  //   the camera coordinate system
    Q_UNUSED(kc)
    worldPoints.clear();

    //triangulate
    for (auto &p : imagePoints) {

        Vector3d u(p.x(), p.y(), 1);
        Vector3d v = K.inverse() * u;

        double lambda = planeCoefficients[3]/(v[0]*planeCoefficients[0]+v[1]*planeCoefficients[1]+v[2]*planeCoefficients[2]);
        Vector3d pt = -lambda * v;

        worldPoints.push_back(pt);
    }
}

//////////////////////////////////////////////////////////////////////
void hw3::estimatePlane
(QVector<Vector3d> const& laserPoints, /* input */
 Vector4d & laserPlane /* output */) {

  laserPlane = Vector4d::Zero(); // << 0,0,0,0;
  int nPoints = laserPoints.size();

  // - fit a plane to the laserPoints in the least squares sense
  //   using the SVD method
  //
  // - if pc is the centroid of the points p_i in the vector
  //   laserPoints, build a matrix A of nPoints rows by 3 columns, and
  //   fill its i-th row with (p_i-pc)^t
  //
  // - the normal vector N of the laser plane is the right hand side
  //   singular vector of the matrix A corresponding to its minimum
  //   singular value
  //
  // - the implicit equation of the plane is N^t(p-pc)=0
  //
  // - compute and print the minimum square error
  Vector3d pc = Vector3d::Zero();
  for (const auto& point : laserPoints) {
      pc += point;
  }
  pc /= nPoints;

  MatrixXd A(nPoints, 3);
  for (int i = 0; i < nPoints; ++i) {
      A.row(i) = (laserPoints[i] - pc).transpose();
  }

  Eigen::JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
  Vector3d N = svd.matrixV().col(2);

  laserPlane.head<3>() = N;
  laserPlane(3) = -N.dot(pc);

  double e = 0.0;
  for (const auto& point : laserPoints) {
      double distance = N.dot(point) + laserPlane(3);
      e += distance * distance;
  }
  e /= nPoints;
  std::cout << "Minimum square error: " << e << std::endl;
}

//////////////////////////////////////////////////////////////////////
void hw3::estimateTurntableCoordSystem
(QVector<Matrix3d>  const& R_pose,                /* input */
 QVector<Vector3d>  const& T_pose,                /* input */
 Vector3d                & patternCenter,         /* input/output */
 Vector3d                & turntableWorldCenter,  /* output */
 QVector<Matrix3d>       & turntableWorldRotation /* output */) {

  int nPoses = R_pose.size();
  if(nPoses< 2 || nPoses!=T_pose.size()) return;

  // [r0i r1i r2i] = R_pose[i] ... 0<=i<R_pose.size()==T_pose.size()

  // - here we assume that r0i is direction of rotation of the turntable,
  //   and it is independent of i, i.e., r0i = r0 for all i
  //
  // - patternCenter(0) contains an input value
  //   corresponding to the x coord of points on the turntable
  //   surface, as measured in the corresponding pose
  //
  // - patternCenter(1) and patternCenter(2) are output values
  //   they measure the (y,z) coordinates of the center of rotation
  // - these coordinates should be the same for all the poses
  //
  // - if p=(px,py,pz)^2 represents the center of rotation in in
  //   normalized camera coordinates, and x=patternCenter(0) is the
  //   input value, and ti=T_pose[i], then for each value of i in the
  //   range 0<=i<nPoses the following equation should hold
  //
  //   p = x*r0 + y*r1i + z*r2i + ti
  //
  //   where p,y, and z are unknown, and everything else is known
  //
  // - he previous equation can be rewritten as
  //
  //   y * r1 + z * r2 - p = -(x*r0+ti) 
  //
  // - to solve the problem, let X=(y,z,px,py,pz)^t be the vector of unknowns
  //
  // - create a matrix A of 3*nPoses rows, and 5 columns, and a vector
  //   b of dimension 3*nPoses
  //
  // - for each value of i in the range 0<=i<nPoses
  //     - fill 3 rows of A with the submatrix [r1 r2 -I], where I is
  //       the identity matrix
  //     - fill 3 rows of the vector b with the vector -(x*r0+ti)
  //
  // - the problem is reduced to solving the system of 3*nPoses linear
  //   equations in 5 variables
  //
  //   A*X = b
  //
  // - since in general we will have at least two poses, we will have
  //   more equations than variables, and the system will not have a
  //   solution
  // - instead solve the equations in the least squares sense, by
  //   minimizing \|A*X-b\|^2
  // - note that there are no constraints on X here
  //   
  // - Eigen::JacobiSVD can be used to compute the SVD decomposition of the
  //   matrix A, and to solve the problem
  //
  // - finally, since the rotation [r0 r1i r2i] = R_pose[i] in the
  //   vector R_pose, all have the the vector r0 pointing down, and
  //   the vectors r1i and r2i spanning a plane paralel to the
  //   turntable top surface, you need to generate for each value of i
  //   a new rotaion matrix R_tt_i = [ r2i r1i -r0 ], and add it in
  //   the output vector turntableWorldRotation
  //
  // - this matrix R_tt_i, together with the translation vector
  //   R_tt=turntableWorldCenter define the pose of the world
  //   (turntable ) coordinate system for the image corresponding to
  //   the index i
  //
  // - not that in this coordinate system, the first two rows of the
  //   rotation matrix span the turntable top plane, and the third
  //   axis points up
  Vector3d r0 = R_pose[0].col(0);
  double x = patternCenter(0);

  MatrixXd A(3 * nPoses, 5);
  VectorXd b(3 * nPoses);

  for (int i = 0; i < nPoses; ++i) {
      Vector3d r1 = R_pose[i].col(1);
      Vector3d r2 = R_pose[i].col(2);
      Vector3d ti = T_pose[i];

      A.block<3, 5>(3 * i, 0) << r1, r2, -Matrix3d::Identity(); // [r1 r2 -I]
      b.segment<3>(3 * i) = -(x * r0 + ti);
  }

  Eigen::JacobiSVD<MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
  VectorXd X = svd.solve(b);

  patternCenter(1) = X(0);
  patternCenter(2) = X(1);

  turntableWorldCenter = Vector3d(X(2), X(3), X(4));

  turntableWorldRotation.clear();
  for (int i = 0; i < nPoses; ++i) {
      Vector3d r1 = R_pose[i].col(1);
      Vector3d r2 = R_pose[i].col(2);
      Matrix3d R_tt_i;

      R_tt_i.col(0) = r2;
      R_tt_i.col(1) = r1;
      R_tt_i.col(2) = -r0;

      turntableWorldRotation.push_back(R_tt_i);
  }
}

//////////////////////////////////////////////////////////////////////
void hw3::generateWireframeModel
(const double height, const double radius, const int sides,
 /* outputs */
 QVector<Vector3d>& worldCoord, QVector<int>& edge) {
  
  worldCoord.clear();
  edge.clear();

  if(height<=0.0 || radius<=0.0 || sides<3) return;

  // - to test the turntable coordinate system computed with the
  //   previous function you have to generate a wireframe gemetric
  //   model, which the application will render on the images, in a
  //   primitive augmented reality fashion
  //
  // - the wireframe model is defined by the vector of vertex coordinates 
  //   worldCoord, and by a number of edges
  //
  // - each vertex of the model has an index in the range
  //   0<=indx<worldCoord.size() - an edge is a unordered pair of
  //   different vertex indices (i,j) (edges (i,j) and (j,i) describe
  //   the same geometric entity) representing a straight line segment
  //   connecting vertices i and j in 3 space
  //
  // - the vertex coordinates are defined in the world (turntable)
  //   coordinate system
  //
  // - the index pairs (i,j) are stored as consecutive int values in
  //   the edge vector; as a result, the size of the output edge
  //   vector is equal to two times the number of edges
  //
  // - the application will project the vertex coordinates onto the
  //   image plane, using the pose of the turntable corresponding to
  //   the image being shown, and for each edge it will draw a line
  //   segment connecting the corresponding projected vertices
  //
  // - with the given parameters, you can build, for example, a
  //   polygonal cylinder of the given number of sides, radius and height
  //
  // - but you could build something else as a function of the given
  //   parameters as well
  //
  // - a vertex with z coordinate equal to 0 showld be rendered as
  //   laying on the turntable top plane; positive z is above the
  //   plane, and negative z is below the plane

  double zBottom = 0.0;
  double zTop = height;

  double angleStep = 2 * M_PI / sides;

  for (int i = 0; i < sides; ++i) {
      double angle = i * angleStep;
      double x = radius * cos(angle);
      double y = radius * sin(angle);

      worldCoord.push_back(Vector3d(x, y, zTop));

      worldCoord.push_back(Vector3d(x, y, zBottom));
  }

  for (int i = 0; i < sides; ++i) {
      int topIndex = 2 * i;
      int bottomIndex = topIndex + 1;
      int nextTopIndex = (topIndex + 2) % (2 * sides);
      int nextBottomIndex = (bottomIndex + 2) % (2 * sides);

      edge.push_back(topIndex);
      edge.push_back(nextTopIndex);

      edge.push_back(bottomIndex);
      edge.push_back(nextBottomIndex);

      edge.push_back(topIndex);
      edge.push_back(bottomIndex);
  }
}
