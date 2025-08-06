// Time-stamp: <2024-11-10 11:01:50 taubin>
//
// Software developed for the Spring 2018 Brown University course
// ENGN 2502 3D Photography
// Copyright (c) 2016, Daniel Moreno and Gabriel Taubin
// Copyright (c) 2018, 2024, Gabriel Taubin
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

#ifndef __PANEL_SCAN_HPP__
#define __PANEL_SCAN_HPP__

#include <QWidget>
#include <QImage>
#include <QElapsedTimer>
#include <QStandardItemModel>
#include <QAtomicInteger>
#include <QByteArray>
#include <QPolygonF>

#include "ui_PanelScan.h"

#include <geo/Vec.hpp>

#include "CalibrationData.hpp"

#define SCANNER_CALIBRATION_FILENAME "Scanner_Calibration.txt"

class PanelScan : public QWidget, protected Ui_PanelScan
{
  Q_OBJECT;

  static bool _registered;
  static bool registerPanel();

  void   loadSettings(void);
  void   saveSettings(void);

  void   getAndSetupCamera(void);
  void   updateCameraResolutions(void);
  void   changeCameraResolution(void);

  int    getLaserMode();

  // CalibrationData & calibrationData();

  void   updateImage
  (QImage image, QVector<Vector2d>* pDetectedPoints = nullptr);
  void   updateCurrentImage();
  void   updateImageFiles();
  int    imageListGetFrames();

  void   showEvent(QShowEvent * event);
  void   hideEvent(QHideEvent * event);

private:

  int    numberOfImagesPerFrame() const;
  void   updateNumberOfImagesPerFrame();
  void   updateAnglePerFrame();
  

public:

   PanelScan(QWidget * parent = 0);
  ~PanelScan();

  void updateState();

signals:

  void setInfoLabel(QString text);
  void abort();

protected slots:

  void _on_workDirectoryChanged();

  // BoundingBox Group
  void on_bboxGroupCheck_stateChanged(int state);
  void on_bboxP0XLine_editingFinished();
  void on_bboxP0YLine_editingFinished();
  void on_bboxP1XLine_editingFinished();
  void on_bboxP1YLine_editingFinished();
  void on_bboxP2XLine_editingFinished();
  void on_bboxP2YLine_editingFinished();
  void on_bboxP3XLine_editingFinished();
  void on_bboxP3YLine_editingFinished();

  void on_bboxFileLoadButton_clicked();
  void on_bboxFileSaveButton_clicked();
  
  // Images Group
  void on_imagesGroupCheck_stateChanged(int state);
  void _on_currentImageChanged
  (const QModelIndex & current, const QModelIndex & previous);
  void on_imagesUpdateButton_clicked();
  void on_imagesNextButton_clicked();
  void on_imagesPreviousButton_clicked();
  void on_imageRotCombo_currentIndexChanged(int index);
  

  // Triangulation Group
  void on_triangulationGroupCheck_stateChanged(int state);
  void on_triangulationRunButton_clicked(bool checked);

  // 3D Scene Group
  void on_view3DGroupCheck_stateChanged(int state);

private:

  static int                       _loopWaitMsec;

  QStandardItemModel                      _model;
  QVector<int>                  _laserFrameStart;
  QVector<int>                _laserImageToFrame;
  QPolygonF                                _quad;
};

#endif // __PANEL_SCAN_HPP__
