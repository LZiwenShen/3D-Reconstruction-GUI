// Time-stamp: <2021-04-03 15:11:47 taubin>
//
// Software developed for the Spring 2021 Brown University course
// ENGN 2502 3D Photography
// Copyright (c) 2016-2021, Daniel Moreno and Gabriel Taubin
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

#include "matlab_io_util.hpp"

//////////////////////////////////////////////////////////////////////
bool matlab::get2DMatrixVector2d
(const MatlabMatrixType & value, /*output*/ QVector<Vector2d>& data,
 const int rows) {

  // typedef QVector<QVector<QVector<double>>> MatlabMatrixType;

  if (value.size() != 1) return false;
  auto & mat = value[0];
  if (mat.size() != 2) return false;
  auto & col0 = mat[0];
  if (col0.size() != rows) return false;
  auto & col1 = mat[1];
  if (col1.size() != rows) return false;

  data.clear();
  for (int i = 0; i < rows; ++i) {
    Vector2d v2(col0[i], col1[i]);
    data.push_back(v2);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////
bool matlab::set2DMatrixVector2d
(const QVector<Vector2d>& data, /*output*/ MatlabMatrixType & value,
 const int rows) {

  // typedef QVector<QVector<QVector<double>>> MatlabMatrixType;

  value.clear();

  if(rows<=0 || rows!=static_cast<int>(data.size())) return false;

  QVector<QVector<double>> mat;
  value.append(mat);
  QVector<double> col0;
  QVector<double> col1;
  mat.append(col0);
  mat.append(col1);

  for (int i = 0; i < rows; ++i) {
    Vector2d v2 = data[i];
    col0.append(v2[0]);
    col1.append(v2[1]);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////
bool matlab::get2DMatrixDouble
(const MatlabMatrixType & value, /*output*/ double * data,
 const int rows, const int cols, const MatrixType type) {

  // typedef QVector<QVector<QVector<double>>> MatlabMatrixType;

  if (value.size() != 1) {
    return false;
  }

  auto & mat = value[0];
  if (mat.size() != cols) {
    return false;
  }

  for (int w = 0; w < cols; ++w) {
    auto & vec = mat[w];
    if (vec.size() != rows) {
      return false;
    }
    for (int h = 0; h < rows; ++h) {
      auto index = (type == RowMajor ? h*cols + w : w*rows + h);
      data[index] = vec[h];
    }
  }
  
  return true;
}

//////////////////////////////////////////////////////////////////////
bool matlab::set2DMatrixDouble
(const double * data, /*output*/ MatlabMatrixType & value,
 const int rows, const int cols, const MatrixType type) {

  // typedef QVector<QVector<QVector<double>>> MatlabMatrixType;
  value.clear();
  if(data==nullptr || rows<1 || cols<1) return false;

  QVector<QVector<double>> mat;
  for (int w = 0; w < cols; ++w) {
    QVector<double> col_w;
    for (int h = 0; h < rows; ++h) {
      auto index = (type == RowMajor ? h*cols + w : w*rows + h);
      double data_wh = data[index];
      col_w.append(data_wh);
    }
    mat.append(col_w);
  }
  value.append(mat);

  return true;
}

//////////////////////////////////////////////////////////////////////
bool matlab::get2DMatrixDouble
(const QString name, const QVariantMap & varMap, /*output*/ double * data,
 const int rows, const int cols, const MatrixType type) {

  if (name.isEmpty() || !varMap.contains(name) ||
      rows<1 || cols<1 || data==nullptr) {
    return false;
  }

  // typedef QVector<QVector<QVector<double>>> MatlabMatrixType;  
  MatlabMatrixType value = varMap[name].value<MatlabMatrixType>();

  return get2DMatrixDouble(value,data,rows,cols,type);
}

//////////////////////////////////////////////////////////////////////
bool matlab::set2DMatrixDouble
(const double * data, const QString name, /*output*/ QVariantMap& varMap,
 const int rows, const int cols, const MatrixType type) {

  if (name.isEmpty() || varMap.contains(name) ||
      rows<1 || cols<1 || data==nullptr) {
    return false;
  }

  // typedef QVector<QVector<QVector<double>>> MatlabMatrixType;  
  MatlabMatrixType value;
  if(set2DMatrixDouble(data,value,rows,cols,type)==false)
    return false;

  QVariant qv = QVariant::fromValue(value);
  varMap.insert(name,qv);

  return true;
}
