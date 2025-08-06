// Time-stamp: <2024-11-06 17:09:55 taubin>
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

#ifndef __PANEL_CALIBRATE_HPP__
#define __PANEL_CALIBRATE_HPP__

#include <QWidget>
#include <QImage>
#include <QStandardItemModel>
#include "ui_PanelCalibrate.h"
#include "CalibrationData.hpp"
#include <opencv2/core/core.hpp>

class QShowEvent;
class QHideEvent;

class PanelCalibrate : public QWidget, protected Ui_PanelCalibrate
{
  Q_OBJECT;

  static bool _registered;
  static bool registerPanel();

  static int _loopWaitMsec;

  QImage                     _currentImage;
  CalibrationData               _calibData;
  QStandardItemModel                _model;

  struct Reprojection { QVector<Vector2d> imagePoints; double rmsError;};
  QVector<Reprojection> _reprojection;

  void loadSettings(void);
  void saveSettings(void);

  void drawChessboardCorners();

  void updateImage(QImage image);
  void updateCurrentImage();
  void updateImageFiles(void);
  // void deleteAllImageFiles();

  void cameraCalibUpdate(void);
  void turntableCalibUpdate(void);

  void updateMinMaxError(void);

  void resetPanel(void);

  bool chessboardDetectCurrent(bool interactive);

  void updateState();

public:

  PanelCalibrate(QWidget * parent = 0);
  ~PanelCalibrate();

protected:

  void showEvent(QShowEvent * e);
  void hideEvent(QHideEvent * e);

  void enableChessboardListAddButton();
  int  imageListCheckedItemsCount();
  void imageListUncheckAllItems();
  int  imageListGetFrames();

  // called only by
  // on_chessboardDetectAndCalibrateButton_clicked();
  void cameraCalibrate();

protected slots:

  void _on_workDirectoryChanged();
  void _on_currentImageChanged
       (const QModelIndex & current, const QModelIndex & previous);

  void setGroupsEnabled(const bool value);

  // Images Group
  void on_imagesGroupCheck_stateChanged(int state);
  void on_imagesUpdateButton_clicked();
  void on_imagesNextButton_clicked();
  void on_imagesPreviousButton_clicked();
  void on_anglePerFrameLine_editingFinished();

  // Chessboard Group
  void on_chessboardGroupCheck_stateChanged(int state);
  void on_chessboardColsSpin_valueChanged(int value);
  void on_chessboardRowsSpin_valueChanged(int value);
  void on_chessboardCellWidthValue_textEdited(const QString &text);
  void on_chessboardCellHeightValue_textEdited(const QString &text);
  void on_chessboardListAddButton_clicked();
  void on_chessboardListDeleteButton_clicked();
  void on_chessboardsCombo_currentIndexChanged(int index);

  void on_chessboardDrawPointLine0_editingFinished();
  void on_chessboardDrawPointLine1_editingFinished();
  void on_chessboardDrawPointLine2_editingFinished();
  void on_chessboardDrawPointCheck_clicked();

  // Camera Group
  void on_cameraGroupCheck_stateChanged(int state);
  void on_drawCornersCheck_stateChanged();
  void on_drawChessboardPosesCheck_stateChanged();
  void on_chessboardDetectAndCalibrateButton_clicked(bool checked);
  void on_clearCalibrationButton_clicked();
  void on_imageRotCombo_currentIndexChanged(int index);

  // Turntable Group
  void on_indxMinSpin_valueChanged(int i);
  void on_indxMaxSpin_valueChanged(int i);
  void on_turntableGroupCheck_stateChanged(int state);
  void on_drawTurntableFrameCheck_stateChanged(int state);
  void on_turntableCalibrateButton_clicked();

  // Lasers Group
  void on_lasersGroupCheck_stateChanged(int state);
  void on_drawLaserBoundingQuadCheck_stateChanged(int state);
  void on_drawLaserDetectedPointsCheck_stateChanged(int state);
  void on_drawLaserTriangulatedPointsCheck_stateChanged(int state);
  void on_drawLaserPlaneCheck_stateChanged(int state);
  void on_laserEstimatePoseDetectButton_clicked(bool checked);
  void on_laserTriangulateButton_clicked(bool checked);
  void on_laserFitPlanesButton_clicked();

};

#endif //__PANEL_CALIBRATE_HPP__
