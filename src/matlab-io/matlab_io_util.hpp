// Time-stamp: <2021-04-03 12:23:10 taubin>
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

#ifndef _matab_io_util_hpp_
#define _matab_io_util_hpp_

#include <QString>
#include <QVector>
#include <QVariantMap>

#include <util/MatrixUtil.hpp>

typedef QVector<QVector<QVector<double>>> MatlabMatrixType;

namespace matlab
{
  enum MatrixType { ColMajor, RowMajor };

  bool get2DMatrixVector2d
  (const MatlabMatrixType & value, /*output*/QVector<Vector2d>& data,
   const int rows);

  bool set2DMatrixVector2d
  (const QVector<Vector2d>& data, /*output*/ MatlabMatrixType & value,
   const int rows);

  bool get2DMatrixDouble
  (const MatlabMatrixType & value, /*output*/ double * data,
   const int rows, const int cols, const MatrixType type = RowMajor);

  bool set2DMatrixDouble
  (const double * data, /* output */ MatlabMatrixType & value,
   const int rows, const int cols, const MatrixType type = RowMajor);

  bool get2DMatrixDouble
  (const QString name, const QVariantMap & varMap, /*output*/ double *data,
   const int rows, const int cols, const MatrixType type = RowMajor);

  bool set2DMatrixDouble
  (const double * data, const QString name, /*output*/ QVariantMap& varMap,
   const int rows, const int cols, const MatrixType type = RowMajor);
  
};

#endif //_matab_io_util_hpp_
