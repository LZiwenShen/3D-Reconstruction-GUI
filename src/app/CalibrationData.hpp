// Time-stamp: <2024-11-05 22:40:40 taubin>
//
// Software developed for the Spring 2016 Brown University course
// ENGN 2502 3D Photography
// Copyright (c) 2016, 2021 Daniel Moreno and Gabriel Taubin
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

#ifndef _CALIBRATION_DATA_HPP_
#define _CALIBRATION_DATA_HPP_

#include <QString>
#include <QVector>
#include <QPolygonF>
#include <QSize>
#include <util/MatrixUtil.hpp>

class CalibrationData {

public:

  bool         cameraIsCalibrated;

  bool      turntableIsCalibrated;

  bool     laserPointsAreDetected;
  bool     laserPosesAreEstimated;
  bool laserPointsAreTriangulated;
  bool       laserPlanesAreFitted;

  // chessboard
  int              chessboardCols;
  int              chessboardRows;
  double      chessboardCellWidth;
  double     chessboardCellHeight;

  // images
  size_t               imageCount;
  QSize                 imageSize;
  struct ImageData {
    QVector<Vector3d> worldPoints;
    QVector<Vector2d> imagePoints;
    Vector3d               rotVec; // R = rodrigues(rotVec);
    Vector3d                    T;
    QString             imageName;
  };
  QVector<ImageData>    imageData;
  double            anglePerFrame;
  int              imagesPerFrame;

  // camera
  double              cameraError;
  Matrix3d               camera_K; // 3x3 intrinsic parameters
  Vector5d              camera_kc; // lens distortion parameters
  
  // turntable
  Matrix3d             turntableFrame;
  Vector3d            turntableCenter;

  // turntable image data
  int                    imgMinCenter;
  int                    imgMaxCenter;
  Vector3d              patternCenter;
  QVector<Matrix3d> turntableRotation;
  
  // lasers
  QVector<int> laserFrameStart;
  QVector<int> laserImageToFrame;
  class LaserFrameData {
  public:
    // chessboard corners
    QVector<Vector3d> cornersChessboard;
    QVector<Vector2d> cornersCamera;
    // bounding quad
    QPolygonF quadCamera;
    // estimated pose
    Matrix3d R;
    Vector3d T;
    // laser 1 image
    QVector<Vector2d> detectedPoints1;
    QVector<Vector3d> triangulatedPoints1;
    // laser 2 image
    QVector<Vector2d> detectedPoints2;
    QVector<Vector3d> triangulatedPoints2;
  public:
    LaserFrameData();
    void clear();
  };
  QVector<LaserFrameData> laserFrameData;
  Vector4d            laserPlane1;
  Vector4d            laserPlane2;

public:
  
  CalibrationData();

  void clearCamera();
  void clearTurntable();
  void clearLasers();
  void clearImageData();
  void clear();
  void print();

};

#endif // _CALIBRATION_DATA_HPP_
