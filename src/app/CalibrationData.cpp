// Time-stamp: <2024-11-06 14:52:32 taubin>
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

#include "CalibrationData.hpp"

#include <iostream>

#include <QDir>
#include <QFileInfo>

////////////////////////////////////////////////////////////////////////////////
CalibrationData::CalibrationData() :
  cameraIsCalibrated(false),
  turntableIsCalibrated(false),
  laserPointsAreDetected(false),
  laserPosesAreEstimated(false),
  laserPointsAreTriangulated(false),
  laserPlanesAreFitted(false),
  chessboardCols(0),
  chessboardRows(0),
  chessboardCellWidth(0.0),
  chessboardCellHeight(0.0),
  imageCount(0),
  imageSize(),
  imageData(),
  anglePerFrame(0.0),
  imagesPerFrame(1),
  cameraError(-1.0),
  camera_K(Matrix3d::Zero()),
  camera_kc(Vector5d::Zero()),
  turntableFrame(Matrix3d::Zero()),
  turntableCenter(Vector3d::Zero()),
  imgMinCenter(-1),
  imgMaxCenter(-1),
  patternCenter(Vector3d::Zero()),
  turntableRotation(),
  laserFrameStart(),
  laserImageToFrame(),
  laserFrameData(),
  laserPlane1(Vector4d::Zero()),
  laserPlane2(Vector4d::Zero()) {
  clear();
}

////////////////////////////////////////////////////////////////////////////////

void CalibrationData::clearCamera() {
  cameraIsCalibrated = false;
  cameraError = -1.0;
  imageCount = 0U;
  imageSize = QSize();
  imageData.clear();
  camera_K = Matrix3d::Zero();
  camera_kc = Vector5d::Zero();
}

void CalibrationData::clearTurntable() {
  turntableIsCalibrated = false;
  turntableFrame = Matrix3d::Zero();
  turntableRotation.clear();
  turntableCenter = Vector3d::Zero();
  patternCenter = Vector3d::Zero();
  imgMinCenter = -1;
  imgMaxCenter = -1;
}

void CalibrationData::clearLasers() {
  laserPointsAreDetected = false;
  laserPosesAreEstimated = false;
  laserPointsAreTriangulated = false;
  laserPlanesAreFitted = false;
  laserFrameStart.clear();
  laserImageToFrame.clear();
  laserFrameData.clear();
  laserPlane1 = Vector4d::Zero();
  laserPlane2 = Vector4d::Zero();
}

void CalibrationData::clearImageData() {
  imageCount = 0;
  imageSize = QSize();
  imageData.clear();
  imgMinCenter = -1;
  imgMaxCenter = -1;
  // patternCenter = Vector3d::Zero();
  // turntableRotation.clear();
  // anglePerFrame = 0.0;
  // imagesPerFrame = 1;
}

void CalibrationData::clear() {
  clearCamera();
  clearTurntable();
  clearLasers();
}

CalibrationData::LaserFrameData::LaserFrameData():
  cornersChessboard(),
  cornersCamera(),
  quadCamera(),
  R(Matrix3d::Identity()),
  T(Vector3d::Zero()),
  detectedPoints1(),
  triangulatedPoints1(),
  detectedPoints2(),
  triangulatedPoints2() {
}

void CalibrationData::LaserFrameData::clear() {
  cornersChessboard.clear();
  cornersCamera.clear();
  quadCamera.clear();
  R = Matrix3d::Identity();
  T = Vector3d::Zero();
  detectedPoints1.clear();
  triangulatedPoints1.clear();
  detectedPoints2.clear();
  triangulatedPoints2.clear();
}

//////////////////////////////////////////////////////////////////////
void CalibrationData::print() {

  std::cout << "CalibrationData::print() {" << std::endl;

  // QSize  imageSize;
  std::cout << "  imageSize = ["
            << imageSize.width() << "," << imageSize.height() << "]" << std::endl;

  // size_t imageCount;
  std::cout << "  imageCount = " << imageCount << "" << std::endl;
  std::cout << "  image file names {" << std::endl;
  for(uint indx=0;indx<imageCount;indx++) {
    const ImageData & iData = imageData.at(indx);
    std::cout << "    ["<< indx <<"] : "
              << qPrintable(iData.imageName) << "\"" << std::endl;

  }
  std::cout << "  }" << std::endl;

  if(cameraIsCalibrated) {
    std::cout << "  camera is calibrated" << std::endl;
    std::cout << "  chessboard detection and pose estimation {" << std::endl;
    for(uint indx=0;indx<imageCount;indx++) {
      const ImageData & iData = imageData.at(indx);

      if(iData.imageName==""         &&
         iData.imagePoints.size()==0 &&
         iData.worldPoints.size()==0) continue;

      std::cout << "    imageData[" << indx << "] = {" << std::endl;
      std::cout << "      nImagePoints = "
                << iData.imagePoints.size() << "" << std::endl;
      std::cout << "      nWorldPoints = "
                << iData.worldPoints.size() << "" << std::endl;
      std::cout << "      rotVec       = [ "
                << iData.rotVec(0) << " "
                << iData.rotVec(1) << " "
                << iData.rotVec(2) << " ]" << std::endl;
      std::cout << "      T            = [ "
                << iData.T(0) << " "
                << iData.T(1) << " "
                << iData.T(2) << " ]" << std::endl;
      std::cout << "    };" << std::endl;
    }
    std::cout << "  }" << std::endl;;


    std::cout << "  camera parameters" << std::endl;
    std::cout << "  K = [" << std::endl
              << camera_K << std::endl
              << "  ];" << std::endl;
    std::cout << "  kc = [ " << std::endl
              << camera_kc << std::endl
              << "  ];" << std::endl;
    std::cout << "  cameraError = "<< cameraError <<"" << std::endl;
  } else {
    std::cout << "  camera is not calibrated" << std::endl;
  }

  if(turntableIsCalibrated) {
    std::cout << "  turntable is calibrated" << std::endl;
    std::cout << "  imgMinCenter = " << imgMinCenter << std::endl;
    std::cout << "  imgMaxCenter = " << imgMaxCenter << std::endl;
    std::cout << "  turntableFrame = [" << std::endl
              << turntableFrame << std::endl
              << "  ];" << std::endl;
    std::cout << "  turntableCenter = [" << std::endl
              << turntableCenter << std::endl
              << "  ];" << std::endl;
    std::cout << "  patternCenter = [" << std::endl
              << patternCenter << std::endl
              << "  ];" << std::endl;
  } else {
    std::cout << "  turntable is not calibrated" << std::endl;
  }

  if(laserPlanesAreFitted) {
    std::cout << "  laser planes are fitted" << std::endl;
    std::cout << "  laserPlane1 = [" << std::endl
              << laserPlane1 << std::endl
              << "  ];" << std::endl;
    std::cout << "  laserPlane2 = [" << std::endl
              << laserPlane2 << std::endl
              << "  ];" << std::endl;
  } else {
    std::cout << "  laser planes are not calibrated" << std::endl;
  }
  
  std::cout << "}" << std::endl;
}
