// Time-stamp: <2024-11-23 18:11:43 taubin>
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

#include <iostream>
#include <math.h>

#include "PanelCapture.hpp"

#include <QMessageBox>
// #include <QFileDialog>
#include <QSettings>
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QDir>
#include <QBuffer>
#include <QImageReader>
#include <QStringListModel>

#include "Application.hpp"
#include "MainWindow.hpp"

#include <util/GuiUtil.hpp>
// #include <util/MatrixUtil.hpp>

#define RPI_SCANNER_3D_THREAD

//////////////////////////////////////////////////////////////////////
// QWidget {

bool PanelCapture::_registered = PanelCapture::registerPanel();
bool PanelCapture::registerPanel() {
  qDebug("PanelCapture: register");
  return MainWindow::registerPanel
    ("Capture",
     [](QWidget *parent) -> QWidget*{ return new PanelCapture(parent); });
  return true;
}

//////////////////////////////////////////////////////////////////////
PanelCapture::PanelCapture(QWidget * parent) :
  QWidget(parent),
  _imageCount(0),
  _fpsTimer(),
  _model(),
  _currentImage(),
  _rpiScanner3D(nullptr),
  _rpiScanner3DSharedDir()
{

  qDebug("PanelCapture::PanelCapture() {");

  setupUi(this);

  QSettings config;

  progressBar->setValue(0);

  // image group
  QStringList rotationList; rotationList << "-90" << "0" << "90";
  GuiUtil::fillCombo(imageRotCombo, rotationList,
                      config.value("PanelCapture/rotation").toString());
  imageRotCombo->setCurrentText("0");

  // capture group
  stepsPerFrameSpin->setRange(1, 30000);
  countSpin->setRange(1, 1000);

  // TODO Tue May  4 22:53:20 2021
  // should be filled with info received from server
  pixelFormatCombo->addItem("JPEG");
  pixelFormatCombo->addItem("RGB3");
  pixelFormatCombo->addItem("YU12");
  pixelFormatCombo->addItem("MJPG");
  on_pixelFormatCombo_currentIndexChanged(0);

  // TODO Tue May  4 22:53:20 2021
  // should be filled with info received from server
  frameSizeCombo->addItem("1640x1232");
  frameSizeCombo->addItem("3280x2464");

  // load other settings
  loadSettings();

  // list view model
  imageList->setModel(&_model);
  auto selectionModel = imageList->selectionModel();
  connect(selectionModel, &QItemSelectionModel::currentChanged,
          this,           &PanelCapture::_on_currentImageChanged);

  auto mainWin  = getApp()->getMainWindow();
  if(mainWin) {
    auto glWidget = mainWin->getImgGLWidget();
    glWidget->clearImage();
  }

  updateNumberOfImagesPerFrame();
  updateAnglePerFrame();

  qDebug("}");
}

PanelCapture::~PanelCapture() {
  saveSettings();
  qDebug(" -- PanelCapture destroyed -- ");
}

void PanelCapture::on_scannerGroupCheck_stateChanged(int state) {
  scannerGroup->setVisible((state!=0));
}

void PanelCapture::on_cameraGroupCheck_stateChanged(int state) {
  cameraGroup->setVisible((state!=0));
}

void PanelCapture::on_captureGroupCheck_stateChanged(int state) {
  captureGroup->setVisible((state!=0));
}

void PanelCapture::on_imagesGroupCheck_stateChanged(int state) {
  imagesGroup->setVisible((state!=0));
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::updateState() {

  // TBD ...
  // enable/disable GUI components

}

void PanelCapture::showEvent(QShowEvent * event) {
    (void) event;

  auto mainWin = getApp()->getMainWindow();
  mainWin->showImgGLWidget(true);
  mainWin->showWrlGLWidget(true);

  connect(mainWin, &MainWindow::workDirectoryChanged,
          this,    &PanelCapture::_on_workDirectoryChanged);

  updateImageFiles();

  // show the first image in imageList
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(nImages>0) {
    imageList->setCurrentIndex(model->index(0,0));
    on_imageRotCombo_currentIndexChanged(0);
  }
}

void PanelCapture::hideEvent(QHideEvent *e) {
  if (e && !e->spontaneous()) {
    auto mainWin = getApp()->getMainWindow();
    disconnect(mainWin, &MainWindow::workDirectoryChanged,
               this,    &PanelCapture::_on_workDirectoryChanged);
  }
}

// } QWidget
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Settings {

void PanelCapture::loadSettings(void) {

  QSettings config;

  _rpiScanner3DSharedDir =
    config.value
    ("PanelCapture/rpiScanner3D/sharedDir","/Volumes").toString();

  // Scanner group
  QString hostname =
    config.value("PanelCapture/scannerHostname","").toString();
  RpiScanner3D rpiScanner3D;
  if(rpiScanner3D.setHostname(hostname)) {
    scannerHostname->setText(hostname);
    QString ipAddress = rpiScanner3D.ipAddress();
    scannerIpAddress->setText(ipAddress);
  }

  // Camera Group
  pixelFormatCombo->setCurrentText
    (config.value("PanelCapture/camera/pixelFormat","MJPG").toString());
  frameSizeCombo->setCurrentText
    (config.value("PanelCapture/camera/frameSize","1640x1232").toString());
  
  // Capture group
  stepsPerRevolutionLine->setText
    (config.value("PanelCapture/stepsPerRevolution",24480).toString());
  stepsPerFrameSpin->setValue
    (config.value("PanelCapture/stepsPerFrame",68).toUInt());
  updateAnglePerFrame();
  blankFrameCheck->setChecked
    (config.value("PanelCapture/blankFrame",true).toBool());
  laser1FrameCheck->setChecked
    (config.value("PanelCapture/laser1Frame",false).toBool());
  laser2FrameCheck->setChecked
    (config.value("PanelCapture/laser2Frame",false).toBool());
  countSpin->setValue
    (config.value("PanelCapture/count", 1).toUInt());
  bool sendImages =
    config.value("PanelCapture/serverSendImages",true).toBool();
  serverSendImagesCheck->setChecked(sendImages);
  serverSaveImagesCheck->setChecked(!sendImages);

  // Images group
  imageRotCombo->setCurrentText
    (config.value("PanelCapture/rotation","0").toString());

  bool scannerGroupVisible =
    config.value("PanelCapture/scannerGroup/visible",false).toBool();
  scannerGroupCheck->setChecked(scannerGroupVisible);
  scannerGroup->setVisible(scannerGroupVisible);

  bool cameraGroupVisible =
    config.value("PanelCapture/cameraGroup/visible",false).toBool();
  cameraGroupCheck->setChecked(cameraGroupVisible);
  cameraGroup->setVisible(cameraGroupVisible);

  bool captureGroupVisible =
    config.value("PanelCapture/captureGroup/visible",false).toBool();
  captureGroupCheck->setChecked(captureGroupVisible);
  captureGroup->setVisible(captureGroupVisible);

  bool imagesGroupVisible =
    config.value("PanelCapture/imagesGroup/visible",false).toBool();
  imagesGroupCheck->setChecked(imagesGroupVisible);
  imagesGroup->setVisible(imagesGroupVisible);
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::saveSettings(void) {
  QSettings config;

  config.setValue("PanelCapture/rpiScanner3D/sharedDir",
                  _rpiScanner3DSharedDir);

  // Scanner group
  config.setValue("PanelCapture/scannerHostname",
                  scannerHostname->text());
  // config.setValue("PanelCapture/scannerIpAddress",
  //                 scannerIpAddress->text());

  // Camera Group
  config.setValue("PanelCapture/camera/pixelFormat",
                  pixelFormatCombo->currentText());
  config.value("PanelCapture/camera/frameSize",
               frameSizeCombo->currentText());

  // Capture group
  config.setValue("PanelCapture/stepsPerFrame",
                  stepsPerRevolutionLine->text());
  config.setValue("PanelCapture/stepsPerFrame",
                  stepsPerFrameSpin->value());
  config.setValue("PanelCapture/blankFrame",
                  blankFrameCheck->isChecked());
  config.setValue("PanelCapture/laser1Frame",
                  laser1FrameCheck->isChecked());
  config.setValue("PanelCapture/laser2Frame",
                  laser2FrameCheck->isChecked());
  config.setValue("PanelCapture/count",
                  countSpin->value());

  bool saveImages = serverSendImagesCheck->isChecked();
  config.setValue("PanelCapture/serverSendImages",saveImages);

  // TODO Tue May  4 23:00:48 2021
  // frameSizeCombo
  // pixelFormatCombo

  // Images group
  config.setValue("PanelCapture/rotation",
                  imageRotCombo->currentText());

  bool scannerGroupVisible = scannerGroupCheck->isChecked();
  config.setValue("PanelCapture/scannerGroup/visible",
                  scannerGroupVisible);

  bool cameraGroupVisible = cameraGroupCheck->isChecked();
  config.setValue("PanelCapture/cameraGroup/visible",
                  cameraGroupVisible);

  bool captureGroupVisible = captureGroupCheck->isChecked();
  config.setValue("PanelCapture/captureGroup/visible",
                  captureGroupVisible);

  bool imagesGroupVisible = imagesGroupCheck->isChecked();
  config.setValue("PanelCapture/imagesGroup/visible",
                  imagesGroupVisible);

}

// } Settings
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Scanner Group {

void PanelCapture::on_scannerHostname_editingFinished() {
  RpiScanner3D rpiScanner3D;
  QString hostname = scannerHostname->text();
  if(rpiScanner3D.setHostname(hostname)) {
    scannerHostname->setText(hostname);
    QString ipAddress = rpiScanner3D.ipAddress();
    scannerIpAddress->setText(ipAddress);
  } else {
    scannerIpAddress->setText("");
  }
}

void PanelCapture::on_scannerIpAdress_editingFinished() {

  qDebug("PanelCapture::on_scannerIpAddress_editingFinished() {");

  QString ipAddress = scannerIpAddress->text();

  // qDebug(QString("  \"%1\"").arg(ipAddress));
  qDebug("  \"%s\"", qPrintable(ipAddress));

  // TODO Sun Apr 11 11:56:13 2021

  qDebug("}");
}

void PanelCapture::on_scannerTestButton_clicked() {
  RpiScanner3D rpiScanner3D;
  QString hostname = scannerHostname->text();
  if(rpiScanner3D.setHostname(hostname)==false) return;
  scannerHostname->setText(hostname);
  QString response;

  scannerCommandLine->setText("  \"time\"");

  response = rpiScanner3D.getServerTime();
  scannerResponseLine->setText(QString("  \"%1\"").arg(response));

  // TODO Tue May  4 23:00:48 2021
  // response = rpiScanner3D.getServerInfo("frame-sizes");
  // fill frameSizeCombo ...

  // response = rpiScanner3D.getServerInfo("pixel-formats");
  // fill pixelFormatCombo ...


}

// } Scanner Group
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Capture Group {

void PanelCapture::on_stepsPerFrameSpin_valueChanged(int value) {
    (void) value;
    updateAnglePerFrame();
}

void PanelCapture::on_anglePerFrameLine_editingFinished() {
  double stepsPerRevolution =
    static_cast<double>(stepsPerRevolutionLine->text().toInt());
  double degreesPerFrame = anglePerFrameLine->text().toDouble();
  if(degreesPerFrame<=0) return;
  int stepsPerFrame =
    static_cast<int>(std::floor(degreesPerFrame*stepsPerRevolution/360.0));
  stepsPerFrameSpin->setValue(stepsPerFrame);
}

void PanelCapture::updateAnglePerFrame() {
  double angleDegPerFrame = 0.0f;
  int stepsPerRevolution = stepsPerRevolutionLine->text().toInt();
  if(stepsPerRevolution>0) {
    int nStepsPerFrame = stepsPerFrameSpin->value();
    if(nStepsPerFrame<1) {
      stepsPerFrameSpin->setValue(1);
      nStepsPerFrame = 1;
    }

    double stepsPerDegree = static_cast<double>(stepsPerRevolution)/360.0;

    angleDegPerFrame = static_cast<double>(nStepsPerFrame)/stepsPerDegree;

  }
  char str[16];
  snprintf(str,16,"%.3f",angleDegPerFrame);
  // anglePerFrameLine->setText(QString("%1").arg(angleDegPerFrame,6,'g',2));
  anglePerFrameLine->setText(QString(str));
}

int PanelCapture::numberOfImagesPerFrame() const {
  int nImagesPerFrame = 0;
  if(blankFrameCheck->isChecked()) nImagesPerFrame++;
  if(laser1FrameCheck->isChecked()) nImagesPerFrame++;
  if(laser2FrameCheck->isChecked()) nImagesPerFrame++;
  return nImagesPerFrame;
}

void PanelCapture::updateNumberOfImagesPerFrame() {
  int nImagesPerFrame = numberOfImagesPerFrame();
  if(nImagesPerFrame==0) {
    blankFrameCheck->setChecked(true);
    nImagesPerFrame = 1;
  }
  imagesPerFrameLine->setText(QString("%1").arg(nImagesPerFrame));
}

void PanelCapture::on_blankFrameCheck_stateChanged() {
  updateNumberOfImagesPerFrame();
}

void PanelCapture::on_laser1FrameCheck_stateChanged() {
  updateNumberOfImagesPerFrame();
}

void PanelCapture::on_laser2FrameCheck_stateChanged() {
  updateNumberOfImagesPerFrame();
}

void PanelCapture::on_serverSendImagesCheck_stateChanged(int state) {
  serverSaveImagesCheck->setChecked((state==0));
}

void PanelCapture::on_serverSaveImagesCheck_stateChanged(int state) {
  serverSendImagesCheck->setChecked((state==0));
}

void PanelCapture::on_pixelFormatCombo_currentIndexChanged(int index) {
  (void)index;
  QString pixelFormat = pixelFormatCombo->currentText();
  cameraPreviewButton->setEnabled((pixelFormat=="JPEG" || pixelFormat=="MJPG"));
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::on_cameraPreviewButton_clicked(bool checked) {

  try {

    if(!checked) {

      // stop the current capture sequence
      if(_rpiScanner3D!=nullptr) _rpiScanner3D->stopCapturePreview();

    } else {

      std::cerr << "PanelCapture::on_cameraPreviewButton_clicked() {" << std::endl;

      progressBar->setRange(0,100);

      QString pixelFormat = pixelFormatCombo->currentText();
      std::cerr << "  pixelFormat" << qPrintable(pixelFormat) << std::endl;

      QString frameSize   = frameSizeCombo->currentText();
      std::cerr << "  frameSize" << qPrintable(frameSize) << std::endl;

      _rpiScanner3D = new RpiScanner3D;
      _rpiScanner3D->setModePreview();
      _rpiScanner3D->setPixelFormat(pixelFormat.toStdString().c_str());
      _rpiScanner3D->setFrameSize(frameSize.toStdString().c_str());

      QString hostname = scannerHostname->text();
      if(_rpiScanner3D->setHostname(hostname)==false) {
        throw new StrException("ERROR : rpiScanner3D.setHostname()");
      }

      scannerHostname->setText(hostname);

      connect(_rpiScanner3D, &RpiScanner3D::streamStarted,
              this, &PanelCapture::_on_streamStarted);
      connect(_rpiScanner3D, &RpiScanner3D::streamProgress,
              this, &PanelCapture::_on_streamProgress);
      connect(_rpiScanner3D, &RpiScanner3D::streamFinished,
              this, &PanelCapture::_on_streamFinished);

      _rpiScanner3D->start();

      std::cerr << "}" << std::endl;
    }

  } catch(StrException* e) {
    qDebug("  %s",e->what());
    delete e;
  }
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::on_captureButton_clicked(bool checked) {

  try {

    if(!checked) {

      // stop the current capture sequence
      if(_rpiScanner3D!=nullptr) _rpiScanner3D->stopCapturePreview();

    } else {

      std::cerr << "PanelCapture::on_captureButton_clicked() {" << std::endl;

      auto mainWin = getApp()->getMainWindow();
      QString scansDir = mainWin->getWorkDirectory();
      int i = scansDir.lastIndexOf("/");
      if(i<0) i = scansDir.lastIndexOf("\\"); // for windows
      if(i>0) scansDir.truncate(i);

      std::cerr << "  scansDir = \"" << qPrintable(scansDir) << "\""<< std::endl;

      // start a new capture sequence

      int laserMode=0;
      if(blankFrameCheck->isChecked())  laserMode |= 0x1;
      if(laser1FrameCheck->isChecked()) laserMode |= 0x2;
      if(laser2FrameCheck->isChecked()) laserMode |= 0x4;

      int stepsPerFrame = stepsPerFrameSpin->value();
      int numberOfFrames = countSpin->value();
      int numberOfImages = numberOfFrames*numberOfImagesPerFrame();
      int usecWait= 50000;

      progressBar->setRange(0,numberOfImages);

      QString pixelFormat = pixelFormatCombo->currentText();
      std::cerr << "  pixelFormat = " << qPrintable(pixelFormat) << std::endl;

      QString frameSize   = frameSizeCombo->currentText();
      std::cerr << "  frameSize = " << qPrintable(frameSize) << std::endl;

      _rpiScanner3D = new RpiScanner3D;
      _rpiScanner3D->setModeCapture();
      _rpiScanner3D->setPixelFormat(pixelFormat.toStdString().c_str());
      _rpiScanner3D->setFrameSize(frameSize.toStdString().c_str());
      _rpiScanner3D->setServerSaveImages(!serverSendImagesCheck->isChecked());
      _rpiScanner3D->setClientScansDir(scansDir.toStdString().c_str());

      QString hostname = scannerHostname->text();
      if(_rpiScanner3D->setHostname(hostname)==false) {
        throw new StrException("ERROR : rpiScanner3D.setHostname()");
      }

      scannerHostname->setText(hostname);

      _rpiScanner3D->setLaserMode(laserMode);
      _rpiScanner3D->setStepsPerFrame(stepsPerFrame);
      _rpiScanner3D->setNumberOfFrames(numberOfFrames);
      _rpiScanner3D->setUsecWait(usecWait);

      connect(_rpiScanner3D, &RpiScanner3D::captureStarted,
              this, &PanelCapture::_on_captureStarted);
      connect(_rpiScanner3D, &RpiScanner3D::captureProgress,
              this, &PanelCapture::_on_captureProgress);
      connect(_rpiScanner3D, &RpiScanner3D::captureFinished,
              this, &PanelCapture::_on_captureFinished);

      _rpiScanner3D->start();

      std::cerr << "}" << std::endl;
    }

  } catch(StrException* e) {
    qDebug("  %s",e->what());
    delete e;
  }
}

////////////////////////////////////////////////////////////////////////////////////
void PanelCapture::_on_captureEnable(const bool value) {
  scannerGroup->setEnabled(value);
  scannerGroupCheck->setEnabled(value);
  cameraGroupCheck->setEnabled(value);
  cameraGroup->setEnabled(value);

  captureGroupCheck->setEnabled(value);
  // captureGroup->setEnabled(value);
  stepsPerRevolutionLabel->setEnabled(value);
  stepsPerRevolutionLine->setEnabled(value);
  stepsPerFrameLabel->setEnabled(value);
  stepsPerFrameSpin->setEnabled(value);
  anglePerFrameLabel->setEnabled(value);
  anglePerFrameLine->setEnabled(value);
  imagesInFrameLabel->setEnabled(value);
  imagesInFrameLayout->setEnabled(value);
  blankFrameCheck->setEnabled(value);
  laser1FrameCheck->setEnabled(value);
  laser2FrameCheck->setEnabled(value);
  imagesPerFrameLine->setEnabled(value);
  countLabel->setEnabled(value);
  countSpin->setEnabled(value);
  serverSendImagesCheck->setEnabled(value);
  serverSaveImagesCheck->setEnabled(value);
  // captureButton

  imagesGroup->setEnabled(value);
  imagesGroupCheck->setEnabled(value);
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::_on_captureStarted(const char* localScanDir) {
  // std::cerr << "PanelCapture::_on_captureStarted() {\n";
  // std::cerr << "  localScanDir = \"" << localScanDir << "\"\n";

  char capture_log_path[PATH_LENGTH];
  memset(capture_log_path,0,PATH_LENGTH);
  snprintf(capture_log_path,PATH_LENGTH,"%s/capture.log",localScanDir);
  
  // std::cerr << "  capture_log_path = \"" << capture_log_path << "\""<< std::endl;

  FILE* fp = fopen(capture_log_path,"w");
  if(fp!=(FILE*)0) {
    fprintf(fp,"anglePerFrame %8.4f\n",anglePerFrameLine->text().toDouble());
    fclose(fp);
  }

  // std::cerr << "  capture.log file written"<< std::endl;
  
  auto mainWin = getApp()->getMainWindow();
  captureButton->setText("STOP");
  mainWin->setChangesEnabled(false);
  _on_captureEnable(false);
  QString scanDir = QString(localScanDir);
  mainWin->setWorkDirectory(scanDir);
  progressBar->setValue(0);
  QApplication::processEvents();

  // std::cerr << "}\n";
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::_on_captureProgress
(const char* imageFileName, QByteArray* imageByteArray) {
  // std::cerr << "PanelCapture::_on_captureProgress() {\n";
  // std::cerr << "  imageFileName = \""<< imageFileName <<"\"\n";

#ifdef DECODE_AND_INSERT
  // TODO Thu May  6 14:20:02 2021
  QBuffer imageBuffer(imageByteArray);
  QImageReader imageReader(&imageBuffer);
  if(imageReader.canRead()) { // "JPEG" && "MJPG"
    QImage         qImage = imageReader.read();
    QImage::Format format = qImage.format();
    int            width  = qImage.width();
    int            height = qImage.height();
    // std::cerr << "  image " << width << "x" << height << " "
    //           << format << std::endl;
  } else {
    // std::cerr << "  imageReader cannot read image\n";
  }
  delete imageByteArray;
  // TODO Tue May  4 16:54:36 2021
  // - instead of doing this, insert file name in imageList
#else // #ifndef DECODE_AND_INSERT
  (void) imageFileName;
  (void) imageByteArray;
  updateImageFiles();
#endif

  // select and display the last image in imageList
  QAbstractItemModel *model = imageList->model();
  int nRows = model->rowCount();
  if(nRows>0) {
    QModelIndex lastIndex = model->index(nRows-1,0);
    imageList->setCurrentIndex(lastIndex);
    progressBar->setValue(nRows);
    QApplication::processEvents();
  }
  // std::cerr << "}\n";
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::_on_captureFinished(void) {
  // std::cerr << "PanelCapture::_on_captureFinished(void) {\n";
  disconnect(_rpiScanner3D, &RpiScanner3D::captureStarted,
             this, &PanelCapture::_on_captureStarted);
  disconnect(_rpiScanner3D, &RpiScanner3D::captureProgress,
             this, &PanelCapture::_on_captureProgress);
  disconnect(_rpiScanner3D, &RpiScanner3D::captureFinished,
             this, &PanelCapture::_on_captureFinished);
  // QString response = _rpiScanner3D->getResponse();

  // CRASHES 2021/05/20
  // !!! // delete _rpiScanner3D;

  _rpiScanner3D = nullptr;
  _on_captureEnable(true);
  captureButton->setText("CAPTURE");
  captureButton->setChecked(false);
  progressBar->setValue(0);
  QApplication::processEvents();
  auto mainWin  = getApp()->getMainWindow();
  mainWin->setChangesEnabled(true);
  // std::cerr << "}\n";
}

////////////////////////////////////////////////////////////////////////////////////
void PanelCapture::_on_streamEnable(const bool value) {
  scannerGroup->setEnabled(value);
  scannerGroupCheck->setEnabled(value);
  cameraGroupCheck->setEnabled(value);

  pixelFormatLabel->setEnabled(value);
  pixelFormatCombo->setEnabled(value);
  frameSizeLabel->setEnabled(value);
  frameSizeCombo->setEnabled(value);

  captureGroup->setEnabled(value);
  captureGroupCheck->setEnabled(value);
  imagesGroup->setEnabled(value);
  imagesGroupCheck->setEnabled(value);
}  

//////////////////////////////////////////////////////////////////////
void PanelCapture::_on_streamStarted(void) {
  // std::cerr << "PanelCapture::_on_streamStarted() {\n";
  cameraPreviewButton->setText("STOP");
  auto mainWin = getApp()->getMainWindow();
  mainWin->setChangesEnabled(false);
  _on_streamEnable(false);
  progressBar->setValue(0);
  QApplication::processEvents();
  // std::cerr << "}\n";
}

void PanelCapture::_on_streamProgress
(const int image_no, QByteArray* imageByteArray) {
  // std::cerr << "PanelCapture::_on_streamProgress() {\n";
  // std::cerr << "  imageNumber = \""<< image_no <<"\"\n";
  QString pixFmt = pixelFormatCombo->currentText();
  int rot = imageRotCombo->currentText().toInt();
  if(rot<0) rot = -rot;
  else if(rot>0) rot = 360-rot;
  // previewButton only enable for pixFmt=="JPEG" ||  pixFmt=="MJPG"
  // if(pixFmt=="JPEG" ||  pixFmt=="MJPG") {
  QBuffer imageBuffer(imageByteArray);
  QImageReader imageReader(&imageBuffer);
  if(imageReader.canRead()) { // pixFmt=="JPEG" ||  pixFmt=="MPEG"
    QImage         qImage = imageReader.read();
    updateImage(qImage,rot);
  }
  // TODO Thu May  6 13:59:28 2021
  // server crashes in some cases because server is not able to process the data
  //
  //   } else { // YU12, RGB3, GRAY
  //     QString size = frameSizeCombo->currentText();
  //     int width=0,height=0;
  //     if(sscanf(size.toStdString().c_str(),"%dx%d",&width,&height)==2) {
  //       QImage qImage(width,height,QImage::Format_RGB32);
  //       char* data = imageByteArray->data();
  //       if(pixFmt=="YU12") {
  //         // convert to GRAY (temporarily)
  //         char* pix = data;
  //         for(int y=0;y<height;y++) {
  //           for(int x=0;x<width;x++) {
  //             int gr = 0xff & (*pix++);
  //             QColor c(gr,gr,gr);
  //             qImage.setPixelColor(x,y,c);
  //           }
  //         }
  //       } else if(pixFmt=="RGB3") {        
  //         std::cerr << "  pixFmt = RGB3" << std::endl;
  //         std::cerr << "  width  = "<< width << std::endl;
  //         std::cerr << "  height = "<< height << std::endl;
  //         std::cerr << "  bytes  = "<< 3*width*height << std::endl;
  //         int size = imageByteArray->size();
  //         std::cerr << "  size   = "<< size << std::endl;
  //         int stride = size/height; // why is stride>3*width ???
  //         // QColor c(r,g,b);
  //         char* pix;
  //         for(int y=0;y<height;y++) {
  //           pix = data+y*stride;
  //           for(int x=0;x<width;x++) {
  //             int r = 0xff & (*pix++);
  //             int g = 0xff & (*pix++);
  //             int b = 0xff & (*pix++);
  //             QColor c(r,g,b);
  //             qImage.setPixelColor(x,y,c);
  //           }
  //         }
  //       }
  //       updateImage(qImage,rot);
  //   }
  // }
  delete imageByteArray;
  progressBar->setValue((image_no%100));
  // std::cerr << "}\n";
}

void PanelCapture::_on_streamFinished(void) {
  // std::cerr << "PanelCapture::_on_streamFinished(void) {\n";
  disconnect(_rpiScanner3D, &RpiScanner3D::streamStarted,
             this, &PanelCapture::_on_streamStarted);
  disconnect(_rpiScanner3D, &RpiScanner3D::streamProgress,
             this, &PanelCapture::_on_streamProgress);
  disconnect(_rpiScanner3D, &RpiScanner3D::streamFinished,
             this, &PanelCapture::_on_streamFinished);
  // QString response = _rpiScanner3D->getResponse();
  delete _rpiScanner3D;
  _rpiScanner3D = nullptr;
  cameraPreviewButton->setText("PREVIEW");
  cameraPreviewButton->setChecked(false);
  progressBar->setValue(0);
  QApplication::processEvents();
  auto mainWin  = getApp()->getMainWindow();
  mainWin->setChangesEnabled(true);
  _on_streamEnable(true);
  // std::cerr << "}\n";
}

// } Capture Group
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Images Group {

void PanelCapture::_on_workDirectoryChanged() {
  updateImageFiles();
}

void PanelCapture::_on_currentImageChanged
(const QModelIndex & current, const QModelIndex & previous) {
  Q_UNUSED(current);
  Q_UNUSED(previous);
  QString rot = imageRotCombo->currentText();
  QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
  updateImage(qImg,0);
}

// updateImage() is called by
// _on_captureProgress
// _on_currentImageChanged

void PanelCapture::updateImage(QImage image, int rotation) {

  auto mainWin = getApp()->getMainWindow();
  if (!mainWin) {
    return;
  }
  auto glWidget = mainWin->getImgGLWidget();

  if (image.isNull()) {
    glWidget->clearImage();
    return;
  }

  // save last (QImage,rotation) pair
  _currentImage = image;

  //update image
  glWidget->setRotation(rotation);
  glWidget->setQImage(image);
  QTimer::singleShot(10, glWidget, SLOT(update()));
}

//////////////////////////////////////////////////////////////////////
void PanelCapture::updateImageFiles() {

  // MainWindow
  auto mainWin = getApp()->getMainWindow();

  // get files
  QStringList fileList;
  QDir dir(mainWin->getWorkDirectory());
  auto filters(QStringList() << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png");
  auto list =
    dir.entryInfoList(filters, QDir::Files, QDir::Name | QDir::IgnoreCase);
  foreach(auto file, list) {
    fileList.append(file.absoluteFilePath());
  }

  if(dir.exists("capture.log")) {

    char capture_log_path[PATH_LENGTH];
    memset(capture_log_path,0,PATH_LENGTH);
    snprintf(capture_log_path,PATH_LENGTH,"%s/capture.log",
             dir.absolutePath().toStdString().c_str());

    float anglePerFrame = 0.0f;
    FILE* fp = fopen(capture_log_path,"r");
    if(fp!=nullptr) {
      if(fscanf(fp,"anglePerFrame %f",&anglePerFrame)==1)
        anglePerFrameLine->setText(QString::number(anglePerFrame));
      fclose(fp);
    }

  } else {
    std::cerr << "  "
              << qPrintable(dir.absolutePath()) << "/capture.log does not exist"
              << std::endl;
  }

  // update the model
  GuiUtil::updateFilesItems(&_model, fileList);
}

void PanelCapture::on_imagesUpdateButton_clicked() {
  std::cerr << "PanelCapture::on_imagesUpdateButton_clicked() {\n";
  updateImageFiles();
  std::cerr << "}\n";
}

void PanelCapture::on_imagesFirstButton_clicked() {

  // TODO Fri Oct 25 23:12:40 2024

}

void PanelCapture::on_imagesLastButton_clicked() {

  // TODO Fri Oct 25 23:12:47 2024
  
}

void PanelCapture::on_imagesNextButton_clicked() {
  QAbstractItemModel *model = imageList->model();
  int nRows = model->rowCount();
  if(nRows>0) {
    QModelIndex indx = imageList->currentIndex();
    int row = 0;
    if(indx.isValid()) {
      row = indx.row()+1;
      if(row==nRows) row = 0;
    }
    QModelIndex nextIndex = model->index(row,0);
    imageList->setCurrentIndex(nextIndex);
  }
}

void PanelCapture::on_imagesPreviousButton_clicked() {
  QAbstractItemModel *model = imageList->model();
  int nRows = model->rowCount();
  if(nRows>0) {
    QModelIndex indx = imageList->currentIndex();
    int row = 0;
    if(indx.isValid()) {
      row = indx.row()-1;
      if(row<0) row = nRows-1;
    }
    QModelIndex nextIndex = model->index(row,0);
    imageList->setCurrentIndex(nextIndex);
  }
}

void PanelCapture::on_imageRotCombo_currentIndexChanged(int index) {
    (void)index;
  QString rot = imageRotCombo->currentText();
  QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
  updateImage(qImg,0);
}

// } Images Group
//////////////////////////////////////////////////////////////////////

