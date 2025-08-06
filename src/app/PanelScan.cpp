// Time-stamp: <2024-11-10 11:35:21 taubin>
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

#include "PanelScan.hpp"

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

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "Application.hpp"
#include "MainWindow.hpp"
#include "ScannerCalibData.hpp"

#include <img/util.hpp>
#include <img/yuv.hpp>
#include <img/ImgArgb.hpp>
#include <img/ImgBit.hpp>
#include <img/ImgBitplane.hpp>
#include <img/ImgDraw.hpp>

#include <util/GuiUtil.hpp>
#include <util/MatrixUtil.hpp>
#include <util/StrException.hpp>

#include <wrl/SceneGraph.hpp>

#include <homework/homework3.hpp>
#include <homework/homework4.hpp>

#ifndef PATH_LENGTH
#define PATH_LENGTH 4096
#endif

int PanelScan::_loopWaitMsec = 0; // 250;

//////////////////////////////////////////////////////////////////////
// QWidget {

bool PanelScan::_registered = PanelScan::registerPanel();
bool PanelScan::registerPanel() {
  qDebug("PanelScan: register");
  return MainWindow::registerPanel
    ("Scan",
     [](QWidget *parent) -> QWidget*{ return new PanelScan(parent); });
  return true;
}

//////////////////////////////////////////////////////////////////////
PanelScan::PanelScan(QWidget * parent) :
  QWidget(parent),
  _model(),
  _laserFrameStart(),
  _laserImageToFrame()
{

  qDebug("PanelScan::PanelScan() {");

  setupUi(this);

  QSettings config;

  scanningProgress->setValue(0);

  // image group
  QStringList rotationList; rotationList << "-90" << "0" << "90";
  GuiUtil::fillCombo(imageRotCombo, rotationList,
                      config.value("PanelScan/rotation").toString());
  imageRotCombo->setCurrentText("0");

  // load other settings
  loadSettings();

  // list view model
  imageList->setModel(&_model);
  auto selectionModel = imageList->selectionModel();
  connect(selectionModel, &QItemSelectionModel::currentChanged,
          this,           &PanelScan::_on_currentImageChanged);

  frameNumberLine->setText("0");

  // triangulation group
  QStringList triChoiceList; triChoiceList << "Laser 1" << "Laser 2" << "Laser 1+2";
  GuiUtil::fillCombo(triangulateChoiceCombo, triChoiceList,
                      config.value("PanelScan/triChoice").toString());
  triangulateChoiceCombo->setCurrentText("Laser 1+2");
  
  qDebug("}");
}

PanelScan::~PanelScan() {
  saveSettings();
  qDebug(" -- PanelScan destroyed -- ");
}

void PanelScan::on_bboxGroupCheck_stateChanged(int state) {
  bboxGroup->setVisible((state!=0));
}

void PanelScan::on_bboxP0XLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[0].setX(bboxP0XLine->text().toDouble());
    _quad[4].setX(bboxP0XLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP0YLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[0].setY(bboxP0YLine->text().toDouble());
    _quad[4].setY(bboxP0YLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP1XLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[1].setX(bboxP1XLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP1YLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[1].setY(bboxP1YLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP2XLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[2].setX(bboxP2XLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP2YLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[2].setY(bboxP2YLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP3XLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[3].setX(bboxP3XLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxP3YLine_editingFinished() {
  if(_quad.size()==5) {
    _quad[3].setY(bboxP3YLine->text().toDouble());
    updateCurrentImage();
  }
}

void PanelScan::on_bboxFileLoadButton_clicked() {
  try {
    
    statusBarLine->setText("loading bbox.txt file ...");
    
    auto mainWin = getApp()->getMainWindow();
    QDir dir(mainWin->getWorkDirectory());
    if(dir.exists("bbox.txt")==false)
      throw new StrException("bbox.txt file does ot exist");      
    
    char bbox_txt_path[PATH_LENGTH];
    memset(bbox_txt_path,0,PATH_LENGTH);
    snprintf(bbox_txt_path,PATH_LENGTH,"%s/bbox.txt",
             dir.absolutePath().toStdString().c_str());
    
    FILE* fp = fopen(bbox_txt_path,"r");
    if(fp==nullptr)
      throw new StrException("unable to open bbox.txt file for reading");      
      
    double x,y;
    if(fscanf(fp,"p0 %lf %lf\n",&x,&y)!=2)
      throw new StrException("unable to read bbox.p0");
    QPointF p0(x,y);
    if(fscanf(fp,"p1 %lf %lf\n",&x,&y)!=2)
      throw new StrException("unable to read bbox.p1");
    QPointF p1(x,y);
    if(fscanf(fp,"p2 %lf %lf\n",&x,&y)!=2)
      throw new StrException("unable to read bbox.p2");
    QPointF p2(x,y);
    if(fscanf(fp,"p3 %lf %lf\n",&x,&y)!=2)
      throw new StrException("unable to read bbox.p3");
    QPointF p3(x,y);
    
    if(p0!=p1 && p1!=p2 && p2!=p3 && p3!=p0) {
      _quad.clear();
      _quad << p0 << p1 << p2 << p3 << p0;
    }
    
    updateState();
    updateCurrentImage();
    
    statusBarLine->setText("loading bbox.txt file ... done");
    
    fclose(fp);

  } catch(StrException* e) {
    statusBarLine->setText(e->what());
    QApplication::processEvents();
    delete e;
  }
}

void PanelScan::on_bboxFileSaveButton_clicked() {
  try {
    
    statusBarLine->setText("saving bbox.txt file ...");
  
    auto mainWin = getApp()->getMainWindow();
    QDir dir(mainWin->getWorkDirectory());
    char bbox_txt_path[PATH_LENGTH];
    memset(bbox_txt_path,0,PATH_LENGTH);
    snprintf(bbox_txt_path,PATH_LENGTH,"%s/bbox.txt",
             dir.absolutePath().toStdString().c_str());
  
    FILE* fp = fopen(bbox_txt_path,"w");
    if(fp==nullptr)
      throw new StrException("unable to open bbox.txt file for writing");
    
    QPointF& p0 = _quad[0];
    QPointF& p1 = _quad[1];
    QPointF& p2 = _quad[2];
    QPointF& p3 = _quad[3];
    fprintf(fp,"p0 %12.4lf %12.4lf\n",p0.x(),p0.y());
    fprintf(fp,"p1 %12.4lf %12.4lf\n",p1.x(),p1.y());
    fprintf(fp,"p2 %12.4lf %12.4lf\n",p2.x(),p2.y());
    fprintf(fp,"p3 %12.4lf %12.4lf\n",p3.x(),p3.y());
    fclose(fp);

    
    statusBarLine->setText("saving bbox.txt file ... done");

  } catch(StrException* e) {
    statusBarLine->setText(e->what());
    QApplication::processEvents();
    delete e;
  }
}

void PanelScan::on_imagesGroupCheck_stateChanged(int state) {
  imagesGroup->setVisible((state!=0));
}

void PanelScan::on_triangulationGroupCheck_stateChanged(int state) {
  triangulationGroup->setVisible((state!=0));
}

void PanelScan::on_view3DGroupCheck_stateChanged(int state) {
  view3DGroup->setVisible((state!=0));
}

//////////////////////////////////////////////////////////////////////
void PanelScan::updateState() {

  auto mainWin  = getApp()->getMainWindow();
  // auto glWidget = mainWin->getImgGLWidget();
  // glWidget->clearImage();
  ScannerCalibData& scannerCalibData = mainWin->getScannerCalibData();
  if(scannerCalibData.isCalibrated) {
    statusBarLine->setText("SCANNER IS CALIBRATED");
    triangulationRunButton->setEnabled(true);
  } else {
    statusBarLine->setText("SCANNER IS NOT CALIBRATED");
    triangulationRunButton->setEnabled(false);
  }

  if(_quad.size()==5) {
    QPointF& p0 = _quad[0];
    bboxP0XLine->setText(QString::number(p0.x()));
    bboxP0YLine->setText(QString::number(p0.y()));
    QPointF& p1 = _quad[1];
    bboxP1XLine->setText(QString::number(p1.x()));
    bboxP1YLine->setText(QString::number(p1.y()));
    QPointF& p2 = _quad[2];
    bboxP2XLine->setText(QString::number(p2.x()));
    bboxP2YLine->setText(QString::number(p2.y()));
    QPointF& p3 = _quad[3];
    bboxP3XLine->setText(QString::number(p3.x()));
    bboxP3YLine->setText(QString::number(p3.y()));
  }
}

//////////////////////////////////////////////////////////////////////
void PanelScan::showEvent(QShowEvent * event) {
    (void) event;

  // this block is here because in the constructor
  // getApp()->getMainWindow()==NULL
  static bool firstShow = true;
  std::cerr << "  firstShow = "<< firstShow << "\n";
  auto mainWin = getApp()->getMainWindow();
  if(firstShow) {
    // connect(wrlFileLoadButton,  SIGNAL(clicked()),
    //         mainWin, SLOT(on_wrlFileLoadButton_clicked()));
    connect(wrlFileSaveButton,  SIGNAL(clicked()),
            mainWin, SLOT(on_wrlFileSaveButton_clicked()));
    // connect(turntableFileLoadButton, SIGNAL(clicked()),
    //         mainWin, SLOT(on_turntableFileLoadButton_clicked()));
    firstShow = false;
  }

  mainWin->showImgGLWidget(true);
  mainWin->showWrlGLWidget(true);

  connect(mainWin, &MainWindow::workDirectoryChanged,
          this,    &PanelScan::_on_workDirectoryChanged);

  updateImageFiles();

  // show the first image in imageList
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(nImages>0) {
    imageList->setCurrentIndex(model->index(0,0));
    on_imageRotCombo_currentIndexChanged(0);
  }
  
  updateState();
}

void PanelScan::hideEvent(QHideEvent *e) {
  if (e && !e->spontaneous()) {
    auto mainWin = getApp()->getMainWindow();
    disconnect(mainWin, &MainWindow::workDirectoryChanged,
               this,    &PanelScan::_on_workDirectoryChanged);
  }
}

// } QWidget
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Settings {

void PanelScan::loadSettings(void) {

  QSettings config;

  // Images group
  imageRotCombo->setCurrentText
    (config.value("PanelScan/rotation","0").toString());

  bool bboxGroupVisible =
    config.value("PanelScan/bboxGroup/visible",false).toBool();
  bboxGroupCheck->setChecked(bboxGroupVisible);
  bboxGroup->setVisible(bboxGroupVisible);

  bool imagesGroupVisible =
    config.value("PanelScan/imagesGroup/visible",false).toBool();
  imagesGroupCheck->setChecked(imagesGroupVisible);
  imagesGroup->setVisible(imagesGroupVisible);

  bool triangulationGroupVisible =
    config.value("PanelScan/triangulationGroup/visible",false).toBool();
  triangulationGroupCheck->setChecked(triangulationGroupVisible);
  triangulationGroup->setVisible(triangulationGroupVisible);

  triangulateWithColorCheck->setChecked
    (config.value("PanelScan/triangulationGroup/withColor",false).toBool());

  triangulateChoiceCombo->setCurrentText
    (config.value("PanelScan/triChoice","Laser 1+2").toString());
  
  bool view3DGroupVisible =
    config.value("PanelScan/view3DGroup/visible",false).toBool();
  view3DGroupCheck->setChecked(view3DGroupVisible);
  view3DGroup->setVisible(view3DGroupVisible);

  double x0 = config.value("PanelScan/quad/x0",0.0).toDouble();    
  double y0 = config.value("PanelScan/quad/y0",0.0).toDouble();    
  double x1 = config.value("PanelScan/quad/x1",0.0).toDouble();    
  double y1 = config.value("PanelScan/quad/y1",0.0).toDouble();    
  double x2 = config.value("PanelScan/quad/x2",0.0).toDouble();    
  double y2 = config.value("PanelScan/quad/y2",0.0).toDouble();    
  double x3 = config.value("PanelScan/quad/x3",0.0).toDouble();    
  double y3 = config.value("PanelScan/quad/y3",0.0).toDouble();    
  _quad.clear();
  QPointF p0(x0,y0);
  QPointF p1(x1,y1);
  QPointF p2(x2,y2);
  QPointF p3(x3,y3);
  if(p0!=p1 && p1!=p2 && p2!=p3 && p3!=p0) {
    _quad << p0 << p1 << p2 << p3 << p0;
  }

}

//////////////////////////////////////////////////////////////////////
void PanelScan::saveSettings(void) {
  QSettings config;

  // Images group
  config.setValue("PanelScan/rotation",
                  imageRotCombo->currentText());

  bool bboxGroupVisible = bboxGroupCheck->isChecked();
  config.setValue("PanelScan/bboxGroup/visible",
                  bboxGroupVisible);

  bool imagesGroupVisible = imagesGroupCheck->isChecked();
  config.setValue("PanelScan/imagesGroup/visible",
                  imagesGroupVisible);

  bool triangulationGroupVisible = triangulationGroupCheck->isChecked();
  config.setValue("PanelScan/triangulationGroup/visible",
                  triangulationGroupVisible);

  config.setValue("PanelScan/triangulationGroup/withColor",
                  triangulateWithColorCheck->isChecked());

  config.setValue("PanelScan/triChoice",
                  triangulateChoiceCombo->currentText());

  bool view3DGroupVisible = view3DGroupCheck->isChecked();
  config.setValue("PanelScan/view3DGroup/visible",
                  view3DGroupVisible);

  if(_quad.size()==5) {
    QPointF& p0 = _quad[0];
    config.setValue("PanelScan/quad/x0",p0.x());
    config.setValue("PanelScan/quad/y0",p0.y());
    QPointF& p1 = _quad[1];
    config.setValue("PanelScan/quad/x1",p1.x());
    config.setValue("PanelScan/quad/y1",p1.y());
    QPointF& p2 = _quad[2];
    config.setValue("PanelScan/quad/x2",p2.x());
    config.setValue("PanelScan/quad/y2",p2.y());
    QPointF& p3 = _quad[3];
    config.setValue("PanelScan/quad/x3",p3.x());
    config.setValue("PanelScan/quad/y3",p3.y());
  }
}

// } Settings
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Images Group {

void PanelScan::_on_workDirectoryChanged() {
  updateImageFiles();
}

void PanelScan::_on_currentImageChanged
(const QModelIndex & current, const QModelIndex & previous) {
  Q_UNUSED(current);
  Q_UNUSED(previous);
  QString rot = imageRotCombo->currentText();
  QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
  updateImage(qImg);
}

//////////////////////////////////////////////////////////////////////
void PanelScan::updateCurrentImage() {
  QString rot = imageRotCombo->currentText();
  QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
  updateImage(qImg);
}

//////////////////////////////////////////////////////////////////////
void PanelScan::updateImage
(QImage image, QVector<Vector2d>* pDetectedPoints) {

  auto mainWin = getApp()->getMainWindow();
  if (!mainWin) {
    return;
  }
  auto glWidget = mainWin->getImgGLWidget();

  if (image.isNull()) {
    glWidget->clearImage();
    return;
  }

  QPainter painter(&image);
  QPen pen;

  if(_quad.size()>0) {
    pen.setColor(Qt::yellow);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawPolygon(_quad);
  }

  if(pDetectedPoints!=nullptr) {
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    painter.setPen(pen);
    for(int iPt=0;iPt<pDetectedPoints->size();iPt++) {
      const Vector2d& pt = (*pDetectedPoints)[iPt];
      painter.drawPoint(pt.x(),pt.y());
      painter.drawEllipse(pt.x()-1, pt.y()-1,3,3);
    }
  }

  //update image
  glWidget->setQImage(image);
  QTimer::singleShot(10, glWidget, SLOT(update()));
}

//////////////////////////////////////////////////////////////////////
void PanelScan::updateImageFiles() {
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
    ScannerCalibData& scannerCalibData = mainWin->getScannerCalibData();
    scannerCalibData.anglePerFrame = anglePerFrame;
    anglePerFrameLine->setText(QString::number(anglePerFrame));

  } else {
    std::cerr << "  "
              << qPrintable(dir.absolutePath()) << "/capture.log does not exist"
              << std::endl;
  }

  GuiUtil::updateFilesItems(&_model, fileList);

  // show the first image in imageList
  QAbstractItemModel *model = imageList->model();
  int nImages = model->rowCount();
  if(nImages>0) {
    imageList->setCurrentIndex(model->index(0,0));
    on_imageRotCombo_currentIndexChanged(0);
    updateCurrentImage();
  }
}

void PanelScan::on_imagesUpdateButton_clicked() {
  std::cerr << "PanelScan::on_imagesUpdateButton_clicked() {\n";
  updateImageFiles();
  std::cerr << "}\n";
}

void PanelScan::on_imagesNextButton_clicked() {
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

void PanelScan::on_imagesPreviousButton_clicked() {
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

void PanelScan::on_imageRotCombo_currentIndexChanged(int index) {
    (void)index;
  QString rot = imageRotCombo->currentText();
  QImage qImg = GuiUtil::loadCurrentImage(imageList,rot,false);
  updateImage(qImg);
}

// } Images Group
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
int PanelScan::imageListGetFrames() {
  int nFrames = 0;

  QVector<int>& frameStart = _laserFrameStart;
  QVector<int>& img2Frame = _laserImageToFrame;
  
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
void PanelScan::on_triangulationRunButton_clicked(bool checked) {
  std::cerr
    << "PanelScan::on_triangulationRunButton_clicked() {"
    << std::endl;
  
  static bool stop = false;

  auto mainWin  = getApp()->getMainWindow();
  ScannerCalibData& scannerCalibData = mainWin->getScannerCalibData();

  if(!checked) {
    stop = true;
    statusBarLine->setText("detecting laser points and triangulating ... stopped");
    triangulateChoiceCombo->setEnabled(true);
    QApplication::processEvents();
  } else if(scannerCalibData.isCalibrated==false) {
    statusBarLine->setText("scanner is not calibrated ...");
    triangulateChoiceCombo->setEnabled(true);
    QApplication::processEvents();
  } else {
    statusBarLine->setText("detecting laser points and triangulating ...");

    wrlFileSaveButton->setEnabled(false);
    triangulateChoiceCombo->setEnabled(false);
    triangulateWithColorCheck->setEnabled(false);

    bool withColor = triangulateWithColorCheck->isChecked();

    Matrix3d const& K  = scannerCalibData.camera_K;
    Vector5d const& kc = scannerCalibData.camera_kc; // lens distortion parameters
    Matrix3d const& R  = scannerCalibData.turntableFrame;
    Vector3d const& T  = scannerCalibData.turntableCenter;

    double anglePerFrame = anglePerFrameLine->text().toDouble();
    int nFrames = imageListGetFrames();
    QAbstractItemModel *model = imageList->model();

    scanningProgress->setMaximum(nFrames);
    QVector<int>& frameStart = _laserFrameStart;

    SceneGraph* pWrl = mainWin->getSceneGraph();    
    std::vector<float>* pCoord1 = nullptr;
    std::vector<float>* pColor1 = nullptr;
    if(pWrl!=nullptr) {
      Node* node = pWrl->find("POINTS_1");
      if(node!=nullptr) pWrl->removeChild(node);
      Shape* s = new Shape();
      s->setName("POINTS_1");
      Appearance* a = new Appearance();
      s->setAppearance(a);
      Material* m = new Material();
      a->setMaterial(m);
      Color c(0.9f,0.5f,0.3f);
      m->setDiffuseColor(c);
      IndexedFaceSet* ifs = new IndexedFaceSet();
      s->setGeometry(ifs);
      pCoord1 = &(ifs->getCoord());
      if(withColor) {
        ifs->setColorPerVertex(true);
        pColor1 = &(ifs->getColor());
      }
      pWrl->addChild(s);
    }
    std::vector<float>* pCoord2 = nullptr;
    std::vector<float>* pColor2 = nullptr;
    if(pWrl!=nullptr) {
      Node* node = pWrl->find("POINTS_2");
      if(node!=nullptr) pWrl->removeChild(node);
      Shape* s = new Shape();
      s->setName("POINTS_2");
      Appearance* a = new Appearance();
      s->setAppearance(a);
      Material* m = new Material();
      a->setMaterial(m);
      Color c(0.5f,0.3f,0.9f);
      m->setDiffuseColor(c);
      IndexedFaceSet* ifs = new IndexedFaceSet();
      s->setGeometry(ifs);
      pCoord2 = &(ifs->getCoord());
      if(withColor) {
        ifs->setColorPerVertex(true);
        pColor2 = &(ifs->getColor());
      }
      pWrl->addChild(s);
    }

    QString triChoice = triangulateChoiceCombo->currentText();
    bool laser1 = triChoice=="Laser 1" || triChoice=="Laser 1+2";
    bool laser2 = triChoice=="Laser 2" || triChoice=="Laser 1+2";

    QModelIndex    indx;
    QStandardItem* item;
    QString rot = imageRotCombo->currentText();
    for(int iF = 0;stop==false && iF<nFrames;iF++) {
      frameNumberLine->setText(QString::number(iF));

      int frameImageCount = frameStart[iF+1]-frameStart[iF];
      if(frameImageCount==3) {

        int row = frameStart[iF];

        double angleDeg = static_cast<double>(iF)*anglePerFrame;
        double angleRad = angleDeg*(M_PI/180.0)+(M_PI/2.0)-(M_PI/8.0);
        double c = std::cos(angleRad);
        double s = std::sin(angleRad);
        Matrix3d R_angle;
        R_angle << c, -s, 0, s, c, 0, 0, 0, 1;
        Matrix3d R_ang_Rt = R_angle*R.transpose();

        // first image with both lasers off
        indx = model->index(row,0);
        imageList->setCurrentIndex(indx);
        QImage qImg0 = GuiUtil::loadCurrentImage(imageList,rot,false);

        item = GuiUtil::getCurrentItem(imageList, &indx);
        if(item!=nullptr) item->setCheckState(Qt::Checked);

        if(_quad.size()==0) {
          int width  = qImg0.width();
          int height = qImg0.height();
          cv::Size imageSize(width,height);

          double x0 = 0.0;
          double x1 = static_cast<double>(width-1);
          double y0 = 0.0;
          double y1 = static_cast<double>(height-1);

          // determine bounding quad
          QPointF p00(x0,y0);
          QPointF p01(x0,y1);
          QPointF p10(x1,y0);
          QPointF p11(x1,y1);
          _quad << p00 << p01 << p11 << p10 << p00;
          updateState();
        }

        // detect in first laser image
        QVector<Vector2d> detectedPoints1;
        QVector<Vector3d>* pColorPoints1 = nullptr;
        if(withColor) pColorPoints1 = new QVector<Vector3d>();
        indx = model->index(row+1,0);
        imageList->setCurrentIndex(indx);
        QImage qImg1 = GuiUtil::loadCurrentImage(imageList,rot,false);
        item = GuiUtil::getCurrentItem(imageList, &indx);
        if(item!=nullptr) item->setCheckState(Qt::Checked);
        hw4::detectLaserPoints(qImg1,qImg0,_quad,detectedPoints1,pColorPoints1);
        updateImage(qImg1,&detectedPoints1);
        QApplication::processEvents();

        if(laser1) {
          QVector<Vector3d> triLaserPoints1; // in camera coordinates
          hw3::triangulate(K, kc, scannerCalibData.laserPlane1,
                           detectedPoints1, triLaserPoints1);
          int nPts1 = triLaserPoints1.size();
          for(int iPt=0;iPt<nPts1;iPt++) {
            QPointF p(detectedPoints1[iPt][0],detectedPoints1[iPt][1]);
            if(_quad.containsPoint(p,Qt::OddEvenFill)) {
              Vector3d& p = triLaserPoints1[iPt];
              Vector3d q = R_ang_Rt*(p-T);
              pCoord1->push_back(q[0]);
              pCoord1->push_back(q[1]);
              pCoord1->push_back(q[2]+28.0); // add turntable height
              if(withColor) {
                Vector3d& c = (*pColorPoints1)[iPt];
                pColor1->push_back(c[0]);
                pColor1->push_back(c[1]);
                pColor1->push_back(c[2]);
              }
            }
          }
        }
        if(withColor) delete pColorPoints1;

        // detect in second laser image
        QVector<Vector2d> detectedPoints2;
        QVector<Vector3d>* pColorPoints2 = nullptr;
        if(withColor) pColorPoints2 = new QVector<Vector3d>();
        indx = model->index(row+2,0);
        imageList->setCurrentIndex(indx);
        QImage qImg2 = GuiUtil::loadCurrentImage(imageList,rot,false);
        item = GuiUtil::getCurrentItem(imageList, &indx);
        if(item!=nullptr) item->setCheckState(Qt::Checked);
        hw4::detectLaserPoints(qImg2,qImg0,_quad,detectedPoints2,pColorPoints2);
        updateImage(qImg2,&detectedPoints2);
        QApplication::processEvents();

        if(laser2) {
          QVector<Vector3d> triLaserPoints2; // in camera coordinates
          hw3::triangulate(K, kc, scannerCalibData.laserPlane2,
                           detectedPoints2, triLaserPoints2);
          int nPts2 = triLaserPoints2.size();
          for(int iPt=0;iPt<nPts2;iPt++) {
            QPointF p(detectedPoints2[iPt][0],detectedPoints2[iPt][1]);
            if(_quad.containsPoint(p,Qt::OddEvenFill)) {
              Vector3d& p = triLaserPoints2[iPt];
              Vector3d q = R_ang_Rt*(p-T);
              pCoord2->push_back(q[0]);
              pCoord2->push_back(q[1]);
              pCoord2->push_back(q[2]+28.0); // add turntable height
              if(withColor) {
                Vector3d& c = (*pColorPoints2)[iPt];
                pColor2->push_back(c[0]);
                pColor2->push_back(c[1]);
                pColor2->push_back(c[2]);
              }
            }
          }
        }

        if(withColor) delete pColorPoints2;
        
        // TODO Wed Nov  6 20:36:45 2024
        // - update 3d view
        mainWin->setSceneGraph(pWrl,false);
        mainWin->refresh();
        std::cerr << "  nPoints1 = " << (pCoord1->size()/3) << std::endl;
        std::cerr << "  nPoints2 = " << (pCoord2->size()/3) << std::endl;

        if(_loopWaitMsec>0) GuiUtil::wait(_loopWaitMsec);
      }

      scanningProgress->setValue(iF+1);
      QApplication::processEvents();
    }
    frameNumberLine->setText("0");
    
    stop = false;
    statusBarLine->setText("detecting laser chessboards ... done");

    wrlFileSaveButton->setEnabled(true);
    triangulateWithColorCheck->setEnabled(true);
    triangulateChoiceCombo->setEnabled(true);
    triangulationRunButton->setChecked(false);


    QApplication::processEvents();
  }

  std::cerr << "}" << std::endl;
}

// } Triangulation Group
//////////////////////////////////////////////////////////////////////
