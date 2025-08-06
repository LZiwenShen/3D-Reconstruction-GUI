// Time-stamp: <2024-11-13 17:06:10 taubin>
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

#define TURNTABLE_WRL "../../assets/turntable_160x32mm.wrl"

#include <iostream>
#include "MainWindow.hpp"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

#include "Application.hpp"
#include "AboutDialog.hpp"

#include <io/LoaderWrl.hpp>
#include <io/SaverWrl.hpp>

#include <util/GuiUtil.hpp>

int MainWindow::_timerInterval = 20;

QMap<QString,
     std::function<QWidget*(QWidget *parent)>> &
     MainWindow::getRegisteredPanels(void) {
  typedef QMap<QString,std::function<QWidget*(QWidget *parent)>> MapType;
  static MapType * _registeredPanels = new MapType;
  return *_registeredPanels;
}

bool MainWindow::registerPanel
(QString panelName,
 std::function<QWidget*(QWidget *parent)> panelFactory) {
  getRegisteredPanels().insert(panelName, panelFactory);
  return true;
}

MainWindow::MainWindow(QWidget * parent) :
  QMainWindow(parent),
  _firstShow(true),
  _timer(nullptr),
  _bdryTop(5),
  _bdryBottom(5),
  _bdryLeft(5),
  _bdryCenter(5),
  _bdryRight(5),
  _toolsWidth(300),
  _scanningFolder(),
  _cameraCalibFolder(),
  _turntableCalibFolder(),
  _laserCalibFolder(),
  _scannerCalibData() {

  std::cerr << "MainWindow::MainWindow() {" << std::endl;
  
  _timer = new QTimer(this);
  _timer->setInterval(_timerInterval);

  setupUi(this);
  
  setWindowTitle(APP_NAME);

  connect(_timer, SIGNAL(timeout()), wrlGLWidget, SLOT(update()));

  wrlGLWidget->setBackgroundColor(qRgb(200, 200, 200));
  wrlGLWidget->setMaterialColor(qRgb(175, 120, 50));

  splitter->setOpaqueResize(false);

  // connect QSplitter::splitterMoved(int pos, int index) to

  LoaderWrl* wrlLoader = new LoaderWrl();
  _loader.registerLoader(wrlLoader);

  SaverWrl* wrlSaver = new SaverWrl();
  _saver.registerSaver(wrlSaver);
  
  //fill the panel combo
  auto panelNames = getRegisteredPanels().keys();
  foreach (auto item, panelNames) {
    panelCombo->insertItem(999, item);
  }

  // restore settings
  QSettings config;

  restoreGeometry(config.value("MainWindow/geometry").toByteArray());
  restoreState(config.value("MainWindow/windowState").toByteArray());
  
  QString currentDir = QDir::currentPath();
  _scanningFolder       =
    config.value("MainWindow/scanningFolder",currentDir).toString();
  _cameraCalibFolder    =
    config.value("MainWindow/cameraCalibFolder",currentDir).toString();
  _turntableCalibFolder =
    config.value("MainWindow/turntableCalibFolder",currentDir).toString();
  _laserCalibFolder     =
    config.value("MainWindow/laserCalibFolder",currentDir).toString();

  bool isCalibrated =
    config.value("MainWindow/scannerCalibData/isCalibrated",false).toBool();
  if(!isCalibrated) {
    _scannerCalibData.clear();
  } else {

    bool isCalibrated = true;

    int n;
    QString str;
    str = config.value("MainWindow/scannerCalibData/camera_K","").toString();
    std::cerr << "camera_K = " << std::endl;
    std::cerr << "\"" << qPrintable(str) << "\""<< std::endl;
    Matrix3d& K = _scannerCalibData.camera_K;
    n = sscanf(str.toStdString().c_str(),"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
               &(K(0,0)),&(K(0,1)),&(K(0,2)),
               &(K(1,0)),&(K(1,1)),&(K(1,2)),
               &(K(2,0)),&(K(2,1)),&(K(2,2)));
    if(n<9) isCalibrated = false;

    str = config.value("MainWindow/scannerCalibData/camera_kc","").toString();
    std::cerr << "camera_kc = " << std::endl;
    std::cerr << "\"" << qPrintable(str) << "\"" << std::endl;
    Vector5d& kc = _scannerCalibData.camera_kc;
    n = sscanf(str.toStdString().c_str(),"%lf %lf %lf %lf %lf",
               &(kc(0)),&(kc(1)),&(kc(2)),&(kc(3)),&(kc(4)));
    if(n<5) isCalibrated = false;
    
    str = config.value("MainWindow/scannerCalibData/turntableFrame","").toString();
    std::cerr << "turntableFrame = " << std::endl;
    std::cerr << "\"" << qPrintable(str) << "\"" << std::endl;
    Matrix3d& R = _scannerCalibData.turntableFrame;
    n = sscanf(str.toStdString().c_str(),"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
               &(R(0,0)),&(R(0,1)),&(R(0,2)),
               &(R(1,0)),&(R(1,1)),&(R(1,2)),
               &(R(2,0)),&(R(2,1)),&(R(2,2)));
    if(n<9) isCalibrated = false;
    
    str = config.value("MainWindow/scannerCalibData/turntableCenter","").toString();
    std::cerr << "turntableCenter = " << std::endl;
    std::cerr << "\"" << qPrintable(str) << "\"" << std::endl;
    Vector3d& T = _scannerCalibData.turntableCenter;
    n = sscanf(str.toStdString().c_str(),"%lf %lf %lf",
               &(T(0)),&(T(1)),&(T(2)));
    if(n<3) isCalibrated = false;
    
    _scannerCalibData.anglePerFrame =
      config.value("MainWindow/scannerCalibData/anglePerFrame",0.0).toDouble();
    
    str = config.value("MainWindow/scannerCalibData/laserPlane1","").toString();
    std::cerr << "laser1 = " << std::endl;
    std::cerr << "\"" << qPrintable(str) << "\"" << std::endl;
    Vector4d& L1 = _scannerCalibData.laserPlane1;
    n = sscanf(str.toStdString().c_str(),"%lf %lf %lf %lf",
               &(L1(0)),&(L1(1)),&(L1(2)),&(L1(3)));
    if(n<4) isCalibrated = false;
    
    str = config.value("MainWindow/scannerCalibData/laserPlane2","").toString();
    std::cerr << "laser2 = " << std::endl;
    std::cerr << "\"" << qPrintable(str) << "\"" << std::endl;
    Vector4d& L2 = _scannerCalibData.laserPlane2;
    n = sscanf(str.toStdString().c_str(),"%lf %lf %lf %lf",
               &(L2(0)),&(L2(1)),&(L2(2)),&(L2(3)));
    if(n<4) isCalibrated = false;

    _scannerCalibData.isCalibrated = isCalibrated;
  }

  auto panel = config.value("MainWindow/currentPanel").toString().trimmed();
  if (!panel.isEmpty() && panelNames.contains(panel)) {
    panelCombo->setCurrentText(panel);
  }

  QStringList workDirCombo_items;
  workDirCombo_items         <<
    "Scanning Folder"        <<
    "Camera Calib Folder"    <<
    "Turntable Calib Folder" <<
    "Laser Calib Folder";

  QString workDirSel = (panel=="Scan" || panel=="Capture")?
    "Scanning Folder": "Camera Calib Folder";
  GuiUtil::fillCombo(workDirCombo, workDirCombo_items, workDirSel);
    
  workDirUpdated();

  showStatusBarMessage("");

  std::cerr << "}" << std::endl;
}

void MainWindow::showEvent(QShowEvent *event) {
  (void)event;
  if(_firstShow) {
    QFile qf(":/3dp-scanner-2021.wrl");
    loadSceneGraph(qf);
    _firstShow = false;
  }
}

void MainWindow::closeEvent(QCloseEvent *e) {
  //save settings
  QSettings config;
  config.setValue("MainWindow/geometry", saveGeometry());
  config.setValue("MainWindow/windowState", saveState());
  config.setValue("MainWindow/currentPanel", panelCombo->currentText());

  // config.setValue("MainWindow/workDirectory", workDirLine->text());

  config.setValue("MainWindow/scanningFolder",       _scanningFolder);
  config.setValue("MainWindow/cameraCalibFolder",    _cameraCalibFolder);
  config.setValue("MainWindow/turntableCalibFolder", _turntableCalibFolder);
  config.setValue("MainWindow/laserCalibFolder",     _laserCalibFolder);

  if(_scannerCalibData.isCalibrated) {
    std::stringstream s1,s2,s3,s4,s5,s6;
    config.setValue("MainWindow/scannerCalibData/isCalibrated",true);

    // s1 << _scannerCalibData.camera_K;
    Matrix3d& K = _scannerCalibData.camera_K;
    s1 << K(0,0) << " " << K(0,1) << " " << K(0,2) << " "
       << K(1,0) << " " << K(1,1) << " " << K(1,2) << " "
       << K(2,0) << " " << K(2,1) << " " << K(2,2);
    config.setValue("MainWindow/scannerCalibData/camera_K",s1.str().c_str());

    // s2 << _scannerCalibData.camera_kc;
    Vector5d& kc = _scannerCalibData.camera_kc;
    s2 << kc(0) << " " << kc(1) << " " << kc(2) << " " << kc(3) << " " << kc(4);
    config.setValue("MainWindow/scannerCalibData/camera_kc",s2.str().c_str());

    // s3 << _scannerCalibData.turntableFrame;
    Matrix3d& R = _scannerCalibData.turntableFrame;
    s3 << R(0,0) << " " << R(0,1) << " " << R(0,2) << " "
       << R(1,0) << " " << R(1,1) << " " << R(1,2) << " "
       << R(2,0) << " " << R(2,1) << " " << R(2,2);
    config.setValue("MainWindow/scannerCalibData/turntableFrame",s3.str().c_str());

    // s4 << _scannerCalibData.turntableCenter;
    Vector3d& T = _scannerCalibData.turntableCenter;
    s4 << T(0) << " " << T(1) << " " << T(2);
    config.setValue("MainWindow/scannerCalibData/turntableCenter",s4.str().c_str());

    config.setValue("MainWindow/scannerCalibData/anglePerFrame",
                    _scannerCalibData.anglePerFrame);

    // s5 << _scannerCalibData.laserPlane1;
    Vector4d& L1 = _scannerCalibData.laserPlane1;
    s5 << L1(0) << " " << L1(1) << " " << L1(2) << " " << L1(3);
    config.setValue("MainWindow/scannerCalibData/laserPlane1",s5.str().c_str());

    // s6 << _scannerCalibData.laserPlane2;
    Vector4d& L2 = _scannerCalibData.laserPlane2;
    s6 << L2(0) << " " << L2(1) << " " << L2(2) << " " << L2(3);
    config.setValue("MainWindow/scannerCalibData/laserPlane2",s6.str().c_str());

  } else {
    config.setValue("MainWindow/scannerCalibData/isCalibrated",false);
  }

  QMainWindow::closeEvent(e);
}

void MainWindow::on_splitter_splitterMoved(int pos, int index) {
    (void)pos; (void) index;
  // std::cerr << "MainWindow::on_splitter_splitterMoved() {\n";
  // std::cerr << "  pos   = " << pos   << "\n";
  // std::cerr << "  index = " << index << "\n";
  // std::cerr << "}\n";
}

void MainWindow::on_actionAbout_triggered() {
  AboutDialog d;
  d.exec();
}

void MainWindow::on_panelCombo_currentIndexChanged(int index) {
  qDebug("MainWindow::on_panelCombo_currentIndexChanged: index %d", index);
  setPanel(panelCombo->currentText());
}

void MainWindow::setPanel(QString name) {
  //remove current widgets from panel
  auto children = currPanelWidget->children();
  foreach(auto child, children) {
    auto widget = dynamic_cast<QWidget*>(child);
    if (widget) {
      widget->hide();
      widget->deleteLater();
    }
  }
  
  //delete current layout
  auto oldLayout = currPanelWidget->layout();
  if (oldLayout) {
    delete oldLayout;
  }

  //create a new layout
  QVBoxLayout * newLayout = new QVBoxLayout(currPanelWidget);
  newLayout->setContentsMargins(0, 0, 0, 0);

  //add new panel
  QWidget * panel = nullptr;
  
  auto & registeredPanels = getRegisteredPanels();
  auto panelFactory = registeredPanels.find(name);
  if (panelFactory!= registeredPanels.end()) { //panel found
    panel = (*panelFactory)(currPanelWidget);
  }

  if (!panel) { //panel not found
    imgGLWidget->clearImage();
    qDebug("MainWindow::on_panelCombo_currentIndexChanged: panel '%s' not found",
           qPrintable(name));
    return;
  }

  //setup new panel
  newLayout->addWidget(panel);
  panel->show();
}

void MainWindow::on_workDirCombo_currentIndexChanged(int index) {
  (void)index;
  QString workDirType = workDirCombo->currentText();
  if(workDirType=="Scanning Folder") // index==0-
    workDirLine->setText(_scanningFolder);
  else if(workDirType=="Camera Calib Folder") // index==1
    workDirLine->setText(_cameraCalibFolder);
  else if(workDirType=="Turntable Calib Folder") // index==2
    workDirLine->setText(_turntableCalibFolder);
  else if(workDirType=="Laser Calib Folder") // index==3
    workDirLine->setText(_laserCalibFolder);
  workDirUpdated();
}

void MainWindow::on_workDirLoadButton_clicked() {
  QString folderType = workDirCombo->currentText();

  QString dir =
    QFileDialog::getExistingDirectory
    (this, "Select "+folderType+" Directory",
     workDirLine->text(),
     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty()) {
    workDirLine->setText(dir);
    workDirUpdated();
  }
}

void MainWindow::on_workDirLine_editingFinished() {
  workDirUpdated();
}

void MainWindow::workDirUpdated(void) {
  static QMutex mutex;
  if (!mutex.tryLock(0)) return;

  // TODO Fri Oct 25 15:39:42 2024
  // --- fix to take into account ...
  // QString _scanningFolder;
  // QString _cameraCalibFolder;
  // QString _turntableCalibFolder;
  // QString _laserCalibFolder;
  
  QString dirName = workDirLine->text();;
  QString prevDirName = workDirLine->property("prevValue").toString();
  if (prevDirName.isEmpty()) prevDirName = QDir::currentPath();
  if (dirName.isEmpty()) dirName = prevDirName;

  QDir workDir(dirName);
  if (!workDir.exists()) { // if it doesn't exist: ask to create
    auto button =
      QMessageBox::question(nullptr,
                            "Warning", "Folder '" + dirName
                            + "' does not exist.\nDo you want to create it?");
    if (button == QMessageBox::Yes) {
      if (workDir.mkpath(dirName)) {
        qInfo("Folder created: %s", qPrintable(dirName));
      } else { //couldn't create
        qCritical("Folder creation failed: %s", qPrintable(dirName));
        dirName = prevDirName;
      }
    } else { //user cliked 'No'
      dirName = prevDirName;
    }
    
  } // workDir exists

  // update the value
  if (workDirLine->text() != prevDirName) {
    workDirLine->setText(dirName);
    QString workDirType = workDirCombo->currentText();
    if(workDirType=="Scanning Folder")
      _scanningFolder = dirName;
    else if(workDirType=="Camera Calib Folder")
      _cameraCalibFolder = dirName;
    else if(workDirType=="Turntable Calib Folder")
      _turntableCalibFolder = dirName;
    else if(workDirType=="Laser Calib Folder")
      _laserCalibFolder = dirName;
  }
  workDirLine->setProperty("prevValue", dirName);
  emit workDirectoryChanged();
  mutex.unlock();
}

QString MainWindow::getWorkDirectory(void) const {
  return workDirLine->text();
}

bool MainWindow::setWorkDirectory(QString path) {
  QDir workDir(path);
  if (!workDir.exists()) {
    return false;
  } else {
    workDirLine->setText(path);
    workDirUpdated();
    return true;
  }
}

void MainWindow::setChangesEnabled(bool enabled) {
  panelCombo->setEnabled(enabled);
  workDirCombo->setEnabled(enabled);
  workDirLine->setEnabled(enabled);
  workDirLoadButton->setEnabled(enabled);
}

// inherited from WrlMainWindows

void MainWindow::updateState() {
  // _toolsWidget->updateState();
}

void MainWindow::showStatusBarMessage(const QString & message) {
  statusbar->showMessage(message);
}

void MainWindow::refresh() {
  wrlGLWidget->update();
}

WrlViewerData& MainWindow::getData() const {
  return wrlGLWidget->getData();
}

SceneGraph* MainWindow::getSceneGraph() {
  return wrlGLWidget->getSceneGraph();
}

void MainWindow::setSceneGraph(SceneGraph* pWrl, bool resetHomeView) {
  wrlGLWidget->setSceneGraph(pWrl,resetHomeView);
  // wrlGLWidget->repaint();
}

SceneGraph* MainWindow::loadSceneGraph(const char* fname) {
  static char str[1024];
  snprintf(str,1024,"Trying to load \"%s\" ...",fname);
  showStatusBarMessage(QString(str));
  SceneGraph* pWrl = new SceneGraph();
  if(_loader.load(fname,*pWrl)) { // if success
    snprintf(str,1024,"Loaded \"%s\"",fname);
    pWrl->updateBBox();
    wrlGLWidget->setSceneGraph(pWrl,true);
    updateState();
  } else {
    snprintf(str,1024,"Unable to load \"%s\"",fname);
    delete pWrl;
    pWrl = (SceneGraph*)0;
  }
  showStatusBarMessage(QString(str));
  return pWrl;
}

SceneGraph* MainWindow::loadSceneGraph(QFile & qf) {
  static char str[1024];
  std::string qfFileName = qf.fileName().toStdString();
  const char* fname = qfFileName.c_str();
  snprintf(str,1024,"Trying to load \"%s\" ...",fname);
  showStatusBarMessage(QString(str));
  SceneGraph* pWrl = new SceneGraph();
  if(_loader.load(qf,*pWrl)) { // if success
    snprintf(str,1024,"Loaded \"%s\"",fname);
    pWrl->updateBBox();
    wrlGLWidget->setSceneGraph(pWrl,true);
    updateState();
  } else {
    snprintf(str,1024,"Unable to load \"%s\"",fname);
    delete pWrl;
    pWrl = (SceneGraph*)0;
  }
  showStatusBarMessage(QString(str));
  return pWrl;
}

int MainWindow::getImgGLWidgetWidth() {
  return imgGLWidget->size().width();
}

int MainWindow::getImgGLWidgetHeight() {
  return imgGLWidget->size().height();
}

int MainWindow::getWrlGLWidgetWidth() {
  return wrlGLWidget->size().width();
}

int MainWindow::getWrlGLWidgetHeight() {
  return wrlGLWidget->size().height();
}

int MainWindow::getPanelsWidgetWidth() {
  return panelsWidget->size().width();
}

int MainWindow::getPanelsWidgetHeight() {
  return panelsWidget->size().height();
}

void MainWindow::showImgGLWidget(bool value) {
  imgGLWidget->setVisible(value);
}

void MainWindow::showWrlGLWidget(bool value) {
  wrlGLWidget->setVisible(value);
}

void MainWindow::resizeImgGLWidget(int w, int h) {
  imgGLWidget->resize(w,h);
}

void MainWindow::resizeWrlGLWidget(int w, int h) {
  wrlGLWidget->resize(w,h);
}

//////////////////////////////////////////////////////////////////////
// void MainWindow::on_wrlFileLoadButton_clicked() {
//
//   std::string filename;
//
//   // stop animation
//   _timer->stop();
//
//   QFileDialog fileDialog(this);
//   fileDialog.setFileMode(QFileDialog::ExistingFile);
//   fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
//   fileDialog.setNameFilter(tr("VRML Files (*.wrl)"));
//   QStringList fileNames;
//   if(fileDialog.exec()) {
//     fileNames = fileDialog.selectedFiles();
//     if(fileNames.size()>0)
//       filename = fileNames.at(0).toStdString();
//   }
//
//   if (filename.empty()) {
//     showStatusBarMessage("load filename is empty");
//   } else {
//     loadSceneGraph(filename.c_str());
//   } 
//
//   // restart animation
//   _timer->start(_timerInterval);
// }

//////////////////////////////////////////////////////////////////////
void MainWindow::on_wrlFileSaveButton_clicked() {

  std::string filename;

  // stop animation
  _timer->stop();

  QFileDialog
    fileDialog(this,"Save Point Cloud",getWorkDirectory(),"*.wrl");

  fileDialog.setFileMode(QFileDialog::AnyFile); // allowed to select only one 
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setNameFilter(tr("VRML Files (*.wrl)"));
  QStringList fileNames;
  if(fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
    if(fileNames.size()>0)
      filename = fileNames.at(0).toStdString();
  }

  // restart animation
  _timer->start(_timerInterval);

  if (filename.empty()) {
    showStatusBarMessage("save filename is empty");
  } else {

    static char str[1024];

    snprintf(str,1024,"Saving \"%s\" ...",filename.c_str());
    showStatusBarMessage(QString(str));

    SceneGraph* pWrl = wrlGLWidget->getSceneGraph();

    if(_saver.save(filename.c_str(),*pWrl)) {
      snprintf(str,1024,"Saved \"%s\"", filename.c_str());
    } else {
      snprintf(str,1024,"Unable to save \"%s\"",filename.c_str());
    }

    showStatusBarMessage(QString(str));
  }
}
