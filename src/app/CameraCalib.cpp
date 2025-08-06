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

#include "CameraCalib.hpp"

#include <iostream>

#include <QDir>
#include <QFileInfo>

#include <matlab-io/MatlabIO.hpp>
#include <matlab-io/matlab_io_util.hpp>

CameraCalib::CameraCalib() :
  K(),
  kc(),
  nX(0),
  nY(0),
  dX(0.0),
  dY(0.0),
  imageCount(0U),
  imageSize(),
  imageData() {
}

void CameraCalib::clear(void) {
  K = Matrix3d::Zero();
  kc = Vector5d::Zero();
  imageCount = 0U;
  imageSize = QSize();
  imageData.clear();
}

bool CameraCalib::loadMatlabCalib(QString filename) {
  //reset previous data
  clear();

  // create a new reader
  MatlabIO matio;
  if (!matio.open(filename, "r")) { //error
    qDebug("[CameraCalib::loadMatlabCalib] Error: Mat file load failed: %s",
           qPrintable(filename));
    return false;
  }

  // read all of the variables in the file
  auto varMap = matio.read();

  // close the file
  matio.close();

  qDebug("[CameraCalib::loadMatlabCalib] debug: loaded '%s'", qPrintable(filename));

  // display the file info
  // matio.whos(varMap);

  //check
  if ( !varMap.contains("n_ima")
       || !varMap.contains("KK")
       || !varMap.contains("kc")
       || !varMap.contains("dX_default")
       || !varMap.contains("dY_default")) { //error
    qDebug("[CameraCalib::loadMatlabCalib] Error: Mat file does not contains camera calibration data: %s",
           qPrintable(filename));
    return false;
  }
  
  //calib data
  matlab::get2DMatrixDouble("KK", varMap, K.data(), 3, 3, matlab::ColMajor);
  matlab::get2DMatrixDouble("kc", varMap, kc.data(), 5, 1);
  nX = varMap["n_sq_x_1"].toInt();
  nY = varMap["n_sq_y_1"].toInt();
  dX = varMap["dX_default"].toDouble();
  dY = varMap["dY_default"].toDouble();
  
  //image data
  imageSize.setWidth(varMap["nx"].toInt());
  imageSize.setHeight(varMap["ny"].toInt());
  QString calib_name = varMap["calib_name"].toString();
  QString format_image = varMap["format_image"].toString();
  int N_slots = varMap["N_slots"].toInt();
  QDir dir = QFileInfo(filename).absoluteDir();

  imageCount = varMap["n_ima"].toUInt();
  imageData.resize(static_cast<int>(imageCount));
  for (size_t i = 0; i < imageCount; ++i) {

    auto m = i + 1;
    auto & data = imageData[i];

    // checkerboard pose (R,T)
    Vector3d rotVec;
    matlab::get2DMatrixDouble
      (QString("omc_%1").arg(m), varMap, rotVec.data(), 3, 1);
    data.R = rodrigues(rotVec);
    // translation T
    matlab::get2DMatrixDouble
      (QString("Tc_%1").arg(m), varMap, data.T.data(), 3, 1);

    // number of checkerboard corners
    int n_sq_x = varMap[QString("n_sq_x_%1").arg(m)].toInt();
    int n_sq_y = varMap[QString("n_sq_y_%1").arg(m)].toInt();
    int n = (1+n_sq_x)*(1+n_sq_y);

    // checkerboard corners in world coordinates
    data.worldPoints.resize(n);
    matlab::get2DMatrixDouble
      (QString("X_%1").arg(m), varMap,
       reinterpret_cast<double*>(data.worldPoints.data()), 3, n, matlab::ColMajor);

    // detected checkerboard corners in pixel coordinates
    data.imagePoints.resize(n);
    matlab::get2DMatrixDouble
      (QString("x_%1").arg(m), varMap,
       reinterpret_cast<double*>(data.imagePoints.data()), 2, n, matlab::ColMajor);

    // image file name
    data.imageName =
      dir.absoluteFilePath(QString("%1%2.%3")
                           .arg(calib_name)
                           .arg(m, N_slots, 10, QLatin1Char('0'))
                           .arg(format_image));

    // bounding quad

    double xMin = 0.0;
    double xMax = static_cast<double>(nX)*dX;
    double yMin = 0.0;
    double yMax = static_cast<double>(nY)*dY;

    // coords of bounding quad in checkerboard plane
    const Vector3d wPt_00(xMin,yMin,0.0);
    const Vector3d wPt_01(xMin,yMax,0.0);
    const Vector3d wPt_10(xMax,yMin,0.0);
    const Vector3d wPt_11(xMax,yMax,0.0);

    // look for bounding quad corners in worldPoints
    const QVector<Vector3d> & wPt = data.worldPoints;
    int nPts = wPt.size();
    int i00 = -1, i01 = -1, i10 = -1, i11 = -1;
    for(int i=0;i<nPts;i++) {
      const Vector3d & wPt_i = wPt[i];
      if(wPt_i == wPt_00) {
        i00 = i;
      } else if(wPt_i == wPt_01) {
        i01 = i;
      } else if(wPt_i == wPt_10) {
        i10 = i;
      } else if(wPt_i == wPt_11) {
        i11 = i;
      }
    }

    QPolygonF& iQuad = data.boundingQuad;
    iQuad.clear();

    const QVector<Vector2d> & iPt = data.imagePoints;
    if(i00>=0 && i01>=0 && i10>=0 && i10>=0) {
      const Vector2d& iPt_00 = iPt[i00];
      const Vector2d& iPt_01 = iPt[i01];
      const Vector2d& iPt_10 = iPt[i10];
      const Vector2d& iPt_11 = iPt[i11];

      // expand quad corners
      const double E0=0.0; // -0.025;
      const double E1=1.0; //  1.025;
      Vector2d q00 = (1.0-E0)*iPt_00+E0*iPt_11;
      Vector2d q11 = (1.0-E1)*iPt_00+E1*iPt_11;
      Vector2d q01 = (1.0-E0)*iPt_01+E0*iPt_10;
      Vector2d q10 = (1.0-E1)*iPt_01+E1*iPt_10;

      QPointF iQ00(q00[0],q00[1]);
      QPointF iQ01(q01[0],q01[1]);
      QPointF iQ11(q11[0],q11[1]);
      QPointF iQ10(q10[0],q10[1]);
      iQuad << iQ00 << iQ01 << iQ11 << iQ10 << iQ00; // isClosed()==true

    }
  }

  // debug
  std::cerr << "-------------------" << std::endl;
  std::cerr << "Camera Calibration Loaded:" << std::endl;
  std::cerr << " - K:\n" <<
    K << std::endl;
  std::cerr << " - kc:\n" <<
    kc << std::endl;
  std::cerr << " - imageCount: " <<
    imageCount << std::endl;
  std::cerr << " - imageSize: " <<
    imageSize.width() << "x" << imageSize.height() << std::endl;
  std::cerr << " - worldPoints: " <<
    (imageData.size() ? imageData.front().worldPoints.size() : 0) << std::endl;
  std::cerr << " - imagePoints: " <<
    (imageData.size() ? imageData.front().imagePoints.size() : 0) << std::endl;
  std::cerr << " - imageName: " <<
    (imageData.size() ? qPrintable(imageData.front().imageName) : "") << std::endl;
  std::cerr << "-------------------" << std::endl;

  return true;
}
