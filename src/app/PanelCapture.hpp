// Time-stamp: <2024-11-06 17:10:00 taubin>
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

#ifndef _PANEL_CAPTURE_HPP_
#define _PANEL_CAPTURE_HPP_

#include <QWidget>
#include <QImage>
#include <QElapsedTimer>
#include <QStandardItemModel>
#include <QAtomicInteger>
#include <QByteArray>

#include "ui_PanelCapture.h"

#include <geo/Vec.hpp>

#include "RpiScanner3D.hpp"

class PanelCapture : public QWidget, protected Ui_PanelCapture
{
  Q_OBJECT;

  static bool _registered;
  static bool registerPanel();

  void   loadSettings(void);
  void   saveSettings(void);

  void   getAndSetupCamera(void);
  void   updateCameraResolutions(void);
  void   changeCameraResolution(void);


  void   updateImage(QImage image, int rotation = 0);
  void   updateImageFiles();
  // void   deleteAllImageFiles();

  void   showEvent(QShowEvent * event);
  void   hideEvent(QHideEvent * event);

private:

  int    numberOfImagesPerFrame() const;
  void   updateNumberOfImagesPerFrame();
  void   updateAnglePerFrame();
  

public:

   PanelCapture(QWidget * parent = 0);
  ~PanelCapture();

  void updateState();

signals:

  void setInfoLabel(QString text);
  void abort();

public slots:

  // Capture Group
  void _on_captureEnable(const bool value);
  void _on_captureStarted(const char* localScanDir);
  void _on_captureProgress(const char* imageFileName, QByteArray* imageByteArray);
  void _on_captureFinished();

  void _on_streamEnable(const bool value);
  void _on_streamStarted(void);
  void _on_streamProgress(const int imageNumber, QByteArray* imageByteArray);
  void _on_streamFinished(void);

protected slots:

  void _on_workDirectoryChanged();

  // Scanner Group
  void on_scannerGroupCheck_stateChanged(int state);
  void on_scannerHostname_editingFinished();
  void on_scannerIpAdress_editingFinished();
  void on_scannerTestButton_clicked();

  // Camera Group
  void on_cameraGroupCheck_stateChanged(int state);
  void on_cameraPreviewButton_clicked(bool checked);
  void on_pixelFormatCombo_currentIndexChanged(int index);

  // Capture Group
  void on_captureGroupCheck_stateChanged(int state);
  void on_stepsPerFrameSpin_valueChanged(int value);
  void on_anglePerFrameLine_editingFinished();
  void on_blankFrameCheck_stateChanged();
  void on_laser1FrameCheck_stateChanged();
  void on_laser2FrameCheck_stateChanged();
  void on_captureButton_clicked(bool checked);
  void on_serverSendImagesCheck_stateChanged(int state);
  void on_serverSaveImagesCheck_stateChanged(int state);

  // Images Group
  void on_imagesGroupCheck_stateChanged(int state);
  void _on_currentImageChanged
  (const QModelIndex & current, const QModelIndex & previous);
  void on_imagesUpdateButton_clicked();
  void on_imagesFirstButton_clicked();
  void on_imagesPreviousButton_clicked();
  void on_imagesNextButton_clicked();
  void on_imagesLastButton_clicked();
  void on_imageRotCombo_currentIndexChanged(int index);

private:

  size_t                             _imageCount;
  QElapsedTimer                        _fpsTimer;
  QStandardItemModel                      _model;
  QImage                           _currentImage;
  RpiScanner3D*                    _rpiScanner3D;
  QString                 _rpiScanner3DSharedDir;
};

#endif // _PANEL_CAPTURE_HPP_
