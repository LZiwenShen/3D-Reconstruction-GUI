// Time-stamp: <2024-11-27 11:17:35 3dsha>
//
// Software developed for the Spring 2016 Brown University course
// ENGN 2502 3D Photography
// Copyright (c) 2016, Daniel Moreno and Gabriel Taubin
// Copyright (c) 2018, Gabriel Taubin
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

#include "PanelCalibrate.hpp"

#include <QShowEvent>
#include <QHideEvent>
#include <QMessageBox>
#include <QSettings>
#include <QPainter>
#include <QTimer>
#include <QDir>
#include <QStringListModel>
#include <QPolygonF>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/matx.hpp>

#include <img/ImgArgb.hpp>
#include <util/GuiUtil.hpp>
#include <util/StrException.hpp>
#include "Application.hpp"
#include "ScannerCalibData.hpp"

#ifdef HAVE_HOMEWORK
# include <homework/homework2.hpp>
# include <homework/homework3.hpp>
# include <homework/homework4.hpp>
#endif //HAVE_HOMEWORK

#ifndef PATH_LENGTH
#define PATH_LENGTH 4096
#endif


// static
int PanelCalibrate::_loopWaitMsec = 0; // 250;

//////////////////////////////////////////////////////////////////////
bool PanelCalibrate::_registered = PanelCalibrate::registerPanel();
bool PanelCalibrate::registerPanel() {
  qDebug("PanelCalibrate: register");
  return MainWindow::registerPanel
    ("Calibrate",
     [](QWidget *parent) -> QWidget*{ return new PanelCalibrate(parent); });
  return true;
}

//////////////////////////////////////////////////////////////////////
PanelCalibrate::PanelCalibrate(QWidget * parent) :
  QWidget(parent),
  _currentImage(),
  _calibData(),
  _model(),
  _reprojection() {
  setupUi(this);

  QSettings config;
  
  // image group
  QStringList rotationList; rotationList << "-90" << "0" << "90";
  GuiUtil::fillCombo
          (imageRotCombo, rotationList,
           config.value("PanelTurntable/rotation","0").toString());
  // imageRotCombo->setCurrentText("0");

  QStringListModel* qslModel = new QStringListModel();
  chessboardsCombo->setModel(qslModel);
  chesboardDetectedLine->setText("0");

  // load other settings
  loadSettings();

  // list view model
  imageList->setModel(&_model);
  auto selectionModel = imageList->selectionModel();
  connect(selectionModel, &QItemSelectionModel::currentChanged,
          this,           &PanelCalibrate::_on_currentImageChanged);

}

//////////////////////////////////////////////////////////////////////
PanelCalibrate::~PanelCalibrate() {
  saveSettings();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::loadSettings(void) {
  QSettings config;

  imageRotCombo->blockSignals(true);
  imageRotCombo->setCurrentText
    (config.value("PanelCalibrate/rotation","0").toString());
  imageRotCombo->blockSignals(false);

  anglePerFrameLine->blockSignals(true);
  anglePerFrameLine->setText
    (config.value("PanelCalibrate/anglePerFrame","0").toString());
  anglePerFrameLine->blockSignals(false);

  drawCornersCheck->blockSignals(true);
  drawCornersCheck->setChecked
    (config.value("PanelCalibrate/drawCorners", false).toBool());
  drawCornersCheck->blockSignals(false);

  drawChessboardPosesCheck->blockSignals(true);
  drawChessboardPosesCheck->setChecked
    (config.value("PanelCalibrate/drawChessboardPoses", false).toBool());
  drawChessboardPosesCheck->blockSignals(false);

  bool visible;

  imagesGroupCheck->blockSignals(true);
  visible =
    config.value("PanelCalibrate/imagesGroup/visible",false).toBool();
  imagesGroupCheck->setChecked(visible);
  imagesGroup->setVisible(visible);
  imagesGroupCheck->blockSignals(false);

  chessboardGroupCheck->blockSignals(true);
  visible =
    config.value("PanelCalibrate/chessboardGroup/visible",false).toBool();
  chessboardGroupCheck->setChecked(visible);
  chessboardGroup->setVisible(visible);
  chessboardGroupCheck->blockSignals(false);

  cameraGroupCheck->blockSignals(true);
  visible =
    config.value("PanelCalibrate/cameraGroup/visible",false).toBool();
  cameraGroupCheck->setChecked(visible);
  cameraGroup->setVisible(visible);
  cameraGroupCheck->blockSignals(false);
  
  turntableGroupCheck->blockSignals(true);
  visible =
    config.value("PanelCalibrate/turntableGroup/visible",false).toBool();
  turntableGroupCheck->setChecked(visible);
  turntableGroup->setVisible(visible);
  turntableGroupCheck->blockSignals(false);

  int chessboardCols =
    config.value("PanelCalibrate/chessboard/cols",7).toInt();
  chessboardColsSpin->setValue(chessboardCols);
  int chessboardRows =
    config.value("PanelCalibrate/chessboard/rows",9).toInt();
  chessboardRowsSpin->setValue(chessboardRows);
  double cellWidth = 
    config.value("PanelCalibrate/chessboard/width",20.0).toDouble();
  chessboardCellWidthValue->setText(QString("%1").arg(cellWidth));
  double cellHeight = 
    config.value("PanelCalibrate/chessboard/height",20.0).toDouble();
  chessboardCellHeightValue->setText(QString("%1").arg(cellHeight));
  QString chessboardDefault =
    QString("%1x%2-%3x%4")
    .arg(chessboardCols).arg(chessboardRows).arg(cellWidth).arg(cellHeight);
  QStringList savedChessboards =
    config.value("PanelCalibrate/chessboards/saved").value<QStringList>();
  if(savedChessboards.contains(chessboardDefault)==false)
    savedChessboards.insert(0,chessboardDefault);
  GuiUtil::fillCombo(chessboardsCombo,savedChessboards,chessboardDefault);
  enableChessboardListAddButton();

  chessboardDrawPointLine0->blockSignals(true);
  double chbPt0 = 
    config.value("PanelCalibrate/chessboard/point0",0.0).toDouble();
  chessboardDrawPointLine0->setText(QString::number(chbPt0));
  chessboardDrawPointLine0->blockSignals(false);

  chessboardDrawPointLine1->blockSignals(true);
  double chbPt1 = 
    config.value("PanelCalibrate/chessboard/point1",0.0).toDouble();
  chessboardDrawPointLine1->setText(QString::number(chbPt1));
  chessboardDrawPointLine1->blockSignals(false);

  chessboardDrawPointLine2->blockSignals(true);
  double chbPt2 = 
    config.value("PanelCalibrate/chessboard/point2",0.0).toDouble();
  chessboardDrawPointLine2->setText(QString::number(chbPt2));
  chessboardDrawPointLine2->blockSignals(false);

  chessboardDrawPointCheck->blockSignals(true);
  bool drawChbPt =
    config.value("PanelCalibrate/chessboard/drawPoint",false).toBool();
  chessboardDrawPointCheck->setChecked(drawChbPt);
  chessboardDrawPointCheck->blockSignals(false);

  indxMinSpin->blockSignals(true);
  indxMinSpin->setMinimum(0);
  indxMinSpin->setMaximum(1000);
  int indxMin = config.value("PanelCalibrate/turntable/indxMinSpin",0).toInt();
  indxMinSpin->setValue(indxMin);
  indxMinSpin->blockSignals(false);

  indxMaxSpin->blockSignals(true);
  indxMaxSpin->setMinimum(0);
  indxMaxSpin->setMaximum(1000);
  int indxMax = config.value("PanelCalibrate/turntable/indxMaxSpin",0).toInt();
  indxMaxSpin->setValue(indxMax);
  indxMaxSpin->blockSignals(false);

  drawTurntableFrameCheck->blockSignals(true);
  drawTurntableFrameCheck->setChecked
    (config.value("PanelCalibrate/turntable/drawTurntableFrame",false).toBool());
  drawTurntableFrameCheck->blockSignals(false);

  lasersGroupCheck->blockSignals(true);
  visible =
    config.value("PanelCalibrate/lasersGroup/visible",false).toBool();
  lasersGroupCheck->setChecked(visible);
  lasersGroup->setVisible(visible);
  lasersGroupCheck->blockSignals(false);

  drawLaserBoundingQuadCheck->blockSignals(true);
  drawLaserBoundingQuadCheck->setChecked
    (config.value("PanelCalibrate/drawLaserBoundingQuad",false).toBool());
  drawLaserBoundingQuadCheck->blockSignals(false);

  drawLaserDetectedPointsCheck->blockSignals(true);
  drawLaserDetectedPointsCheck->setChecked
    (config.value("PanelCalibrate/drawLaserDetectedPoints",false).toBool());
  drawLaserDetectedPointsCheck->blockSignals(false);

  drawLaserTriangulatedPointsCheck->blockSignals(true);
  drawLaserTriangulatedPointsCheck->setChecked
    (config.value("PanelCalibrate/drawLaserTriangulatedPoints",false).toBool());
  drawLaserTriangulatedPointsCheck->blockSignals(false);

  drawLaserPlaneCheck->blockSignals(true);
  drawLaserPlaneCheck->setChecked
    (config.value("PanelCalibrate/drawLaserPlane",false).toBool());
  drawLaserPlaneCheck->blockSignals(false);
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::saveSettings(void) {
  QSettings config;

  // std::cerr << "config.fileName()   = "
  //           << qPrintable(config.fileName()) << std::endl;

  config.setValue("PanelCalibrate/rotation",
                  imageRotCombo->currentText());

  config.setValue("PanelCalibrate/anglePerFrame",
                  anglePerFrameLine->text());

  config.setValue("PanelCalibrate/drawCorners",
                  drawCornersCheck->isChecked());
  config.setValue("PanelCalibrate/drawChessboardPoses",
                  drawChessboardPosesCheck->isChecked());
  config.setValue("PanelCalibrate/chessboard/cols",
                  chessboardColsSpin->value());
  config.setValue("PanelCalibrate/chessboard/rows",
                  chessboardRowsSpin->value());
  config.setValue("PanelCalibrate/chessboard/width",
                  chessboardCellWidthValue->text().toDouble());
  config.setValue("PanelCalibrate/chessboard/height",
                  chessboardCellHeightValue->text().toDouble());

  config.setValue("PanelCalibrate/imagesGroup/visible",
                  imagesGroupCheck->isChecked());
  config.setValue("PanelCalibrate/chessboardGroup/visible",
                  chessboardGroupCheck->isChecked());
  config.setValue("PanelCalibrate/cameraGroup/visible",
                  cameraGroupCheck->isChecked());
  config.setValue("PanelCalibrate/turntableGroup/visible",
                  turntableGroupCheck->isChecked());

  QStringList savedChessboards;
  int nChessboards = chessboardsCombo->count();
  for(int index=0;index<nChessboards;index++)
    savedChessboards.append(chessboardsCombo->itemText(index));
  config.setValue("PanelCalibrate/chessboards/saved",
                  QVariant::fromValue(savedChessboards));

  double chbPt0 = chessboardDrawPointLine0->text().toDouble();
  config.setValue("PanelCalibrate/chessboard/point0",chbPt0);
  double chbPt1 = chessboardDrawPointLine1->text().toDouble();
  config.setValue("PanelCalibrate/chessboard/point1",chbPt1);
  chessboardDrawPointLine1->setText(QString::number(chbPt1));
  double chbPt2 = chessboardDrawPointLine2->text().toDouble();
  config.setValue("PanelCalibrate/chessboard/point2",chbPt2);
  chessboardDrawPointLine2->setText(QString::number(chbPt2));
  bool drawChbPt = chessboardDrawPointCheck->isChecked();
  config.setValue("PanelCalibrate/chessboard/drawPoint",drawChbPt);

  int indxMin = indxMinSpin->value();
  config.setValue("PanelCalibrate/turntable/indxMinSpin",indxMin);
  int indxMax = indxMaxSpin->value();
  config.setValue("PanelCalibrate/turntable/indxMaxSpin", indxMax);
  config.setValue("PanelCalibrate/turntable/drawTurntableFrame",
                  drawTurntableFrameCheck->isChecked());

  config.setValue("PanelCalibrate/lasersGroup/visible",
                  lasersGroupCheck->isChecked());
  config.setValue("PanelCalibrate/drawLaserBoundingQuad",
                  drawLaserBoundingQuadCheck->isChecked());
  config.setValue("PanelCalibrate/drawLaserDetectedPoints",
                  drawLaserDetectedPointsCheck->isChecked());
  config.setValue("PanelCalibrate/drawLaserTriangulatedPoints",
                  drawLaserTriangulatedPointsCheck->isChecked());
  config.setValue("PanelCalibrate/drawLaserPlane",
                  drawLaserPlaneCheck->isChecked());
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::cameraCalibUpdate() {

  calibErrorLine->setText(QString::number(_calibData.cameraError));

  K00Line->setText(QString::number(_calibData.camera_K(0, 0)));
  K01Line->setText(QString::number(_calibData.camera_K(0, 1)));
  K02Line->setText(QString::number(_calibData.camera_K(0, 2)));

  K10Line->setText(QString::number(_calibData.camera_K(1, 0)));
  K11Line->setText(QString::number(_calibData.camera_K(1, 1)));
  K12Line->setText(QString::number(_calibData.camera_K(1, 2)));

  K20Line->setText(QString::number(_calibData.camera_K(2, 0)));
  K21Line->setText(QString::number(_calibData.camera_K(2, 1)));
  K22Line->setText(QString::number(_calibData.camera_K(2, 2)));
 
  kc0Line->setText(QString::number(_calibData.camera_kc[0]));
  kc1Line->setText(QString::number(_calibData.camera_kc[1]));
  kc2Line->setText(QString::number(_calibData.camera_kc[2]));
  kc3Line->setText(QString::number(_calibData.camera_kc[3]));
  kc4Line->setText(QString::number(_calibData.camera_kc[4]));
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::turntableCalibUpdate(void) {
  turntablePatternCenter0->setText
    (QString::number(_calibData.patternCenter[0]));
  turntablePatternCenter1->setText
    (QString::number(_calibData.patternCenter[1]));
  turntablePatternCenter2->setText
    (QString::number(_calibData.patternCenter[2]));
  
  turntableCenter0->setText
    (QString::number(_calibData.turntableCenter[0]));
  turntableCenter1->setText
    (QString::number(_calibData.turntableCenter[1]));
  turntableCenter2->setText
    (QString::number(_calibData.turntableCenter[2]));
  
  turntableAxis0->setText
    (QString::number(_calibData.turntableFrame(0,2)));
  turntableAxis1->setText
    (QString::number(_calibData.turntableFrame(1,2)));
  turntableAxis2->setText
    (QString::number(_calibData.turntableFrame(2,2)));
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::updateState() {
  QAbstractItemModel *model = imageList->model();
  int  nImages    = model->rowCount();
  imageCountLine->setText(QString::number(nImages));

  anglePerFrameLine->setText(QString::number(_calibData.anglePerFrame));

  bool chEnable = (nImages>0);
  chessboardDetectAndCalibrateButton->setEnabled(chEnable);

  // saveCalibrationButton->setEnabled(false); // TODO
  // loadCalibrationButton->setEnabled(false); // TODO

  bool cameraIsCalibrated = _calibData.cameraIsCalibrated;
  // clearCalibrationButton->setEnabled(true);
  turntableCalibrateButton->setEnabled(cameraIsCalibrated);

  bool laserPointsAreDetected =
    _calibData.laserPointsAreDetected;
  bool laserPosesAreEstimated =
    _calibData.laserPointsAreDetected;
  bool laserPointsAreTriangulated =
    _calibData.laserPointsAreTriangulated;

  laserEstimatePoseDetectButton->setEnabled(cameraIsCalibrated);
  
  laserTriangulateButton->setEnabled
    (laserPointsAreDetected && laserPosesAreEstimated);

  laserFitPlanesButton->setEnabled(laserPointsAreTriangulated);

  cameraCalibUpdate();  
  turntableCalibUpdate();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::showEvent(QShowEvent * e) {
  if (e && !e->spontaneous()) {
    auto mainWin = getApp()->getMainWindow();
    auto glWidget = mainWin->getImgGLWidget();
    glWidget->clearImage();
    glWidget->setRotation(0);
    
    connect(mainWin, &MainWindow::workDirectoryChanged,
            this, &PanelCalibrate::_on_workDirectoryChanged);

    updateImageFiles();
    updateState();

    mainWin->showImgGLWidget(true);
    mainWin->showWrlGLWidget(true);

    // show the first image in imageList
    QAbstractItemModel *model = imageList->model();
    int nImages = model->rowCount();
    if(nImages>0) {
      imageList->setCurrentIndex(model->index(0,0));
      on_imageRotCombo_currentIndexChanged(0);
    }
  }
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::hideEvent(QHideEvent * e) {
  if (e && !e->spontaneous()) {
    auto mainWin = getApp()->getMainWindow();
    disconnect(mainWin, &MainWindow::workDirectoryChanged,
               this,    &PanelCalibrate::_on_workDirectoryChanged);

    // if(_calibData.cameraIsCalibrated) saveMatlabCalib();
  }
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::_on_workDirectoryChanged() {

  // drawCornersCheck->setChecked(false);
  // drawChessboardPosesCheck->setChecked(false);
  // drawTurntableFrameCheck->setChecked(false);
  // drawLaserDetectedPointsCheck->setChecked(false);
  // drawLaserTriangulatedPointsCheck->setChecked(false);
  // drawLaserBoundingQuadCheck->setChecked(false);
  // drawLaserPlaneCheck->setChecked(false);

  _calibData.clearImageData();

  updateImageFiles();

  QString rot = imageRotCombo->currentText();
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(nImages>0) {
    imageList->setCurrentIndex(model->index(0,0));
    QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
    updateImage(qImg);
  }

  updateState();
}

void PanelCalibrate::on_imagesGroupCheck_stateChanged(int state) {
  imagesGroup->setVisible((state!=0));
  std::cerr << "panelCalibration->widh() = " << width() << std::endl;
}

void PanelCalibrate::on_chessboardGroupCheck_stateChanged(int state) {
  chessboardGroup->setVisible((state!=0));
  std::cerr << "panelCalibration->widh() = " << width() << std::endl;
}

void PanelCalibrate::on_cameraGroupCheck_stateChanged(int state) {
  cameraGroup->setVisible((state!=0));
  std::cerr << "panelCalibration->widh() = " << width() << std::endl;
}

void PanelCalibrate::on_turntableGroupCheck_stateChanged(int state) {
  turntableGroup->setVisible((state!=0));
  std::cerr << "panelCalibration->widh() = " << width() << std::endl;
}

void PanelCalibrate::on_indxMinSpin_valueChanged(int i) {
  (void)i;
  int iMin = indxMinSpin->value();
  int iMax = indxMaxSpin->value();
  if(iMin>iMax) indxMinSpin->setValue(iMax);
  _calibData.clearTurntable();
  updateState();
}

void PanelCalibrate::on_indxMaxSpin_valueChanged(int i) {
  (void)i;
  int iMin = indxMinSpin->value();
  int iMax = indxMaxSpin->value();
  if(iMin>iMax) indxMaxSpin->setValue(iMin);
  _calibData.clearTurntable();
  updateState();
}

void PanelCalibrate::on_drawTurntableFrameCheck_stateChanged(int state) {
  (void) state;
  updateCurrentImage();
  updateState();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::updateImageFiles() {
  auto mainWin = getApp()->getMainWindow();
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

    double anglePerFrame = 0.0;
    FILE* fp = fopen(capture_log_path,"r");
    if(fp!=nullptr) {
      fscanf(fp,"anglePerFrame %lf",&anglePerFrame);
      fclose(fp);
    }
    // ScannerCalibData& scannerCalibData = mainWin->getScannerCalibData();
    _calibData.anglePerFrame = anglePerFrame;
    anglePerFrameLine->setText(QString::number(anglePerFrame));

  } else {
    std::cerr << "  "
              << qPrintable(dir.absolutePath()) << "/capture.log does not exist"
              << std::endl;
  }
  
  GuiUtil::updateFilesItems(&_model/*imageList->model()*/, fileList);
  QAbstractItemModel *model = imageList->model();
  int  nImages = model->rowCount();
  indxMinSpin->setMinimum(0);
  indxMinSpin->setMaximum((nImages>0)?nImages-1:0);
  indxMaxSpin->setMinimum(0);
  indxMaxSpin->setMaximum((nImages>0)?nImages-1:0);
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_imageRotCombo_currentIndexChanged(int index) {
    (void)index;
  updateCurrentImage();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::_on_currentImageChanged
(const QModelIndex & current, const QModelIndex & previous) {
  Q_UNUSED(current);
  Q_UNUSED(previous);
  updateCurrentImage();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_imagesUpdateButton_clicked() {
  std::cerr << "PanelCalibrate::on_imagesUpdateButton_clicked() {\n";
  updateImageFiles();
  std::cerr << "}\n";
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_imagesNextButton_clicked() {
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(nImages>0) {
    QModelIndex indx = imageList->currentIndex();
    int row = 0;
    if(indx.isValid()) {
      row = indx.row()+1;
      if(row==nImages) row = 0;
    }
    QModelIndex nextIndex = model->index(row,0);
    imageList->setCurrentIndex(nextIndex);
  }
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_imagesPreviousButton_clicked() {
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(nImages>0) {
    QModelIndex indx = imageList->currentIndex();
    int row = 0;
    if(indx.isValid()) {
      row = indx.row()-1;
      if(row<0) row = nImages-1;
    }
    QModelIndex nextIndex = model->index(row,0);
    imageList->setCurrentIndex(nextIndex);
  }
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_anglePerFrameLine_editingFinished() {
  double anglePerFrame = anglePerFrameLine->text().toDouble();
  _calibData.anglePerFrame = anglePerFrame;
  updateState();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::updateImage(QImage image) {
  auto mainWin = getApp()->getMainWindow();
  if (!mainWin) {
    return;
  }
  auto glWidget = mainWin->getImgGLWidget();

  if (image.isNull()) {
    glWidget->clearImage();
    imageSizeLine->setText("");
    return;
  }

  imageSizeLine->setText(QString("%1x%2").arg(image.width()).arg(image.height()));
  
  int    chessboardRows       = chessboardRowsSpin->value();
  int    chessboardCols       = chessboardColsSpin->value();
  double chessboardCellWidth  = chessboardCellWidthValue->text().toDouble();
  double chessboardCellHeight = chessboardCellHeightValue->text().toDouble();

  _currentImage = image;
  int index = _currentImage.text("index").toInt();
  // QAbstractItemModel *model = imageList->model();
  // int nImages = (model!=nullptr)?model->rowCount():0;

  QPainter painter(&image);
  QPen pen;
  qreal  red,green,blue;
  QColor penColor;
  int radius   = 4;
  int diameter = 2*radius;

  // draw detected chessboard corners
  if(drawCornersCheck->isChecked() &&
     index<_calibData.imageData.size()) {

    QVector<Vector2d> const& imagePoints = _calibData.imageData[index].imagePoints;
    int nPoints = imagePoints.size();
    if(nPoints==chessboardRows*chessboardCols) {

      // TODO Thu May 20 23:21:28 2021
      // - use drawChessboardCorners() instead

      //   0 - tCol - chessboardCols-1
      //   |           |
      //  red  ------ yellow - 0
      //   |           |     - tRow
      // green ------ blue --- chessboardRows-1

      for(int row=0;row<chessboardRows;row++) {
        float tRow = ((float)row)/((float)(chessboardRows-1));
        for(int col=0;col<chessboardCols;col++) {
          float tCol = ((float)col)/((float)(chessboardCols-1));

          // color = (1-tRow)*(1-tCol)*red   + (1-tRow)*(  tCol)*yellow
          //       + (  tRow)*(1-tCol)*green + (  tRow)*(1-tCol)*blue

          // (r,g,b) = (1-tRow)*(1-tCol)*(1,0,0) + (1-tRow)*(  tCol)*(1,1,0)
          //         + (  tRow)*(1-tCol)*(0,1,0) + (  tRow)*(  tCol)*(0,0,1)

          red   = 1.0f-tRow;
          green = (1.0f-tRow)*tCol+tRow*(1.0f-tCol);
          blue  = tRow*tCol;
          penColor.setRgbF(red,green,blue);

          pen.setColor(penColor);
          pen.setWidth(5);
          painter.setPen(pen);

          const Vector2d & v = imagePoints[col+row*chessboardCols];
          painter.drawEllipse(v[0]-radius, v[1]-radius, diameter, diameter);

        } // for(int col=0;col<chessboardCols;col++)
      } // for(int row=0;row<chessboardRows;row++)
    
    } // if(nPoints==chessboardRows*chessboardCols)
  } // if(drawCornersCheck->isChecked() && index<_calibData.imageData.size())

  if(_calibData.cameraIsCalibrated &&
     drawChessboardPosesCheck->isChecked()) { // draw chessboard frame

    const Matrix3d& K         = _calibData.camera_K;
    const Vector5d& kc        = _calibData.camera_kc;
    auto const&     imageData = _calibData.imageData[index];
    Matrix3d        R         = MatrixUtil::rodrigues(imageData.rotVec);
    const Vector3d& T         = imageData.T;

    double L = 1.25*(chessboardCellWidth+chessboardCellHeight);

    Vector3d w0,w1,w2,w3;
    Vector2d p0,p1,p2,p3;
    w0 << 0.0,0.0,0.0;
    hw4::projectPoint(K,kc,R,T,w0,p0);
    w1 << L,0.0,0.0;
    hw4::projectPoint(K,kc,R,T,w1,p1);
    w2 << 0.0,L,0.0;
    hw4::projectPoint(K,kc,R,T,w2,p2);
    w3 << 0.0,0.0,L;
    hw4::projectPoint(K,kc,R,T,w3,p3);

    pen.setWidth(4);
    // with R = [ r1 r2 r3 ], L > edge lengh
    // draw red   line from T to T+L*r1
    pen.setColor(Qt::red);
    // pen.setWidth(1.5);
    painter.setPen(pen);
    painter.drawLine(p0.x(),p0.y(),p1.x(),p1.y());
    // draw green line from T to T+L*r2
    pen.setColor(Qt::green);
    // pen.setWidth(1.5);
    painter.setPen(pen);
    painter.drawLine(p0.x(),p0.y(),p2.x(),p2.y());
    // draw blue  line from T to T+L*r3
    pen.setColor(Qt::blue);
    // pen.setWidth(1.5);
    painter.setPen(pen);
    painter.drawLine(p0.x(),p0.y(),p3.x(),p3.y());

  } // if(_calibData.turntableIsCalibrated)

  if(_calibData.cameraIsCalibrated &&
     chessboardDrawPointCheck->isChecked()) {  

    double p0 = chessboardDrawPointLine0->text().toDouble();
    double p1 = chessboardDrawPointLine1->text().toDouble();
    double p2 = chessboardDrawPointLine2->text().toDouble();

    const Matrix3d& K         = _calibData.camera_K;
    const Vector5d& kc        = _calibData.camera_kc;
    auto const&     imageData = _calibData.imageData[index];
    Matrix3d        R         = MatrixUtil::rodrigues(imageData.rotVec);
    const Vector3d& T         = imageData.T;
    
    Vector3d chessboardPoint(p0,p1,p2);
    Vector2d imgPt(0,0);

    hw4::projectPoint(K,kc,R,T,chessboardPoint,imgPt);

    pen.setWidth(5);
    pen.setColor(QColorConstants::Svg::orange);
    painter.setPen(pen);

    painter.drawEllipse(imgPt[0]-radius, imgPt[1]-radius, diameter, diameter);
  }

  if(_calibData.turntableIsCalibrated &&
     drawTurntableFrameCheck->isChecked()) {

    // char str[32]; // used to print properly formatted floats

    std::cerr << "drawing turntable coord system {" << std::endl;

    // in camera coords
    Vector3d& turntableWorldCenter = _calibData.turntableCenter;
      
    std::cerr << "turntable center (camera coords) = [" << std::endl;
    std::cerr << turntableWorldCenter << std::endl;
    std::cerr << "];" << std::endl;

    double& dX = _calibData.chessboardCellWidth;
    double& dY = _calibData.chessboardCellHeight;
    double scale = 0.75*(dX+dY);

    // turntable frame points in world (turntable) coordinates
    QVector<Vector3d> wPt;

    Vector3d wPt0(Vector3d::Zero());
    Vector3d vX(2*scale,     0,       0);
    Vector3d vY(     0,2*scale,       0);
    Vector3d vZ(     0,    0, 10*scale);
    wPt.push_back(wPt0);
    wPt.push_back(wPt0+vX);
    wPt.push_back(wPt0+vY);
    wPt.push_back(wPt0+vZ);

    QVector<Vector2d> imgPt;

    Matrix3d& K       = _calibData.camera_K;
    Vector5d& kc      = _calibData.camera_kc;
    // Matrix3d& R_tt    = _calibData.turntableRotation[index];
    Matrix3d& R_tt    = _calibData.turntableFrame;

    int frame = index/_calibData.imagesPerFrame;
    double angleDeg = _calibData.anglePerFrame*static_cast<double>(frame);
    double angleRad = angleDeg*M_PI/180.0;
    double c = std::cos(angleRad);
    double s = std::sin(angleRad);
    Matrix3d R_frame;
    R_frame <<  c, s, 0,
               -s, c, 0,
                0, 0, 1;

    Matrix3d R_tt_frame;
    R_tt_frame = R_tt*R_frame;

    Vector3d& T_tt    = _calibData.turntableCenter;

    // hw4::projectPoints(K, kc, R_tt_frame, T_tt, wPt, imgPt);
    hw3::projectPoints(K, kc, R_tt_frame, T_tt, wPt, imgPt);

    const Vector2d& pt = imgPt[0];

    QPen pen;
      
    pen.setColor(Qt::yellow);      
    pen.setWidth(6);
    painter.setPen(pen);
    painter.drawPoint(pt.x(),pt.y());
    painter.drawEllipse(pt.x()-2, pt.y()-2,4,4);

    if(_calibData.imgMinCenter<= index && index<=_calibData.imgMaxCenter) {
      pen.setColor(Qt::red);      
      pen.setWidth(4);
      painter.setPen(pen);
      painter.drawLine(imgPt[0][0],imgPt[0][1],imgPt[1][0],imgPt[1][1]);
      
      pen.setColor(Qt::green);      
      pen.setWidth(4);
      painter.setPen(pen);
      painter.drawLine(imgPt[0][0],imgPt[0][1],imgPt[2][0],imgPt[2][1]);
    }

    pen.setColor(Qt::blue);      
    pen.setWidth(4);
    painter.setPen(pen);
    painter.drawLine(imgPt[0][0],imgPt[0][1],imgPt[3][0],imgPt[3][1]);

    std::cerr << "} drawing turntable coord system" << std::endl;
  }

  if(drawLaserDetectedPointsCheck->isChecked() &&
    _calibData.laserImageToFrame.size()>index) {
    int iFrame = _calibData.laserImageToFrame[index];
    int iLaser = index-_calibData.laserFrameStart[iFrame];
    if(iLaser>0) {
      CalibrationData::LaserFrameData&
        lfd = _calibData.laserFrameData[iFrame];
      QVector<Vector2d>& detectedPoints =
        (iLaser==1)?lfd.detectedPoints1:lfd.detectedPoints2;
      if(detectedPoints.size()>0) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(2);
        painter.setPen(pen);
        for(int iPt=0;iPt<detectedPoints.size();iPt++) {
          const Vector2d& pt = detectedPoints[iPt];
          painter.drawPoint(pt.x(),pt.y());
          painter.drawEllipse(pt.x()-1, pt.y()-1,3,3);
        }
      }
    }
  }

  if(drawLaserTriangulatedPointsCheck->isChecked() &&
    _calibData.laserImageToFrame.size()>index) {
    int iF = _calibData.laserImageToFrame[index];  // frame number
    int iL = index-_calibData.laserFrameStart[iF]; // 0,1,2
    if(iL>0) {

      Matrix3d& K  = _calibData.camera_K;
      Vector5d& kc = _calibData.camera_kc;

      int nFrames = _calibData.laserFrameData.size();;
      for(int iFrame=0;iFrame<nFrames;iFrame++) {
        CalibrationData::LaserFrameData&
          lfd = _calibData.laserFrameData[iFrame];
        QVector<Vector3d>& triangulatedPoints =
          (iL==1)?lfd.triangulatedPoints1:lfd.triangulatedPoints2;
        if(triangulatedPoints.size()>0) {

          QVector<Vector2d> projPoints;
          hw3::projectPoints(K,kc,triangulatedPoints,projPoints);

          QPen pen;
          // pen.setColor(Qt::green);
          pen.setColor(QColorConstants::Svg::tomato);
          pen.setWidth(2);
          painter.setPen(pen);
          for(int iPt=0;iPt<projPoints.size();iPt++) {
            const Vector2d& pt = projPoints[iPt];
            painter.drawPoint(pt.x(),pt.y());
            painter.drawEllipse(pt.x()-1, pt.y()-1,3,3);
          }
        }
      }
    }
  }

  if(drawLaserBoundingQuadCheck->isChecked() &&
    _calibData.laserImageToFrame.size()>index) {
    int iF = _calibData.laserImageToFrame[index];  // frame number
    int iL = index-_calibData.laserFrameStart[iF]; // 0,1,2
    if(iL>0) {
      CalibrationData::LaserFrameData& lfd = _calibData.laserFrameData[iF];
      QPolygonF& quad = lfd.quadCamera;
      pen.setColor(Qt::yellow);
      pen.setWidth(2);
      painter.setPen(pen);
      painter.drawPolygon(quad);
    }
  }
  
  if(drawLaserPlaneCheck->isChecked() && _calibData.laserPlanesAreFitted) {
    int iF = _calibData.laserImageToFrame[index];  // frame number
    int iL = index-_calibData.laserFrameStart[iF]; // 0,1,2

    if(iL>0) {
      CalibrationData::LaserFrameData& lfd = _calibData.laserFrameData[iF];

      int        chessboardRows = chessboardRowsSpin->value();
      int        chessboardCols = chessboardColsSpin->value();
      cv::Size   cornerCount(chessboardCols,chessboardRows);
    
      double     cellWidth  = chessboardCellWidthValue->text().toDouble();
      double     cellHeight = chessboardCellHeightValue->text().toDouble();
      cv::Size2f cornerSize(cellWidth,cellHeight);

      double x0 = -cellWidth;
      double x1 = static_cast<double>(chessboardCols)*cellWidth;
      double y0 = -cellHeight;
      double y1 = static_cast<double>(chessboardRows)*cellHeight;
      double z0 = -3.0*(cellWidth+cellHeight);
      double z1 = 0.0;

      QVector<Vector3d> wPt;
      wPt.push_back(Vector3d(x0,y0,z0)); // 0
      wPt.push_back(Vector3d(x0,y0,z1)); // 1
      wPt.push_back(Vector3d(x0,y1,z0)); // 2
      wPt.push_back(Vector3d(x0,y1,z1)); // 3
      wPt.push_back(Vector3d(x1,y0,z0)); // 4
      wPt.push_back(Vector3d(x1,y0,z1)); // 5
      wPt.push_back(Vector3d(x1,y1,z0)); // 6
      wPt.push_back(Vector3d(x1,y1,z1)); // 7

      
      Matrix3d& K  = _calibData.camera_K;
      Vector5d& kc = _calibData.camera_kc;
      Matrix3d& R  = lfd.R;
      Vector3d& T  = lfd.T;

      QVector<Vector2d> iPt;
      hw3::projectPoints(K, kc, R, T, wPt, iPt);

      QPen pen;
      QVector<int> edge;

      if(true) { // draw bounding box
        pen.setColor(Qt::yellow);
        pen.setWidth(2);
        painter.setPen(pen);
        
        // x edges
        edge.push_back(0); edge.push_back(4);
        edge.push_back(1); edge.push_back(5);
        edge.push_back(2); edge.push_back(6);
        edge.push_back(3); edge.push_back(7);
        // y edges
        edge.push_back(0); edge.push_back(2);
        edge.push_back(1); edge.push_back(3);
        edge.push_back(4); edge.push_back(6);
        edge.push_back(5); edge.push_back(7);
        // z edges
        edge.push_back(0); edge.push_back(1);
        edge.push_back(2); edge.push_back(3);
        edge.push_back(4); edge.push_back(5);
        edge.push_back(6); edge.push_back(7);
        
        int i,j,k;
        for(i=0;i<12;i++) {
          j = edge[2*i  ];
          k = edge[2*i+1];
          painter.drawLine(iPt[j][0],iPt[j][1],iPt[k][0],iPt[k][1]);
        }
      }

      if(true) { // draw laser plane
        // laser plane only intersects the y edges ?
        pen.setColor(Qt::red);
        pen.setWidth(4);
        painter.setPen(pen);

        // bounding box corners in normalized camera coordinates
        QVector<Vector3d> cPt;
        for(int i=0;i<8;i++) {
          Vector3d  cp = R * wPt[i] + T;
          cPt.push_back(cp);
        }

        // evaluate the laser plane implicit function on the bounding
        // box corners; bounding box vertices must be in camera coordinates

        Vector4d& lPlane =
          (iL==1)?_calibData.laserPlane1:_calibData.laserPlane2;

        QVector<double> f;
        for(int i=0;i<8;i++) {
          // bounding box corner in camera coordinates
          Vector3d& cp = cPt[i];
          // evaluate linear function
          double    fi =
            lPlane[0]*cp[0]+lPlane[1]*cp[1]+lPlane[2]*cp[2]+lPlane[3];
          f.push_back(fi);
        }

        // corners of quad = intersection of laser plane and bounding box
        QVector<Vector3d> lPt;
        double t;
        // y edges
        // (0,2) --- 0
        t = f[2]/(f[2]-f[0]);
        lPt.push_back(t*cPt[0]+(1.0-t)*cPt[2]);
        // (1,3) --- 1
        t = f[3]/(f[3]-f[1]);
        lPt.push_back(t*cPt[1]+(1.0-t)*cPt[3]);
        // (4,6) --- 2
        t = f[6]/(f[6]-f[4]);
        lPt.push_back(t*cPt[4]+(1.0-t)*cPt[6]);
        // (5,7) --- 3
        t = f[7]/(f[7]-f[5]);
        lPt.push_back(t*cPt[5]+(1.0-t)*cPt[7]);

        // project quad corners onto image plane
        iPt.clear();
        hw3::projectPoints(K, kc, lPt, iPt);

        edge.clear();
        edge.push_back(0); edge.push_back(2);
        edge.push_back(2); edge.push_back(3);
        edge.push_back(3); edge.push_back(1);
        edge.push_back(1); edge.push_back(0);
        
        int i,j,k;
        for(i=0;i<4;i++) {
          j = edge[2*i  ];
          k = edge[2*i+1];
          painter.drawLine(iPt[j][0],iPt[j][1],iPt[k][0],iPt[k][1]);
        }
      }      
    }

  }
  
  //update image
  glWidget->setQImage(image);
  QTimer::singleShot(10, glWidget, SLOT(update()));
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::updateCurrentImage() {
  QString rot = imageRotCombo->currentText();
  QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
  updateImage(qImg);
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::resetPanel(void) {
  auto mainWin = getApp()->getMainWindow();
  if (mainWin) { mainWin->getImgGLWidget()->clearImage(); }
  _model.clear();
  _calibData.clear();
  _reprojection.clear();
  updateState();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::drawChessboardCorners() {

  int chessboardRows = chessboardRowsSpin->value();
  int chessboardCols = chessboardColsSpin->value();

  int index = _currentImage.text("index").toInt();
  CalibrationData::ImageData & data = _calibData.imageData[index];
  
  int nDetectedCorners = data.imagePoints.size();
  if(nDetectedCorners<chessboardRows*chessboardCols)
      return;

  QPainter painter(&_currentImage);
  QPen   pen;
  qreal  red,green,blue;
  QColor penColor;
  int    radius   = 4;
  int    diameter = 2*radius;
  int    d0,d1,d2,d3,x0,y0,x1,y1;

  //   0 - tCol - chessboardCols-1
  //   |           |
  //  red  ------ yellow - 0
  //   |           |     - tRow
  // green ------ blue --- chessboardRows-1

  for(int row=0;row<chessboardRows;row++) {
    float tRow = ((float)row)/((float)(chessboardRows-1));
    for(int col=0;col<chessboardCols;col++) {
      float tCol = ((float)col)/((float)(chessboardCols-1));

      // color = (1-tRow)*(1-tCol)*red   + (1-tRow)*(  tCol)*yellow
      //       + (  tRow)*(1-tCol)*green + (  tRow)*(1-tCol)*blue

      // (r,g,b) = (1-tRow)*(1-tCol)*(1,0,0) + (1-tRow)*(  tCol)*(1,1,0)
      //         + (  tRow)*(1-tCol)*(0,1,0) + (  tRow)*(  tCol)*(0,0,1)

      red   = 1.0f-tRow;
      green = (1.0f-tRow)*tCol+tRow*(1.0f-tCol);
      blue  = tRow*tCol;
      penColor.setRgbF(red,green,blue);

      pen.setColor(penColor);
      pen.setWidth(8);
      painter.setPen(pen);

      Vector2d & v = data.imagePoints[col+row*chessboardCols];
      painter.drawEllipse(v[0]-radius, v[1]-radius, diameter, diameter);

      if(false /* row==0 */) {
        // search for bottom of chessboard point along this line

        Vector2d & w = data.imagePoints[col+1*chessboardCols];
        d0 = static_cast<int>(1.3*(v[0]-w[0])); x0 = v[0]+d0;
        d1 = static_cast<int>(1.3*(v[1]-w[1])); y0 = v[1]+d1;
        d2 = static_cast<int>(2.7*(v[0]-w[0])); x1 = v[0]+d2;
        d3 = static_cast<int>(2.7*(v[1]-w[1])); y1 = v[1]+d3;

        pen.setWidth(2.0);
        painter.setPen(pen);
        painter.drawLine(x0,y0,x1,y1);

      } // endif(row==0)

    } // for(int col=0;col<chessboardCols;col++)
  } // for(int row=0;row<chessboardRows;row++)
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardColsSpin_valueChanged(int value) {
  (void)value;
  enableChessboardListAddButton();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardRowsSpin_valueChanged(int value) {
  (void)value;
  enableChessboardListAddButton();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardCellWidthValue_textEdited(const QString &text) {
  (void)text;
  enableChessboardListAddButton();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardCellHeightValue_textEdited(const QString &text) {
  (void)text;
  enableChessboardListAddButton();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::enableChessboardListAddButton() {
  int     cols   = chessboardColsSpin->value();
  int     rows   = chessboardRowsSpin->value();
  double  width  = chessboardCellWidthValue->text().toDouble();
  double  height = chessboardCellHeightValue->text().toDouble();
  QString chessboardCurrent =
    QString("%1x%2-%3x%4").arg(cols).arg(rows).arg(width).arg(height);
  int index = chessboardsCombo->findText(chessboardCurrent);
  chessboardListAddButton->setEnabled((index<0));
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardListAddButton_clicked() {
  int     cols   = chessboardColsSpin->value();
  int     rows   = chessboardRowsSpin->value();
  double  width  = chessboardCellWidthValue->text().toDouble();
  double  height = chessboardCellHeightValue->text().toDouble();
  QString chessboardCurrent =
    QString("%1x%2-%3x%4").arg(cols).arg(rows).arg(width).arg(height);
  int index = chessboardsCombo->findText(chessboardCurrent);
  if(index<0) {
    chessboardsCombo->addItem(chessboardCurrent);
    index = chessboardsCombo->findText(chessboardCurrent);
  }
  chessboardsCombo->setCurrentIndex(index);
  enableChessboardListAddButton();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardListDeleteButton_clicked() {
  if(chessboardsCombo->count()>0) {
    chessboardsCombo->removeItem(chessboardsCombo->currentIndex());
    emit on_chessboardsCombo_currentIndexChanged(chessboardsCombo->currentIndex());
  }
  enableChessboardListAddButton();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardsCombo_currentIndexChanged(int index) {
  (void)index;
  QString text = chessboardsCombo->currentText();
  int    cols=0;
  int    rows=0;
  float  width=0.0;
  float  height=0.0;
  if(sscanf(text.toStdString().c_str(),
            "%dx%d-%fx%f",&cols,&rows,&width,&height)==4) {
    chessboardColsSpin->setValue(cols);
    chessboardRowsSpin->setValue(rows);
    chessboardCellWidthValue->setText(QString("%1").arg(width));
    chessboardCellHeightValue->setText(QString("%1").arg(height));
  }
}

void PanelCalibrate::on_chessboardDrawPointLine0_editingFinished() {
  updateCurrentImage();
}

void PanelCalibrate::on_chessboardDrawPointLine1_editingFinished() {
  updateCurrentImage();
}

void PanelCalibrate::on_chessboardDrawPointLine2_editingFinished() {
  updateCurrentImage();
}

void PanelCalibrate::on_chessboardDrawPointCheck_clicked() {
  updateCurrentImage();
}

//////////////////////////////////////////////////////////////////////
int PanelCalibrate::imageListCheckedItemsCount() {
  int nChecked = 0;
  QAbstractItemModel *model = imageList->model();
  if(model!=nullptr) {
    int nImages = model->rowCount();
    for(int row = 0;row<nImages;row++) {
      QModelIndex indx = model->index(row,0);
      QStandardItem* item = GuiUtil::getItem(imageList, indx);
      if(item->checkState()==Qt::Checked) nChecked++;
    }
  }
  return nChecked;
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::imageListUncheckAllItems() {
  QAbstractItemModel *model = imageList->model();
  if(model!=nullptr) {
    int nImages = model->rowCount();
    // uncheck each item
    for(int row = 0;row<nImages;row++) {
      QModelIndex indx = model->index(row,0);
      QStandardItem* item = GuiUtil::getItem(imageList, indx);
      item->setCheckState(Qt::Unchecked);
    }
    if(nImages>0) {
      // select first item
      imageList->setCurrentIndex(model->index(0,0));
      on_imageRotCombo_currentIndexChanged(0);
    }
  }
}

//////////////////////////////////////////////////////////////////////
int PanelCalibrate::imageListGetFrames() {
  int nFrames = 0;

  QVector<int>& frameStart = _calibData.laserFrameStart;
  QVector<int>& img2Frame = _calibData.laserImageToFrame;
  
  frameStart.clear();

  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();

  int iFrame = 0;
  int frameNumberPrev = -1;
  int frameNumber = 0;
  int frameImage  = 0;
  for(int row = 0;row<nImages;row++) {
    QModelIndex indx = model->index(row,0);
    QFileInfo   info(indx.data().toString());
    QString     imageName = info.fileName();
    if(sscanf(imageName.toStdString().c_str(),
              "F_%d_%d",&frameNumber,&frameImage)==2) {
      if(frameImage==0) frameStart.push_back(row);
      if(row>0 && frameNumber!=frameNumberPrev) iFrame++;
      img2Frame.push_back(iFrame);
      frameNumberPrev = frameNumber;
    }
  }
  nFrames = frameStart.size();
  frameStart.push_back(nImages);
  
  return nFrames;
}

//////////////////////////////////////////////////////////////////////
bool PanelCalibrate::chessboardDetectCurrent(bool interactive) {
  if(interactive)
    std::cerr << "PanelCalibrate::chessboardDetectCurrent() {\n";

  // get the number of images in the image list
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(interactive)
    std::cerr << "  nImages = " << nImages << "\n";
  if(nImages<=0) {
    if(interactive) {
      QMessageBox::critical(this, "Error", "Empty list of images.");
      std::cerr << "  empty list of images\n";
      std::cerr << "}\n";
    }
    return false; // ERROR
  }

  // get the index of the currently selected image
  QModelIndex modelIndex;
  auto item = GuiUtil::getCurrentItem(imageList, &modelIndex);
  if (!item) { //no item selected
    if(interactive) {
      QMessageBox::critical(this, "Error", "No item is selected.");
      std::cerr << "  no item selected\n";
      std::cerr << "}\n";
    }
    return false; // ERROR
  }
  int iImage  = (modelIndex.isValid())?modelIndex.row():0;
  if(interactive)
    std::cerr << "  iImage  = " << iImage << "\n";

  QFileInfo info(modelIndex.data().toString());
  auto imageName = info.fileName();
  if(interactive)
    std::cerr << "  imageName = " << qPrintable(imageName) << "\n";

  int  frameNumber = 0;
  int  frameImage  = 0;
  int  n = sscanf(imageName.toStdString().c_str(),"F_%d_%d",
                  &frameNumber,&frameImage);
  if(interactive && n==2) {
    std::cerr << "  frameNumber = " << frameNumber << "\n";
    std::cerr << "  frameImage  = " << frameImage << "\n";
  }

  // QImage _currentImage;
  int width  = _currentImage.width();
  int height = _currentImage.height();
  cv::Size  imageSize(width,height);
  
  // convert _currentImage to intensityImage
  cv::Mat1b intensityImage(imageSize);
  for(int y=0;y<height;y++) {
    for(int x=0;x<width;x++) {
      intensityImage.at<uchar>(y,x) = qGray(_currentImage.pixel(x,y));
    }
  }

  int        chessboardRows = chessboardRowsSpin->value();
  int        chessboardCols = chessboardColsSpin->value();
  cv::Size   cornerCount(chessboardCols,chessboardRows);

  double     cellWidth  = chessboardCellWidthValue->text().toDouble();
  double     cellHeight = chessboardCellHeightValue->text().toDouble();
  cv::Size2f cornerSize(cellWidth,cellHeight);

  std::vector<cv::Point3f> cornersWorld;
  std::vector<cv::Point2f> cornersCamera;

  int nCornersExpected = chessboardRows*chessboardCols;
  int nCornersDetected =
    hw4::detectChessboardCorners
    (intensityImage,cornerCount,cornerSize,cornersWorld, cornersCamera);

  if(nCornersDetected!=nCornersExpected) {
    if(false/*interactive*/) {
      QMessageBox::critical
        (this, "Error",
         QString("Detected only %1 Corners. Expected %2.")
         .arg(nCornersDetected)
         .arg(nCornersExpected));
      // return; ???
      std::cerr << "  detected only " << nCornersDetected
                << " corners of " << nCornersExpected <<" expected\n";
      std::cerr << "}\n";
    }
    return false; // ERROR
  }

  // save image with detected corners ???
  // QDir workDir(getApp()->getMainWindow()->getWorkDirectory());
  // _currentImage.save("detected.png");

  // update _calib 
  _calibData.chessboardCols       = chessboardCols;
  _calibData.chessboardRows       = chessboardRows;
  _calibData.chessboardCellWidth  = cellWidth;
  _calibData.chessboardCellHeight = cellHeight;
  _calibData.imageData.resize(static_cast<int>(nImages));
  _calibData.imageCount = nImages;
  _calibData.imageSize.setWidth(width);
  _calibData.imageSize.setHeight(height);

  CalibrationData::ImageData & data = _calibData.imageData[iImage];
  data.imageName = imageName;

  // copy cornersWorld onto data.worldPoints
  data.worldPoints.clear();
  for(size_t i=0;i<cornersWorld.size();i++) {
    cv::Point3f & q = cornersWorld[i];
    Vector3d p(q.x,q.y,q.z); 
    data.worldPoints.push_back(p);
  }

  // copy cornersCamera onto data.imagePoints
  data.imagePoints.clear();
  for(size_t i=0;i<cornersCamera.size();i++) {
    cv::Point2f & q = cornersCamera[i];
    Vector2d p(q.x,q.y); 
    data.imagePoints.push_back(p);
  }

  // draw chessboard corners { /////////////////////////////////////////
  if (drawCornersCheck->isChecked())
    drawChessboardCorners();

  // update image
  auto mainWin = getApp()->getMainWindow();
  auto glWidget = mainWin->getImgGLWidget();
  glWidget->setQImage(_currentImage);
  QTimer::singleShot(10, glWidget, SLOT(update()));

  if(interactive)
    std::cerr << "}\n";

  item->setCheckState(Qt::Checked);
  int nDetected = imageListCheckedItemsCount();
  chesboardDetectedLine->setText(QString("%1").arg(nDetected));
  
  return true; // SUCCESS
} // } PanelCalibrate::chessboardDetectCurrent()

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_chessboardDetectAndCalibrateButton_clicked(bool checked) {
  std::cerr
    << "PanelCalibrate::chessboardDetectAndCalibrateButton_clicked() {"
    << std::endl;
  
  int nDetected = 0;
  
  static bool stop = false;
  if(!checked) {
    stop = true;
    statusBarLine->setText("detecting all chessboards ... stopped");
    QApplication::processEvents();
  } else {
    statusBarLine->setText("detecting all chessboards ...");

    drawCornersCheck->setChecked(true);
    drawChessboardPosesCheck->setChecked(true);

    QApplication::processEvents();

    const bool interactive = false;

    QAbstractItemModel *model = imageList->model();
    int nImages = model->rowCount();

    calibrationProgress->setMaximum(nImages);
    calibrationProgress->setValue(0);
    imageListUncheckAllItems();

    // process images
    chesboardDetectedLine->setText(QString("%1").arg(nDetected));    
    for(int row = 0;stop==false && row<nImages;row++) {
      QModelIndex indx = model->index(row,0);
      QFileInfo   info(indx.data().toString());
      QString imageName = info.fileName();
      int     frameNumber = 0;
      int     frameImage  = 0;
      sscanf(imageName.toStdString().c_str(),"F_%d_%d",&frameNumber,&frameImage);
      if(frameImage==0) {
        imageList->setCurrentIndex(indx);
        /* bool success = */ chessboardDetectCurrent(interactive);
        if(_loopWaitMsec>0) GuiUtil::wait(_loopWaitMsec);
        calibrationProgress->setValue(row+1);
        nDetected++;
        chesboardDetectedLine->setText(QString("%1").arg(nDetected));    
      }
      QApplication::processEvents();
    }
    stop = false;
    statusBarLine->setText("detecting all chessboards ... done");
    QApplication::processEvents();
  }
  chessboardDetectAndCalibrateButton->setChecked(false);
  QApplication::processEvents();
  calibrationProgress->setValue(0);

  updateState();

  std::cerr << "  nDetected = "<<nDetected<<"\n";
  std::cerr << "}\n";

  cameraCalibrate();
  
} // } PanelCalibrate::on_chessboardDetectAndCalibrateButton_clicked()

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_drawCornersCheck_stateChanged() {
  updateCurrentImage();
  updateState();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_drawChessboardPosesCheck_stateChanged() {
  updateCurrentImage();
  updateState();
}  

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_clearCalibrationButton_clicked() {
  std::cerr << "PanelCalibrate::on_clearCalibrationButton_clicked() {\n";
  _calibData.clear();
  chesboardDetectedLine->setText("0");
  imageListUncheckAllItems();  
  updateState();
  updateCurrentImage();
  std::cerr << "}\n";
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::setGroupsEnabled(const bool value) {

  imagesGroupCheck->setEnabled(value);
  imagesGroup->setEnabled(value);
  chessboardGroupCheck->setEnabled(value);
  chessboardGroup->setEnabled(value);
  cameraGroupCheck->setEnabled(value);
  cameraGroup->setEnabled(value);
  lasersGroupCheck->setEnabled(value);
  lasersGroup->setEnabled(value);
  turntableGroupCheck->setEnabled(value);
  turntableGroup->setEnabled(value);

}

//////////////////////////////////////////////////////////////////////
// protected
void PanelCalibrate::cameraCalibrate() {
  std::cerr << "PanelCalibrate::cameraCalibrate() {\n";

  statusBarLine->setText("calibrating camera ...");
  QApplication::processEvents();

  int nImages = _calibData.imageCount;
  if(nImages<3) {
    QMessageBox::critical(this, "Error", "Minimum Number of Calibration Images = 3");
    updateState();
    return;
  }

  int width  = _calibData.imageSize.width();
  int height = _calibData.imageSize.height();
  if(width<=0 || height<=0) {
    QMessageBox::critical(this, "Error", "imageWidth & imageHeight should be > 0");
    updateState();
    return;
  }
  cv::Size2i imageSize(width,height);

  int chessboardRows = chessboardRowsSpin->value();
  int chessboardCols = chessboardColsSpin->value();
  int nCorners       = chessboardRows*chessboardCols;
  _calibData.chessboardCols = chessboardRows;
  _calibData.chessboardRows = chessboardCols;
  
  double cellWidth  = chessboardCellWidthValue->text().toDouble();
  double cellHeight = chessboardCellHeightValue->text().toDouble();
  _calibData.chessboardCellWidth  = cellWidth;
  _calibData.chessboardCellHeight = cellHeight;
  
  std::cerr << "  nCorners = " << nCorners << "\n";

  std::vector<int> iImageUsed;
  std::vector<std::vector<cv::Point3f> > worldCornersActive;
  std::vector<std::vector<cv::Point2f> > imageCornersActive;
  QVector<CalibrationData::ImageData> & dataVec = _calibData.imageData;
  for(int iImage=0; iImage<nImages;iImage++) {
    CalibrationData::ImageData & data = dataVec[iImage];
    if(data.imagePoints.size()!=nCorners || data.worldPoints.size()!=nCorners) {
      std::cerr << "  skipping image " << iImage << "\n";
      std::cerr << "    nImageCorners = " << data.imagePoints.size() << "\n";
      std::cerr << "    nWorldCorners = " << data.worldPoints.size() << "\n";
    } else {
      iImageUsed.push_back(iImage);
      QVector<Vector3d> & worldPoints = data.worldPoints;
      QVector<Vector2d> & imagePoints = data.imagePoints;

      std::vector<cv::Point3f> worldCorners;
      std::vector<cv::Point2f> imageCorners;
      for(int k=0;k<nCorners;k++) {
        Vector3d & wp = worldPoints[k];
        Vector2d & ip = imagePoints[k];
        cv::Point3f wc(wp(0),wp(1),wp(2));
        cv::Point2f ic(ip(0),ip(1));
        worldCorners.push_back(wc);
        imageCorners.push_back(ic);
      }

      worldCornersActive.push_back(worldCorners);
      imageCornersActive.push_back(imageCorners);
    }
  }

  int nImagesUsed = iImageUsed.size();
  std::cerr << "  nImagesUsed = " << nImagesUsed << "\n";

  std::vector<cv::Mat> calRvecs;
  std::vector<cv::Mat> calTvecs;
  cv::Mat K;
  cv::Mat kc;

  double camError = hw4::calibrateCamera
    (worldCornersActive,imageCornersActive,imageSize,K,kc,calRvecs,calTvecs);

  // save the intrinsic parameters
  _calibData.camera_K(0,0) = K.at<double>(0,0);
  _calibData.camera_K(0,1) = K.at<double>(0,1);
  _calibData.camera_K(0,2) = K.at<double>(0,2);
  _calibData.camera_K(1,0) = K.at<double>(1,0);
  _calibData.camera_K(1,1) = K.at<double>(1,1);
  _calibData.camera_K(1,2) = K.at<double>(1,2);
  _calibData.camera_K(2,0) = K.at<double>(2,0);
  _calibData.camera_K(2,1) = K.at<double>(2,1);
  _calibData.camera_K(2,2) = K.at<double>(2,2);
  // save the lens distortion parameters
  _calibData.camera_kc(0) = kc.at<double>(0,0);
  _calibData.camera_kc(1) = kc.at<double>(0,1);
  _calibData.camera_kc(2) = kc.at<double>(0,2);
  _calibData.camera_kc(3) = kc.at<double>(0,3);
  _calibData.camera_kc(4) = kc.at<double>(0,4);
  // save the camera poses
  for(int j=0;j<nImagesUsed;j++) {
    int iImage = iImageUsed[j];
    CalibrationData::ImageData & data = dataVec[iImage];
    //
    cv::Mat  & Rvec  = calRvecs[j];
    Vector3d & cRvec = data.rotVec;
    cRvec(0) = Rvec.at<double>(0,0);
    cRvec(1) = Rvec.at<double>(1,0);
    cRvec(2) = Rvec.at<double>(2,0);
    //
    cv::Mat  & Tvec  = calTvecs[j];
    Vector3d & cTvec = data.T;
    cTvec(0) = Tvec.at<double>(0,0);
    cTvec(1) = Tvec.at<double>(1,0);
    cTvec(2) = Tvec.at<double>(2,0);
  }

  std::cerr << "  camError = " << camError << "\n";

  _calibData.cameraError        = camError;
  _calibData.cameraIsCalibrated = true;

  _calibData.print();

  statusBarLine->setText("calibrating camera ... done");
  QApplication::processEvents();

  updateState();
  updateCurrentImage();
  
  std::cerr << "}\n";
}

//////////////////////////////////////////////////////////////////////
// Turntable Group

void PanelCalibrate::on_turntableCalibrateButton_clicked() {
  std::cerr << "PanelCalibrate::on_turntableCalibrateButton_clicked() {\n";

  // char line[256]; // used for console printing

  statusBarLine->setText("calibrating turntable ...");

  drawCornersCheck->setChecked(false);
  drawChessboardPosesCheck->setChecked(false);
  drawTurntableFrameCheck->setChecked(true);

  QApplication::processEvents();

  _calibData.turntableIsCalibrated = false;
  _calibData.clearTurntable();

  try {

    if(_calibData.cameraIsCalibrated==false)
      throw new StrException("camera is not calibrated");

    QAbstractItemModel *model = imageList->model();
    int nImages = model->rowCount();
    std::cerr << "  nImages = " << nImages << std::endl;  
    int indxMin = indxMinSpin->value();
    int indxMax = indxMaxSpin->value();
    std::cerr << "  indxMin = " << indxMin << std::endl;  
    std::cerr << "  indxMax = " << indxMax << std::endl;
    if(indxMin>=indxMax)
      throw new StrException("invalid image range");

    QVector<Matrix3d> R_pose;
    QVector<Vector3d> T_pose;
    for(int i=indxMin;i<=indxMax;i++) {
      CalibrationData::ImageData & imgData = _calibData.imageData[i];
      R_pose.push_back(MatrixUtil::rodrigues(imgData.rotVec));
      T_pose.push_back(imgData.T);
    }

    // turntable center in chessboard coordinates
    Vector3d& patternCenter = _calibData.patternCenter;
    patternCenter << chessboardDrawPointLine0->text().toDouble(),0,0 ;

    // turntable center in camera coordinates
    Vector3d& turntableCenter = _calibData.turntableCenter;

    // temporary
    QVector<Matrix3d> R_tt;

    hw4::calibrateTurntable
      (R_pose, T_pose, patternCenter, turntableCenter, R_tt);

    // at this point it should be true that
    // turntableCenter == R_pose[i] * patternCenter + T_pose[i];
    // for iMin<=i<=iMax :

    Matrix3d& turntableFrame = _calibData.turntableFrame;
    turntableFrame = R_tt[0];

    QVector<Matrix3d>& turntableRotation = _calibData.turntableRotation;
    turntableRotation.clear();
    int i,j;
    // out of range indx values
    for(i=0;i<indxMin;i++) {
      turntableRotation.push_back(Matrix3d::Zero());
    }
    // in range indx values
    for(j=0;i<=indxMax;i++,j++) {
      turntableRotation.push_back(R_tt[j]);
    }
    // out of range indx values
    int imgCount = _calibData.imageCount;
    for(;i<=imgCount;i++) {
      turntableRotation.push_back(Matrix3d::Zero());
    }

    // remember the range
    _calibData.imgMinCenter = indxMin;
    _calibData.imgMaxCenter = indxMax;

    _calibData.turntableIsCalibrated = true;

  } catch(StrException* e) {
    statusBarLine->setText(e->what());
    QApplication::processEvents();
    delete e;
  }

  _calibData.print();

  statusBarLine->setText("calibrating camera ... done");
  QApplication::processEvents();

  updateState();
  updateCurrentImage();

  std::cerr << "}\n";
}

//////////////////////////////////////////////////////////////////////
// Lasers Group

void PanelCalibrate::on_lasersGroupCheck_stateChanged(int state) {
  lasersGroup->setVisible((state!=0));
  std::cerr << "panelCalibration->widh() = " << width() << std::endl;
}

void PanelCalibrate::on_drawLaserBoundingQuadCheck_stateChanged(int state) {
  (void) state;
  updateCurrentImage();
  updateState();
}

void PanelCalibrate::on_drawLaserDetectedPointsCheck_stateChanged(int state) {
  (void) state;
  updateCurrentImage();
  updateState();
}

void PanelCalibrate::on_drawLaserTriangulatedPointsCheck_stateChanged(int state) {
  (void) state;
  updateCurrentImage();
  updateState();
}

void PanelCalibrate::on_drawLaserPlaneCheck_stateChanged(int state) {
  (void) state;
  updateCurrentImage();
  updateState();
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_laserEstimatePoseDetectButton_clicked(bool checked) {
  std::cerr
    << "PanelCalibrate::on_laserEstimatePoseDetectButton_clicked() {"
    << std::endl;
  
  static bool stop = false;
  if(!checked) {
    stop = true;
    statusBarLine->setText("estimating poses and detecting ... stopped");
    QApplication::processEvents();
  } else if(_calibData.cameraIsCalibrated==false) {
    statusBarLine->setText("camera is not calibrated ...");
    QApplication::processEvents();
  } else {
    statusBarLine->setText("estimating poses and detecting ...");

    drawCornersCheck->setChecked(false);
    drawChessboardPosesCheck->setChecked(false);
    drawTurntableFrameCheck->setChecked(false);
    drawLaserDetectedPointsCheck->setChecked(true);
    drawLaserBoundingQuadCheck->setChecked(true);
    drawLaserTriangulatedPointsCheck->setChecked(false);
    drawLaserPlaneCheck->setChecked(false);

    QApplication::processEvents();

    _calibData.clearLasers();
    
    // camera calibration parameters
    Matrix3d const& K  = _calibData.camera_K;
    Vector5d const& kc = _calibData.camera_kc; // lens distortion parameters
    // convert to cv arrays
    cv::Mat cv_K = cv::Mat::zeros(3, 3, CV_64F);
    cv_K.at<double>(0,0) = K(0,0);
    cv_K.at<double>(0,1) = K(0,1);
    cv_K.at<double>(0,2) = K(0,2);
    cv_K.at<double>(1,0) = K(1,0);
    cv_K.at<double>(1,1) = K(1,1);
    cv_K.at<double>(1,2) = K(1,2);
    cv_K.at<double>(2,0) = K(2,0);
    cv_K.at<double>(2,1) = K(2,1);
    cv_K.at<double>(2,2) = K(2,2);
    cv::Mat cv_kc = cv::Mat::zeros(1,5,CV_64F);
    cv_kc.at<double>(0,0) = kc(0);
    cv_kc.at<double>(0,1) = kc(1);
    cv_kc.at<double>(0,2) = kc(2);
    cv_kc.at<double>(0,3) = kc(3);
    cv_kc.at<double>(0,4) = kc(4);

    int        chessboardRows = chessboardRowsSpin->value();
    int        chessboardCols = chessboardColsSpin->value();
    cv::Size   cornerCount(chessboardCols,chessboardRows);
    
    double     cellWidth  = chessboardCellWidthValue->text().toDouble();
    double     cellHeight = chessboardCellHeightValue->text().toDouble();
    cv::Size2f cornerSize(cellWidth,cellHeight);

    double xMin = 0.0;
    double xMax = static_cast<double>(chessboardCols-1)*cellWidth;
    double yMin = 0.0;
    double yMax = static_cast<double>(chessboardRows-1)*cellHeight;

    // coords of bounding quad in checkerboard plane
    cv::Point3f wPt00(xMin,yMin,0.0);
    cv::Point3f wPt01(xMin,yMax,0.0);
    cv::Point3f wPt10(xMax,yMin,0.0);
    cv::Point3f wPt11(xMax,yMax,0.0);

    // disable every other button
    lasersGroupCheck->setEnabled(false);
    // laserEstimatePoseDetectButton->setEnabled(false);
    laserTriangulateButton->setEnabled(false);
    laserFitPlanesButton->setEnabled(false);

    QApplication::processEvents();
    calibrationProgress->setValue(0);
    imageListUncheckAllItems();

    QAbstractItemModel *model = imageList->model();
    // int nImages = model->rowCount();

    int nFrames = imageListGetFrames();
    QVector<CalibrationData::LaserFrameData>&
        laserFrameData = _calibData.laserFrameData;
    laserFrameData.clear();
    for(int iF = 0;stop==false && iF<nFrames;iF++) {
      laserFrameData.push_back(CalibrationData::LaserFrameData());
    }    

    calibrationProgress->setMaximum(nFrames);
    QVector<int>& frameStart = _calibData.laserFrameStart;
    // QVector<int>& img2frame = _calibData.laserImageToFrame;

    int nDetected1 = 0;
    int nDetected2 = 0;

    QModelIndex indx;
    QStandardItem* item;
    QString rot = imageRotCombo->currentText();
    for(int iF = 0;stop==false && iF<nFrames;iF++) {
      CalibrationData::LaserFrameData& laserFrame_data_iF = laserFrameData[iF];

      int frameImageCount = frameStart[iF+1]-frameStart[iF];
      if(frameImageCount==3) {

        int row = frameStart[iF];

        // first image with both lasers off
        indx = model->index(row,0);
        imageList->setCurrentIndex(indx);
        QImage qImg0 = GuiUtil::loadCurrentImage(imageList,rot,false);

        item = GuiUtil::getCurrentItem(imageList, &indx);
        if(item!=nullptr) item->setCheckState(Qt::Checked);

        int width  = qImg0.width();
        int height = qImg0.height();
        cv::Size  imageSize(width,height);
  
        // convert qImg0 to cv::Mat format
        cv::Mat1b cvImg0(imageSize);
        for(int row=0;row<height;row++) {
          for(int col=0;col<width;col++) {
            cvImg0.at<uchar>(row,col) = qGray(qImg0.pixel(col,row));
          }
        }

        std::vector<cv::Point3f> cornersWorld;
        std::vector<cv::Point2f> cornersCamera;

        // detect chessboard corners
        int nCorners = hw4::detectChessboardCorners
          (/* inputs */ cvImg0, cornerCount, cornerSize,
           /* outputs */ cornersWorld, cornersCamera);

        // TODO Tue Nov  5 19:05:04 2024
        // if(nCorners==0) => ERROR

        // save chessboard corners
        laserFrame_data_iF.cornersChessboard.clear();
        laserFrame_data_iF.cornersCamera.clear();
        for(int iC=0; iC<nCorners;iC++) {
          cv::Point3f & q3 = cornersWorld[iC];
          Vector3d p3(q3.x,q3.y,q3.z);
          laserFrame_data_iF.cornersChessboard.push_back(p3);
          cv::Point2f & q2 = cornersCamera[iC];
          Vector2d p2(q2.x,q2.y);
          laserFrame_data_iF.cornersCamera.push_back(p2);
        }

        cv::Mat R_vec = cv::Mat::zeros(3, 1, CV_64F);
        cv::Mat T_vec = cv::Mat::zeros(3, 1, CV_64F);
        hw4::estimatePose
          (/* inputs */ cv_K, cv_kc, cornersWorld, cornersCamera, imageSize,
           /* outputs */ R_vec, T_vec);
      
        Vector3d rotVec;
        rotVec(0) = R_vec.at<double>(0,0);
        rotVec(1) = R_vec.at<double>(1,0);
        rotVec(2) = R_vec.at<double>(2,0);
        laserFrame_data_iF.R = MatrixUtil::rodrigues(rotVec);

        Vector3d& T = laserFrame_data_iF.T;
        T(0) = T_vec.at<double>(0,0);
        T(1) = T_vec.at<double>(1,0);
        T(2) = T_vec.at<double>(2,0);

        // look for bounding quad corners in worldPoints
        int i00 = -1, i01 = -1, i10 = -1, i11 = -1;
        for(int i=0;i<nCorners;i++) {
          cv::Point3f const& wPt_i = cornersWorld[i];
          // if(wPt_i.x==wPt00.x && wPt_i.y==wPt00.y) i00 = i;
          // else if(wPt_i.x==wPt01.x && wPt_i.y==wPt01.y) i01 = i;
          // else if(wPt_i.x==wPt10.x && wPt_i.y==wPt10.y) i10 = i;
          // else if(wPt_i.x==wPt11.x && wPt_i.y==wPt11.y) i11 = i;
          if(cv::norm(wPt_i-wPt00)<0.01) i00 = i;
          else if(cv::norm(wPt_i-wPt01)<0.01) i01 = i;
          else if(cv::norm(wPt_i-wPt10)<0.01) i10 = i;
          else if(cv::norm(wPt_i-wPt11)<0.01) i11 = i;
        }

        // determine bounding quad
        QPolygonF& quad = laserFrame_data_iF.quadCamera;
        quad.clear();
        if(i00>=0 && i01>=0 && i10>=0 && i10>=0) {
          QPointF p00(cornersCamera[i00].x,cornersCamera[i00].y);
          QPointF p01(cornersCamera[i01].x,cornersCamera[i01].y);
          QPointF p10(cornersCamera[i10].x,cornersCamera[i10].y);
          QPointF p11(cornersCamera[i11].x,cornersCamera[i11].y);
          quad << p00 << p01 << p11 << p10 << p00;
        }

        // first laser image
        indx = model->index(row+1,0);
        imageList->setCurrentIndex(indx);
        QImage qImg1 = GuiUtil::loadCurrentImage(imageList,rot,false);
        item = GuiUtil::getCurrentItem(imageList, &indx);
        if(item!=nullptr) item->setCheckState(Qt::Checked);
        hw4::detectLaserPoints(qImg1,qImg0,quad,laserFrame_data_iF.detectedPoints1);
        nDetected1 += laserFrame_data_iF.detectedPoints1.size();
        updateImage(qImg1);
        QApplication::processEvents();
      
        if(_loopWaitMsec>0) GuiUtil::wait(_loopWaitMsec);

        // get second laser image
        indx = model->index(row+2,0);
        imageList->setCurrentIndex(indx);
        QImage qImg2 = GuiUtil::loadCurrentImage(imageList,rot,false);
        item = GuiUtil::getCurrentItem(imageList, &indx);
        if(item!=nullptr) item->setCheckState(Qt::Checked);
        hw4::detectLaserPoints(qImg2,qImg0,quad,laserFrame_data_iF.detectedPoints2);
        nDetected2 += laserFrame_data_iF.detectedPoints2.size();
        updateImage(qImg2);
        QApplication::processEvents();

        if(_loopWaitMsec>0) GuiUtil::wait(_loopWaitMsec);
      }

      calibrationProgress->setValue(iF+1);
      QApplication::processEvents();
    }
    
    stop = false;
    statusBarLine->setText("detecting laser chessboards ... done");

    if(nDetected1==0 && nDetected2==0) {
      _calibData.laserPointsAreDetected = false;
      _calibData.laserPosesAreEstimated = false;
    } else {
      _calibData.laserPointsAreDetected = true;
      _calibData.laserPosesAreEstimated = true;
    }

    QApplication::processEvents();
  }
  
  laserEstimatePoseDetectButton->setChecked(false);
  QApplication::processEvents();
  calibrationProgress->setValue(0);

  updateState();
  
  std::cerr << "}" << std::endl;
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_laserTriangulateButton_clicked(bool checked) {
  std::cerr
    << "PanelCalibrate::on_laserTriangulateButton_clicked() {"
    << std::endl;
  
  static bool stop = false;
  if(!checked) {
    stop = true;
    statusBarLine->setText("triangulating laser points ... stopped");
    QApplication::processEvents();
  } else if(_calibData.cameraIsCalibrated==false) {
    statusBarLine->setText("camera is not calibrated ...");
    QApplication::processEvents();
  } else if(_calibData.laserPointsAreDetected==false) {
    statusBarLine->setText("laser points are not detected ...");
    QApplication::processEvents();
  } else if(_calibData.laserPosesAreEstimated==false) {
    statusBarLine->setText("laser poses are not estimated ...");
    QApplication::processEvents();
  } else {
    statusBarLine->setText("triangulating laser points ...");

    drawCornersCheck->setChecked(false);
    drawChessboardPosesCheck->setChecked(false);
    drawTurntableFrameCheck->setChecked(false);
    drawLaserDetectedPointsCheck->setChecked(false);
    drawLaserBoundingQuadCheck->setChecked(true);
    drawLaserTriangulatedPointsCheck->setChecked(true);
    drawLaserPlaneCheck->setChecked(false);

    QApplication::processEvents();

    // camera calibration parameters
    Matrix3d const& K  = _calibData.camera_K;
    Vector5d const& kc = _calibData.camera_kc; // lens distortion parameters

    // disable every other button
    lasersGroupCheck->setEnabled(false);
    laserEstimatePoseDetectButton->setEnabled(false);
    // laserTriangulateButton->setEnabled(false);
    laserFitPlanesButton->setEnabled(false);

    QApplication::processEvents();
    calibrationProgress->setValue(0);
    imageListUncheckAllItems();

    QVector<CalibrationData::LaserFrameData>&
        laserFrameData = _calibData.laserFrameData;
    int nFrames = laserFrameData.size();

    calibrationProgress->setMaximum(nFrames);
    QVector<int>& frameStart = _calibData.laserFrameStart;

    for(int iF = 0;stop==false && iF<nFrames;iF++) {
      CalibrationData::LaserFrameData& laserFrameData_iF = laserFrameData[iF];

      int frameImageCount = frameStart[iF+1]-frameStart[iF];
      if(frameImageCount<3) continue;

      const Matrix3d& R_iF = laserFrameData_iF.R;
      const Vector3d& T_iF = laserFrameData_iF.T;
      Vector4d checkerboardPlane;
      hw3::checkerboardPlane(R_iF, T_iF, checkerboardPlane);
      
      QVector<Vector2d>& detectedPoints1 = laserFrameData_iF.detectedPoints1;
      QVector<Vector2d>& detectedPoints2 = laserFrameData_iF.detectedPoints2;

      QVector<Vector3d>& triLaserPoints1 = laserFrameData_iF.triangulatedPoints1;
      QVector<Vector3d>& triLaserPoints2 = laserFrameData_iF.triangulatedPoints2;

      triLaserPoints1.clear();
      triLaserPoints2.clear();

      hw3::triangulate(K, kc, checkerboardPlane, detectedPoints1, triLaserPoints1);
      hw3::triangulate(K, kc, checkerboardPlane, detectedPoints2, triLaserPoints2);

      calibrationProgress->setValue(iF+1);
    }
    
    stop = false;
    statusBarLine->setText("triangulating laser points ... done");

    _calibData.laserPointsAreTriangulated = true;

    QApplication::processEvents();
  }
  
  laserTriangulateButton->setChecked(false);
  QApplication::processEvents();
  calibrationProgress->setValue(0);

  updateState();
  
  std::cerr << "}" << std::endl;
}

//////////////////////////////////////////////////////////////////////
void PanelCalibrate::on_laserFitPlanesButton_clicked() {
  _calibData.laserPlanesAreFitted = false;

  statusBarLine->setText("fitting laser planes ...");

  drawCornersCheck->setChecked(false);
  drawChessboardPosesCheck->setChecked(false);
  drawTurntableFrameCheck->setChecked(false);
  drawLaserDetectedPointsCheck->setChecked(false);
  drawLaserBoundingQuadCheck->setChecked(false);
  drawLaserTriangulatedPointsCheck->setChecked(false);
  drawLaserPlaneCheck->setChecked(true);

  QApplication::processEvents();

  Vector4d & laserPlane1 = _calibData.laserPlane1;
  Vector4d & laserPlane2 = _calibData.laserPlane2;
  laserPlane1.Zero();
  laserPlane2.Zero();

  int nFrames = _calibData.laserFrameData.size();
  if(nFrames>=2) {

    QVector<Vector3d> laserPoints1;
    QVector<Vector3d> laserPoints2;
    for(int iF=0;iF<nFrames;iF++) {
      CalibrationData::LaserFrameData & lfd = _calibData.laserFrameData[iF];
      laserPoints1.append(lfd.triangulatedPoints1);
      laserPoints2.append(lfd.triangulatedPoints2);
    }
    hw3::estimatePlane(laserPoints1,laserPlane1);
    hw3::estimatePlane(laserPoints2,laserPlane2);

    _calibData.laserPlanesAreFitted = true;

    // set MainWindow scanner calibration data

    auto mainWin = getApp()->getMainWindow();
    ScannerCalibData& scannerCalibData = mainWin->getScannerCalibData();
    scannerCalibData.camera_K        = _calibData.camera_K;
    scannerCalibData.camera_kc       = _calibData.camera_kc;
    scannerCalibData.turntableFrame  = _calibData.turntableFrame;
    scannerCalibData.turntableCenter = _calibData.turntableCenter;
    scannerCalibData.laserPlane1     = _calibData.laserPlane1;
    scannerCalibData.laserPlane2     = _calibData.laserPlane2;
    scannerCalibData.isCalibrated    = true;

  } else {
    _calibData.laserPlanesAreFitted = false;
  }    

  // display in GUI
  laserPlane10->setText(QString::number(laserPlane1[0]));
  laserPlane11->setText(QString::number(laserPlane1[1]));
  laserPlane12->setText(QString::number(laserPlane1[2]));
  laserPlane13->setText(QString::number(laserPlane1[3]));
  
  // display in GUI
  laserPlane20->setText(QString::number(laserPlane2[0]));
  laserPlane21->setText(QString::number(laserPlane2[1]));
  laserPlane22->setText(QString::number(laserPlane2[2]));
  laserPlane23->setText(QString::number(laserPlane2[3]));

  statusBarLine->setText("fitting laser planes ... done");
  
  updateState();
  updateCurrentImage();
  QApplication::processEvents();
}
