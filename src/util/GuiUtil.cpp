// Time-stamp: <2021-05-16 21:03:25 taubin>
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

#include "GuiUtil.hpp"

#include <QComboBox>
#include <QLineEdit>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QListView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDir>

#include "Application.hpp"

int GuiUtil::fillCombo
(QComboBox *combo, const QStringList &list, const QString &defaultSelection) {
  //save current value
  auto currentText = combo->currentText();

  //block signals
  combo->blockSignals(true);

  //clear 
  combo->clear();

  //update
  combo->addItems(list);

  //set current
  bool useDefault = currentText.isEmpty();
  if (!useDefault) { //search for the previous value
    auto index = combo->findText(currentText);
    useDefault = (index < 0);
    if (!useDefault) {
      combo->setCurrentIndex(index);
    }
  }
  if (useDefault) { //search for the default value
    auto index = combo->findText(defaultSelection);
    combo->setCurrentIndex((index<0 ? 0 : index));
  }

  //unblock
  combo->blockSignals(false);

  //notify if changed
  if (currentText != combo->currentText()) { //changed
    emit combo->currentIndexChanged(combo->currentIndex());
  }

  return combo->count();
}

bool GuiUtil::validateFloat
(QLineEdit * line, float minVal, float maxVal, float defaultValue) {
  bool ok = false;
  if (line) {
    float val = line->text().toFloat(&ok);
    if (!ok) { val = defaultValue; }
    if (val < minVal) { val = minVal; }
    if (val > maxVal) { val = maxVal; }
    line->setText(QString::number(val));
  }
  return ok;
}

void GuiUtil::wait(int msecs) {
  QElapsedTimer timer;
  timer.start();
  while (timer.elapsed()<msecs) {
    QCoreApplication::processEvents();
  }
}

QStandardItem *GuiUtil::getItem
(QListView * listView, QModelIndex& modelIndex) {

  if (listView==nullptr) return nullptr;
  
  QStandardItemModel* model =
    dynamic_cast<QStandardItemModel*>(listView->model());
  if(model==nullptr) return nullptr;

  QStandardItem* item = model->itemFromIndex(modelIndex);

  return item;
}

QStandardItem *GuiUtil::getCurrentItem
(QListView * listView, QModelIndex * selectedIndex) {

  if (listView==nullptr) return nullptr;
  
  QStandardItemModel* model =
    dynamic_cast<QStandardItemModel*>(listView->model());
  if(model==nullptr) return nullptr;

  QItemSelectionModel* selectionModel = listView->selectionModel();
  if (selectionModel==nullptr) return nullptr;
  
  QModelIndex modelIndex = selectionModel->currentIndex();
  QStandardItem* item = model->itemFromIndex(modelIndex);
  
  if (selectedIndex) *selectedIndex = modelIndex;

  return item;
}

QImage GuiUtil::loadCurrentImage
(QListView * listView, QString rotation, bool displayError, QString subdir) {

  QModelIndex modelIndex;
  QStandardItem* item = getCurrentItem(listView, &modelIndex);
  if (item==nullptr) { //no item selected
    if (displayError) {
      QMessageBox::critical(getApp()->getMainWindow(),
                            "Error", "No item is selected.");
    }
    return QImage();
  }

  QFileInfo info(item->data().toString());
  auto imageName = info.fileName();
  auto imagePath = info.absolutePath();

  QDir dir(imagePath);
  if (!subdir.isEmpty()) {
    dir.cd(subdir);
  }
  QString filename = dir.absoluteFilePath(imageName);
  QImage qImg;
  if(!qImg.load(filename) ||
     !(rotation=="-90" || rotation=="0" || rotation=="90")) {
    
    if (displayError) {
      QMessageBox::critical(getApp()->getMainWindow(),
                            "Error", QString("Image load failed: %1").arg(filename));
    }
    return QImage();
  }
  qImg.setText("filename", filename);
  qImg.setText("index", QString::number(modelIndex.row()));


  // TODO Sun Apr 11 12:42:36 2021
  // if(rotation!="0") rotae

  // QImage QImage::transformed
  //   (const QTransform &matrix,
  //   Qt::TransformationMode mode = Qt::FastTransformation) const    
  if(rotation=="0") {
    return qImg;
  } else {
    // QTransform(qreal m11, qreal m12, qreal m13,
    //            qreal m21, qreal m22, qreal m23,
    //            qreal m31, qreal m32, qreal m33 = 1.0)
    
    int w = qImg.width();
    int h = qImg.height();
    if(rotation=="-90") {

      // x' = y
      // y' = w-x

      // QTransform tr(0,1,0,-1,0,w,0,0,1);
      QTransform tr(0,-1,0,1,0,0,0,w,1);

      return qImg.transformed(tr);

    } else if(rotation=="90") {
      // x' = h-y
      // y' = x

      // QTransform tr(0,-1,h,1,0,0,0,0,1);
      QTransform tr(0,1,0,-1,0,0,h,0,1);

      return qImg.transformed(tr);
    }
  }
  return QImage();
}

void GuiUtil::updateFilesItems(QStandardItemModel * model, QStringList fileList) {
  if (!model) { //unsupported model (or null)
    return;
  }

  //update the model
  model->clear();
  auto parent = model->invisibleRootItem();
  foreach(auto filename, fileList) {
    QFileInfo info(filename);
    QStandardItem *item = new QStandardItem(info.fileName());
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled |
                   Qt::ItemIsUserCheckable |
                   Qt::ItemNeverHasChildren);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData(info.absoluteFilePath());
    parent->appendRow(item);
  }
}
