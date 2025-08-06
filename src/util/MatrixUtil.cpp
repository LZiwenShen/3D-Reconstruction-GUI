// Time-stamp: <2024-10-28 16:49:09 taubin>
//
// Software developed for the Spring 2016 Brown University course
// ENGN 2502 3D Photography
// Copyright (c) 2016, Daniel Moreno and Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL DANIEL MORENO NOR GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "MatrixUtil.hpp"

#include <Eigen/SVD>
#include <cmath>

using namespace Eigen;

Matrix3d MatrixUtil::rodrigues(Vector3d const& rotVec) {
  double eps = 1e-14;
  double theta = rotVec.norm();
  if (theta<eps) {
    return Matrix3d::Identity();
  }

  Vector3d u = rotVec/theta;
  double   c = std::cos(theta);
  double   s  = std::sin(theta);
  double   d = 1.0-c;
  Matrix3d U;
  U <<  0  , -u(2),  u(1),
       u(2),   0  , -u(0),
      -u(1),  u(0),   0  ; 
  Matrix3d uut = u*u.transpose();
  Matrix3d R = Matrix3d::Identity()*c + U*s + uut*d;

  return R;
}

// inverse rodrigues
Vector3d MatrixUtil::rodrigues(Matrix3d const& R,
                               bool const isOrthogonal) {
  Vector3d rotVec(0,0,0);

  // if isOrthogonal==true
  // R is guaranteed to be a rotation matrix,
  // i.e., orthogonal with R.determinant()==1

  Matrix3d Q;
  if(!isOrthogonal) { // check whether R is orthogonal or not

    // TODO Mon Oct 28 16:35:39 2024
    // - what to do if det(R)<0 ?
    // - perhaps set rotVec == (nan,nan,nan) ?

    double eps = 1e-14;
    Matrix3d M;
    M = R*R.transpose()-Matrix3d::Identity();
    double err = M.norm();
    if(err>eps) {
      // if R is not orthogonal
      // replace it by Q, the orthogonal minimizer of \|R-Q\|^2
      JacobiSVD<Matrix<double,Dynamic,Dynamic> > R_SVD(R,ComputeThinU|ComputeThinV);
      // auto S = R_SVD.singularValues();
      MatrixXd U = R_SVD.matrixU();
      MatrixXd V = R_SVD.matrixV();  
      Q = U*V.transpose();
    } else {
      Q = R;
    }
  }

  // Q   == I*c + U*s + uut*d; // where I == Matrix3d::Identity()
  // =>
  // Q^t == I*c - U*s + uut*d;
  // =>
  //                     [   0     -s*u(2)  s*u(1) ]
  // (Q-Q^t)/2 == s*U == [  s*u(2)   0     -s*u(0) ]
  //                     [ -s*u(1)  s*u(0)   0     ]

  Matrix3d sU = 0.5*(Q-Q.transpose());
  Vector3d su(sU(2,1),sU(0,2),sU(1,0));
  // compute s = sin(theta)
  double s  = su.norm(); // s>=0 => 0<=theta<=pi
  // compute c=cos(theta)
  // (Q+Q^t)/2 == I*c + uut*(1-c) == (I-uut)*c + uut;
  // =>
  // trace{Q} == trace{(Q+Q^t)/2} == c*{trace{I}-trace{uut}}+trace{uut}
  //          == c*{3-1}+1
  // =>
  // trace{Q} == 2*c+1;
  double c = (Q.trace()-1.0)/2.0;
  double theta = std::atan2(s,c);

  if(s>0.0) {
    rotVec = su*(theta/s);
  } else if(c<0) {
    // theta==pi
    if(Q(0,0)>0.0) {
      // Q == diag(1,-1,-1)
      rotVec = Vector3d(theta,0,0);
  } else if(Q(1,1)>0.0) {
      // Q == diag(-1,1,-1)
      rotVec = Vector3d(0,theta,0);
    } else /* if(Q(2,2)>0.0) */ {
      // Q == diag(-1,-1,1)
      rotVec = Vector3d(0,0,theta);
    }
  }
  
  return rotVec;
}
