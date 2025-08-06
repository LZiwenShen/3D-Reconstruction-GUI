// Time-stamp: <2024-09-21 12:15:52 taubin>
//
// Software License Agreement (BSD License)
//
//  Copyright (c) 2012, Willow Garage, Inc.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above
//     copyright notice, this list of conditions and the following
//     disclaimer in the documentation and/or other materials provided
//     with the distribution.
//   * Neither the name of Willow Garage, Inc. nor the names of its
//     contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
//  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
//  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
//  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//  SUCH DAMAGE.
//
//  File:    MatlabIO.cpp
//  Author:  Hilton Bristow
//  Created: Jun 27, 2012
//
// Modified by Daniel Moreno  Feb 9 2015
// Modified by Gabriel Taubin Mar 8 2016
// Modified by Gabriel Taubin Fri Apr 2 2021

#include "MatlabIO.hpp"

#include <QVector>

#include <cerrno>
#include <cstring>
#include <cassert>
// #include <iostream>

#define MATLAB_5_HEADER \
  "MATLAB 5.0 MAT-file, Platform: PCWIN64, Created on: "

// static
const char* MatlabIO::tagType(const uint32_t& tag_type) {
  switch(tag_type) {
  case MAT_INT8:       return       "MAT_INT8"; break;
  case MAT_UINT8:      return      "MAT_UINT8"; break;
  case MAT_INT16:      return      "MAT_INT16"; break;
  case MAT_UINT16:     return     "MAT_UINT16"; break;
  case MAT_INT32:      return      "MAT_INT32"; break;
  case MAT_UINT32:     return     "MAT_UINT32"; break;
  case MAT_FLOAT:      return      "MAT_FLOAT"; break;
  case MAT_DOUBLE:     return     "MAT_DOUBLE"; break;
  case MAT_INT64:      return      "MAT_INT64"; break;
  case MAT_UINT64:     return     "MAT_UINT64"; break;
  case MAT_MATRIX:     return     "MAT_MATRIX"; break;
  case MAT_COMPRESSED: return "MAT_COMPRESSED"; break;
  case MAT_UTF8:       return       "MAT_UTF8"; break;
  case MAT_UTF16:      return      "MAT_UTF16"; break;
  case MAT_UTF32:      return      "MAT_UTF32"; break;
  default: break;
  }
  return "UNSUPPORTED";
}

// static
const char* MatlabIO::matType(const uint32_t& mat_type) {
  switch(mat_type) {
  case MAT_CELL_CLASS:   return   "MAT_CELL_CLASS"; break;
  case MAT_STRUCT_CLASS: return "MAT_STRUCT_CLASS"; break;
  case MAT_OBJECT_CLASS: return "MAT_OBJECT_CLASS"; break;
  case MAT_CHAR_CLASS:   return   "MAT_CHAR_CLASS"; break;
  case MAT_SPARSE_CLASS: return "MAT_SPARSE_CLASS"; break;
  case MAT_DOUBLE_CLASS: return "MAT_DOUBLE_CLASS"; break;
  case MAT_FLOAT_CLASS:  return  "MAT_FLOAT_CLASS"; break;
  case MAT_INT8_CLASS:   return   "MAT_INT8_CLASS"; break;
  case MAT_UINT8_CLASS:  return  "MAT_UINT8_CLASS"; break;
  case MAT_INT16_CLASS:  return  "MAT_INT16_CLASS"; break;
  case MAT_UINT16_CLASS: return "MAT_UINT16_CLASS"; break;
  case MAT_INT32_CLASS:  return  "MAT_INT32_CLASS"; break;
  case MAT_UINT32_CLASS: return "MAT_UINT32_CLASS"; break;
  case MAT_INT64_CLASS:  return  "MAT_INT64_CLASS"; break;
  case MAT_UINT64_CLASS: return "MAT_UINT64_CLASS"; break;
  default: break;
  }
  return "UNSUPPORTED";
}

MatlabIO::MatlabIO():
  header_(MATLAB_5_HEADER),
  subsys_("        "),
  endian_("IM"),
  _version(VERSION_5),
  _byte_swap(false),
  _bytes_read(128),
  _filename(""),
  _fid() {

  // for (unsigned int n = 0; n < HEADER_LENGTH+1; ++n) header_[n] = '\0';
  // for (unsigned int n = 0; n < SUBSYS_LENGTH+1; ++n) subsys_[n] = '\0';
  // for (unsigned int n = 0; n < ENDIAN_LENGTH+1; ++n) endian_[n] = '\0';


}

/*! @brief Open a filestream for reading or writing
 *
 * @param filename the full name and filepath of the file
 * @param mode either "r" for reading or "w" for writing
 * @return true if the file open succeeded, false otherwise
 */

bool MatlabIO::open(QString filename, QString mode) {
  // open the file
  _filename = filename;
  if (mode.compare("r") == 0)
    _fid.open(qPrintable(filename), std::fstream::in  | std::fstream::binary);
  if (mode.compare("w") == 0)
    _fid.open(qPrintable(filename), std::fstream::out | std::fstream::binary);
  return !_fid.fail();
}

/*! @brief close the filestream and release all resources
 *
 * @return true if the filestream was successfully closed,
 * false otherwise. Even in the case of failure, the filestream
 * will no longer point to a valid object
 */

bool MatlabIO::close(void) {
  // close the file and release any associated objects
  _fid.close();
  return !_fid.fail();
}

/*! @brief product of the elements of a vector
 *
 * The function is useful for calculating the total number
 * of elements in an array given a vector of dims.
 * @param vec the input vector
 * @return the product of elements in the input
 */

template<typename T>
T product(const QVector<T>& vec) {
  T acc = 1;
  for (int n = 0; n < vec.size(); ++n)
    acc *= vec[n];
  return acc;
}

/*! @brief convert the type of a variable
 *
 * Given a vector of type char, interpret the data as an vector of
 * type T1, and convert it to a vector of type T2.
 *
 * @param in the input char vector
 * @return the same data, reinterpreted as type T2 through
 * storage type T1
 */

template<class T1, class T2>
QVector<T2> convertPrimitiveType(QByteArray in) {
  // firstly reinterpret the input as type T1
  const unsigned int T1_size =
      static_cast<const unsigned int>(in.size() / sizeof(T1));
  const T1* in_ptr = reinterpret_cast<const T1*>(in.data());

  // construct the new vector
  QVector<T2> out;
  for (unsigned int i = 0U; i < T1_size; ++i) {
    out.push_back(static_cast<T2>(in_ptr[i]));
  }
  
  return out;
}

/*! @brief get the .mat file header information
 *
 * The fields read are:
 *
 * header_  : the matlab header as a human readable string,
 * subsys_  : subsystem specific information,
 * _version : the .mat file version (5 or 73),
 * endian_  : the bye ordering of the .mat file.
 *
 * If the byte ordering needs reversal, this is automatically handled by esfstream.
 */

void MatlabIO::readHeader(void) {

  // get the header information from the Mat file
  for (unsigned int n = 0; n < HEADER_LENGTH+1; ++n) header_[n] = '\0';
  for (unsigned int n = 0; n < SUBSYS_LENGTH+1; ++n) subsys_[n] = '\0';
  for (unsigned int n = 0; n < ENDIAN_LENGTH+1; ++n) endian_[n] = '\0';
  _fid.read(header_, sizeof(char)*HEADER_LENGTH);
  _fid.read(subsys_, sizeof(char)*SUBSYS_LENGTH);
  _fid.read((char *)&_version, sizeof(int16_t));
  _fid.read(endian_, sizeof(char)*ENDIAN_LENGTH);

  // get the actual version
  if (_version == 0x0100) _version = VERSION_5;
  if (_version == 0x0200) _version = VERSION_73;

  // get the endianess
  if (strcmp(endian_, "IM") == 0) _byte_swap = false;
  if (strcmp(endian_, "MI") == 0) _byte_swap = true;
  // turn on byte swapping if necessary
  _fid.setByteSwap(_byte_swap);

  //printf("Header: %s\nSubsys: %s\nVersion: %d\nEndian: %s\nByte
  // Swap: %d\n", header_, subsys_, version_, endian_, byte_swap_);

  _bytes_read = 128;
}

/*! @brief interpret the variable header information
 *
 * Given a binary data blob, determine the data type and number of
 * bytes that constitute the data blob. This internally handles
 * whether the header is in long or short format.
 *
 * @param data_type the returned data type
 * @param dbytes the returned number of bytes that constitute the data blob
 * @param wbytes the whole number of bytes that include the header size,
 * the size of the data and any padding to 64-bit boundaries. This is equivalent
 * to the entire number of bytes effectively used by a variable
 * @param data the input binary blob
 * @return a pointer to the beginning of the data segment of the binary blob
 */

const char * MatlabIO::readVariableTag
(uint32_t &data_type, uint32_t &dbytes, uint32_t &wbytes, const char *buff8) {    
  bool small = false;
  const uint32_t *buff32 = reinterpret_cast<const uint32_t *>(buff8);
  data_type = buff32[0];

  if ((data_type >> 16) != 0) {
    // small data format
    dbytes = data_type >> 16;
    data_type = (data_type << 16) >> 16;
    small = true;
  } else {
    // regular format
    dbytes = buff32[1];
  }

  // get the whole number of bytes (wbytes) consumed by this variable,
  // including header and padding
  if (small) wbytes = 8;
  else if (data_type == MAT_COMPRESSED) wbytes = 8 + dbytes;
  else wbytes = 8 + dbytes + ((8-dbytes) % 8);

  // return the seek head positioned over the data payload
  return buff8 + (small ? 4 : 8);
}

/*! @brief construct: return "construct";
 *
 * TODO: implement this
 * @param dims
 * @param real
 * @return
 */

QVariant MatlabIO::constructStruct(QVector<uint32_t>& dims, QByteArray real) {

  // get the length of each field
  uint32_t length_type, length_dbytes, length_wbytes;
  uint32_t length =
    *reinterpret_cast<const uint32_t*>
    (readVariableTag(length_type, length_dbytes, length_wbytes, real.data()));

  // get the total number of fields
  uint32_t nfields_type, nfields_dbytes, nfields_wbytes;
  const char* nfields_ptr =
    readVariableTag(nfields_type, nfields_dbytes, nfields_wbytes,
                    real.data()+length_wbytes);
  assert((nfields_dbytes % length) == 0);
  uint32_t nfields = nfields_dbytes / length;

  // populate a vector of field names
  QStringList field_names;
  for (unsigned int n = 0; n < nfields; ++n) {
    field_names.push_back(nfields_ptr+(n*length));
  }

  // iterate through each of the cells and construct the matrices
  QVector<QMap<QString,QVariant>> array;
  const char* field_ptr = real.data()+length_wbytes+nfields_wbytes;
  for (unsigned int m = 0; m < product<uint32_t>(dims); ++m) {
    QMap<QString,QVariant> strct;
    for (unsigned int n = 0; n < nfields; ++n) {
      uint32_t data_type, dbytes, wbytes;
      const char* data_ptr =
        readVariableTag(data_type, dbytes, wbytes, field_ptr);
      assert(data_type == MAT_MATRIX);
      
      QString varName;
      QVariant varData;
      if (collateMatrixFields(data_type, /*dbytes,*/
                              QByteArray(data_ptr, dbytes), varName, varData)) {
        strct.insert(field_names[n], varData);
      }
      field_ptr += wbytes;
    }
    array.push_back(strct);
  }
  return QVariant::fromValue(array);
}

/*! @brief construct a cell array
 *
 * If the variable is of type MAT_CELL, construct a cell array. This
 * is done by iteratively calling collateMatrixFields() on each
 * element of the cell, and storing the result in a
 * vector<MatlabIOContainer>.  Cell fields may not have a name, but
 * are still required to have a name tag. In this case, placeholder
 * names are substituted. The dimensionality of the cell array is
 * ignored, and the size is linearized in column major format.
 *
 * @param dims the dimesionality of the cell array (ignored)
 * @param real the real part
 * @return the wrapped cell array
 */

QVariant MatlabIO::constructCell(QVector<uint32_t>& dims, QByteArray real) {

  QVector<QVariant> cell;
  const char* field_ptr = real.data();
  for (unsigned int n = 0; n < product<uint32_t>(dims); ++n) {
    uint32_t data_type, dbytes, wbytes;
    const char* data_ptr = readVariableTag(data_type, dbytes, wbytes, field_ptr);
    //printf("cell data_type: %d,  dbytes: %d\n", data_type, dbytes);
    assert(data_type == MAT_MATRIX);
    
    QString varName;
    QVariant varData;
    if (collateMatrixFields(data_type, /*dbytes,*/
                            QByteArray(data_ptr, dbytes), varName, varData)) {
      cell.push_back(varData);
    }
    field_ptr += wbytes;
  }
  return QVariant::fromValue(cell);
}

/*! @brief construct a matrix from an extracted set of fields
 *
 * Given the variable size, name, data and data type, construct a
 * matrix.  Note that Matlab may store variables in a different data
 * type to the actual variable data type (T) to save space. For
 * example matrix a = [1 2 3 4 5]; in Matlab will intrinsically be of
 * type double (everything is unless otherwise explicitly stated) but
 * could be stored as a uint8_t to save space.  The type of the
 * variable returned should necessarily be double, since it's
 * impossible to know at compile time which data types Matlab has
 * decided to store a set of variables in.
 *
 * @param dims the variable dimensionality (i, j, k, ...)
 * @param real the real part
 * @param imag the imaginary part (imag.size() == 0 if the data is real)
 * @param stor_type the storage type of the value
 * @return the wrapped matrix
 */
template<class T>
QVariant MatlabIO::constructMatrix
(QVector<uint32_t>& dims, QByteArray real, QByteArray imag, uint32_t stor_type) {

  QVector<T> vec_real;
  QVector<T> vec_imag;
  switch (stor_type) {
  case MAT_INT8:
    vec_real = convertPrimitiveType<int8_t, T>(real);
    vec_imag = convertPrimitiveType<int8_t, T>(imag);
    break;
  case MAT_UINT8:
    vec_real = convertPrimitiveType<uint8_t, T>(real);
    vec_imag = convertPrimitiveType<uint8_t, T>(imag);
    break;
  case MAT_INT16:
    vec_real = convertPrimitiveType<int16_t, T>(real);
    vec_imag = convertPrimitiveType<int16_t, T>(imag);
    break;
  case MAT_UINT16:
    vec_real = convertPrimitiveType<uint16_t, T>(real);
    vec_imag = convertPrimitiveType<uint16_t, T>(imag);
    break;
  case MAT_INT32:
    vec_real = convertPrimitiveType<int32_t, T>(real);
    vec_imag = convertPrimitiveType<int32_t, T>(imag);
    break;
  case MAT_UINT32:
    vec_real = convertPrimitiveType<uint32_t, T>(real);
    vec_imag = convertPrimitiveType<uint32_t, T>(imag);
    break;
  case MAT_INT64:
    vec_real = convertPrimitiveType<int64_t, T>(real);
    vec_imag = convertPrimitiveType<int64_t, T>(imag);
    break;
  case MAT_UINT64:
    vec_real = convertPrimitiveType<uint64_t, T>(real);
    vec_imag = convertPrimitiveType<uint64_t, T>(imag);
    break;

  case MAT_FLOAT:
    vec_real = convertPrimitiveType<float, T>(real);
    vec_imag = convertPrimitiveType<float, T>(imag);
    break;
  case MAT_DOUBLE:
    vec_real = convertPrimitiveType<double, T>(real);
    vec_imag = convertPrimitiveType<double, T>(imag);
    break;
  case MAT_UTF8:
    vec_real = convertPrimitiveType<char, T>(real);
    vec_imag = convertPrimitiveType<char, T>(imag);
    break;
  default:
    return QVariant();
  }

  // assert that the conversion has not modified the number of elements
  int numel = 1;
  for (int n = 0; n < dims.size(); ++n) numel *= dims[n];
  assert(vec_real.size() == numel);

  // if the data is a scalar, don't write it to a matrix
  if (vec_real.size() == 1 && vec_imag.size() == 0) {
    return QVariant::fromValue(vec_real.first());
  }

  // get the number of channels
  const unsigned int channels = dims.size() == 3 ? dims[2] : 1;
  //bool complx = vec_imag.size() != 0;

  // put each plane of the image into a vector
  QVector<QVector<QVector<T>>> mat;
  mat.resize(channels);
  int index = 0;
  for (unsigned int n = 0; n < channels; ++n) {
    auto & channel = mat[n];
    channel.resize(dims[1]);
    for (unsigned int w = 0; w < dims[1]; ++w) {
      auto & col = channel[w];
      col.resize(dims[0]);
      for (unsigned int h = 0; h < dims[0]; ++h) {
        col[h] = vec_real[index++];
      }
    }
  }
  
  return QVariant::fromValue(mat); 
}

/*! @brief interpret all fields of a matrix
 *
 * collateMatrixFields takes a binary blob of data and strips out the
 * matrix fields.  These fields necessarily include: the variable
 * dimensionality, the variable name and the real part of the variable
 * data. It optionally includes the imaginary part of the variable
 * data if that exists too. The extracted fields are used to either
 * construct a matrix, cell array or struct, or a scalar in the case
 * where the variable dimensionality is (1,1)
 *
 * @param data_type the type of the data stored in the binary blob
 * @param nbytes the number of bytes that constitute the binary blob
 * @param data the binary blob
 *
 * @return the variable (matrix, struct, cell, scalar) wrapped in a container
 */

bool MatlabIO::collateMatrixFields
(uint32_t data_type, /* uint32_t data_bytes,*/ QByteArray data,
 QString & varName, QVariant & varData) {
  // assert(data_bytes==data.size());

  if(data_type!=MAT_MATRIX) return false;

  bool success = true;

  printf("collateMatrixFields() {\n");

  printf("  data_type   = %s\n",tagType(data_type));
  printf("  data_size   = %llu\n",data.size());

  //////////////////////////////////////////////////////////////////////

  // get the flags
  bool complx  = data[9] & (1 << 3);
  //bool logical = data[9] & (1 << 1);

  printf("  complex     = %s\n",(complx)?"true":"false");
  
  //////////////////////////////////////////////////////////////////////

  // get the type of the encapsulated data
  char enc_data_type = data[8];
  printf("  data_type   = %s\n",matType(enc_data_type));

  // the preamble size is 16 bytes
  uint32_t pre_wbytes = 16;

  printf("  pre_wbytes  = %d\n",pre_wbytes);

  printf("       [i] 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");
  printf("  preamble");
  for(int i=0;i<16;i++)
    printf(" %02x",static_cast<uint>(data[i]));
  printf("\n");;

  //////////////////////////////////////////////////////////////////////
  
  // get the dimensions
  uint32_t dim_type, dim_dbytes, dim_wbytes;

  const uint32_t * dim_data =
    reinterpret_cast<const uint32_t *>
    (readVariableTag(dim_type, dim_dbytes, dim_wbytes, data.data() + pre_wbytes));

  QVector<uint32_t> dims;
  int nDims = dim_dbytes/sizeof(uint32_t);
  for (int i=0; i<nDims; ++i) {
    dims.push_back(dim_data[i]);
  }
  
  printf("  dim_type    = %s\n",tagType(dim_type));
  printf("  dim_dbytes  = %d\n",dim_dbytes);
  printf("  dim_wbytes  = %d\n",dim_wbytes);
  printf("  dims[%d]     = [",nDims);
  for (int i=0; i<nDims; ++i)
    printf(" %d",dims[i]);
  printf(" ]\n");

  //////////////////////////////////////////////////////////////////////

  // get the variable name
  uint32_t name_type, name_dbytes, name_wbytes;

  const char* name_data =
    readVariableTag(name_type, name_dbytes, name_wbytes,
                    data.data() + pre_wbytes + dim_wbytes);

  varName = QByteArray(name_data, name_dbytes);

  printf("  name_type   = %s\n",tagType(name_type));
  printf("  name_dbytes = %d\n",name_dbytes);
  printf("  name_wbytes = %d\n",name_wbytes);
  printf("  name        = \"%s\"\n",name_data);

  //////////////////////////////////////////////////////////////////////

  if (enc_data_type == MAT_CELL_CLASS) {

    QByteArray real(data.data()+pre_wbytes+dim_wbytes+name_wbytes,
                    data.size()-pre_wbytes+dim_wbytes+name_wbytes);

    varData = constructCell(dims, real);

    printf("  cell_size   = %llu\n",real.size());

  } else if (enc_data_type == MAT_STRUCT_CLASS) {

    QByteArray real(data.data()+pre_wbytes+dim_wbytes+name_wbytes,
                    data.size()-pre_wbytes+dim_wbytes+name_wbytes);

    varData = constructStruct(dims, real);

    printf("  struct_size = %llu\n",real.size());
    
  } else {

    uint32_t real_type, real_dbytes, real_wbytes;

    const char* real_data =
      readVariableTag
      (real_type, real_dbytes, real_wbytes,
       data.data() + pre_wbytes + dim_wbytes+name_wbytes);

    printf("  real_type   = %s\n",tagType(real_type));
    printf("  real_dbytes = %d\n",real_dbytes);
    printf("  real_wbytes = %d\n",real_wbytes);

    QByteArray real(real_data, real_dbytes);

    printf("  real_size   = %llu\n",real.size());

    QByteArray imag;

    if (complx) {
      // get the imaginery data
      uint32_t imag_type, imag_dbytes, imag_wbytes;

      const char* imag_data =
        readVariableTag(imag_type, imag_dbytes, imag_wbytes,
                        data.data() + pre_wbytes + dim_wbytes +
                        name_wbytes + real_wbytes);

      printf("  imag_type   = %s\n",tagType(imag_type));
      printf("  imag_dbytes = %d\n",imag_dbytes);
      printf("  imag_wbytes = %d\n",imag_wbytes);
      
      assert(imag_type == real_type);
      for ( ; imag_data != imag_data+imag_dbytes; imag_data++)
        imag.push_back(*imag_data);
    }

    printf("  imag_size   = %llu\n",imag.size());

    // construct whatever object we happened to get
    switch (enc_data_type) {
    case MAT_INT8_CLASS:
    case MAT_UINT8_CLASS:
    case MAT_INT16_CLASS:
    case MAT_UINT16_CLASS:
    case MAT_INT32_CLASS:
    case MAT_UINT32_CLASS:
    case MAT_FLOAT_CLASS:
    case MAT_DOUBLE_CLASS:
    case MAT_INT64_CLASS:
    case MAT_UINT64_CLASS:
      varData = constructMatrix<uint64_t>(dims, real, imag, real_type); break;
    case MAT_CHAR_CLASS:
      varData = QString(real); break;
 // non-handled types
    case MAT_SPARSE_CLASS:
    case MAT_OBJECT_CLASS:
    default:
      success = false;
    }
  }

  printf("  varName      = \"%s\"\n",varName.toStdString().c_str());
  printf("  varData_type = \"%s\"\n",varData.typeName());
  
  printf("}\n");
  
  return success;
}

/*! @brief uncompress a variable
 *
 * If the data type of a variable is MAT_COMPRESSED, then the binary
 * data blob has been compressed using zlib compression. This function
 * uncompresses the blob, then calls readVariable() to interpret the
 * actual data
 *
 * @param data_type the type of the data stored in the binary blob
 * @param dbytes the number of bytes that constitue the binary blob
 * @param wbytes the whole number of bytes that consistute the header,
 * the binary blob, and any padding to 64-bit boundaries
 * @param data the binary blob
 * @return the binary blob, uncompressed
 */

QByteArray MatlabIO::uncompressVariable
(uint32_t& data_type, uint32_t& dbytes, uint32_t& wbytes, QByteArray data)  {

  // inflate the variable header
  QByteArray buf = qUncompress(data);

  // get the headers
  readVariableTag(data_type, dbytes, wbytes, buf);
  
  // convert to a vector
  assert(buf.size() == static_cast<int>(dbytes) + 8);
  QByteArray udata(buf.data() + 8, dbytes);

  return udata;
}

/*! @brief Interpret a variable from a binary block of data
 *
 * This function may be called recursively when either uncompressing
 * data or interpreting fields of a struct or cell array
 *
 * @param data_type the type of the data stored in the binary blob
 * @param nbytes the number of bytes that constitute the binary blob
 * @param data the binary blob @return an interpreted variable
 */

bool MatlabIO::readVariable
(uint32_t data_type, uint32_t nbytes, QByteArray data,
 QString & varName, QVariant & varData) {

  switch (data_type) {
    case MAT_COMPRESSED:
      { // uncompress the data
        uint32_t udata_type;
        uint32_t udbytes;
        uint32_t uwbytes;
        uchar dSize[4] = {static_cast<uchar>(0xff & (nbytes >> 24)),
                          static_cast<uchar>(0xff & (nbytes >> 16)),
                          static_cast<uchar>(0xff & (nbytes >>  8)),
                          static_cast<uchar>(0xff & (nbytes >>  0))};
        QByteArray zData = QByteArray(reinterpret_cast<char*>(dSize), 4) + data;
        QByteArray udata = uncompressVariable(udata_type, udbytes, uwbytes, zData);
        return readVariable(udata_type, udbytes, udata, varName, varData);
      }
    case MAT_MATRIX:
      { // deserialize the matrix
        return collateMatrixFields(data_type, /*nbytes,*/ data, varName, varData);
      }
    default: break;
    }

  return false;
}

/*! @brief read a block of data from the file being parsed
 *
 * This function attempts to read an entire variable from the file
 * being parsed.  The data block is then encapsulated in a vector and
 * passed onto readVariable() for interpretation. This design means
 * that the file is touched a minimal number of times, and later
 * manipulation of the data can make use of automatic memory
 * management, reference counting, etc.
 *
 * @return the block of data interpreted as a variable and stored in a
 * generic container
 */

bool MatlabIO::readBlock(QString & varName, QVariant & varData) {

  // get the data type and number of bytes consumed
  // by this variable. Check to see if it's using
  // the small data format (seriously, who thought of that? You save
  // at best 8 bytes...)

  char buf[8];
  _fid.read(buf, sizeof(char)*8);
  uint32_t data_type;
  uint32_t dbytes;
  uint32_t wbytes;
  readVariableTag(data_type, dbytes, wbytes, buf);

  // read the binary data block
  //printf("\nReading binary data block...\n"); fflush(stdout);
  QByteArray data(dbytes, 0);
  _fid.read(data.data(), sizeof(char)*data.size());

  // move the seek head position to the next 64-bit boundary
  // (but only if the data is uncompressed. Saving yet another 8 tiny bytes...)
  if (data_type != MAT_COMPRESSED) {
    //printf("Aligning seek head to next 64-bit boundary...\n");
    std::streampos head_pos = _fid.tellg();
    int padding = head_pos % 8;
    _fid.seekg(padding, std::fstream::cur); // relative 
  }

  // now read the variable contained in the block
  return readVariable(data_type, dbytes, data, varName, varData);
}

/*! @brief Read all variables from a file
 *
 * Reads every variable encountered when parsing a valid Matlab .mat file.
 * If any of the variables is a function pointer, or other Matlab specific
 * object, it will be passed. Most integral types will be parsed successfully.
 * Matlab matrices will be converted to OpenCV matrices of the same type.
 * Note: Matlab stores images in RGB format whereas OpenCV stores images in
 * BGR format, so if displaying a parsed image using cv::imshow(), the
 * colours will be inverted.
 * @return a vector of containers storing the name and data of each variable
 * in the file
 */

QMap<QString,QVariant> MatlabIO::read(void) {
  printf("MatlabIO::read(void) {\n");
  // uint32_t pos = _fid.tellp();          // .tellg();
  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));

  // allocate the output
  QMap<QString,QVariant> varMap;

  printf("  reading header ...\n");

  // read the header information
  readHeader();

  // get all of the variables
  while(hasMoreVariables()) {

    printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));
    printf("  reading block ...\n");

    QString  varName;
    QVariant varData;
    if (readBlock(varName, varData)==false) {
      printf("  ERROR : reading block varName=\"%s\"\n",
             varName.toStdString().c_str()); 
      continue;
    }
    varMap.insert(varName, varData);
  }

  // if(debug)
  // whos(varMap);

  printf("  pos = %d\n", static_cast<uint32_t>(_fid.tellp()));
  printf("}\n");

  return varMap;
}

/*! @brief Print a formatted list of the contents of a file
 *
 * Similar to the 'whos' function in matlab, this function prints to stdout
 * a list of variables and their C++ datatypes stored in the associated .mat file
 * @param variables the variables read from the .mat file using the read() function
 */

void MatlabIO::whos(QMap<QString,QVariant> const& varMap) const {

  // get the longest filename for printf format string
  int flmax = 0;
  auto nameList = varMap.keys();
  foreach(auto name, nameList) { 
    if (name.length() > flmax) {
      flmax = name.length();
    }
  }
  
  printf("  varMap {\n");
  printf("    file   : %s\n", qPrintable(_filename));
  printf("    header : \"%s\"\n", header_);
  printf("    subsys : \"%s\"\n", subsys_);
  printf("    endian : \"%s\"\n", endian_);
  printf("    vars {\n");
  foreach(auto name, nameList) {

    int type = varMap[name].typeId();
    QString typeStr = "";
    switch(type) {
    case QMetaType::Void: typeStr = "Void"; break;
    case QMetaType::Bool: typeStr = "Bool"; break;
    case QMetaType::Int: typeStr = "Int"; break;
    case QMetaType::UInt: typeStr = "UInt"; break;
    case QMetaType::Double: typeStr = "Double"; break;
    case QMetaType::QChar: typeStr = "QChar"; break;
    case QMetaType::QString: typeStr = "QString"; break;
    case QMetaType::QByteArray: typeStr = "QByteArray"; break;
    case QMetaType::Nullptr: typeStr = "Nullptr"; break;
    case QMetaType::VoidStar: typeStr = "VoidStar"; break;
    case QMetaType::Long: typeStr = "Long"; break;
    case QMetaType::LongLong: typeStr = "LongLong"; break;
    case QMetaType::Short: typeStr = "Short"; break;
    case QMetaType::Char: typeStr = "Char"; break;
    case QMetaType::Char16: typeStr = "Char16"; break;
    case QMetaType::Char32: typeStr = "Char32"; break;
    case QMetaType::ULong: typeStr = "ULong"; break;
    case QMetaType::ULongLong: typeStr = "ULongLong"; break;
    case QMetaType::UShort: typeStr = "UShort"; break;
    case QMetaType::SChar: typeStr = "SChar"; break;
    case QMetaType::UChar: typeStr = "UChar"; break;
    case QMetaType::Float: typeStr = "Float"; break;
    case QMetaType::Float16: typeStr = "Float16"; break;
    case QMetaType::QObjectStar: typeStr = "QObjectStar"; break;
    case QMetaType::QCursor: typeStr = "QCursor"; break;
    case QMetaType::QDate: typeStr = "QDate"; break;
    case QMetaType::QSize: typeStr = "QSize"; break;
    case QMetaType::QTime: typeStr = "QTime"; break;
    case QMetaType::QVariantList: typeStr = "QVariantList"; break;
    case QMetaType::QPolygon: typeStr = "QPolygon"; break;
    case QMetaType::QPolygonF: typeStr = "QPolygonF"; break;
    case QMetaType::QColor: typeStr = "QColor"; break;
    case QMetaType::QColorSpace: typeStr = "QColorSpace"; break;
    case QMetaType::QSizeF: typeStr = "QSizeF"; break;
    case QMetaType::QRectF: typeStr = "QRectF"; break;
    case QMetaType::QLine: typeStr = "QLine"; break;
    case QMetaType::QTextLength: typeStr = "QTextLength"; break;
    case QMetaType::QStringList: typeStr = "QStringList"; break;
    case QMetaType::QVariantMap: typeStr = "QVariantMap"; break;
    case QMetaType::QVariantHash: typeStr = "QVariantHash"; break;
    case QMetaType::QVariantPair: typeStr = "QVariantPair"; break;
    case QMetaType::QIcon: typeStr = "QIcon"; break;
    case QMetaType::QPen: typeStr = "QPen"; break;
    case QMetaType::QLineF: typeStr = "QLineF"; break;
    case QMetaType::QTextFormat: typeStr = "QTextFormat"; break;
    case QMetaType::QRect: typeStr = "QRect"; break;
    case QMetaType::QPoint: typeStr = "QPoint"; break;
    case QMetaType::QUrl: typeStr = "QUrl"; break;
    case QMetaType::QRegularExpression: typeStr = "QRegularExpression"; break;
    case QMetaType::QDateTime: typeStr = "QDateTime"; break;
    case QMetaType::QPointF: typeStr = "QPointF"; break;
    case QMetaType::QPalette: typeStr = "QPalette"; break;
    case QMetaType::QFont: typeStr = "QFont"; break;
    case QMetaType::QBrush: typeStr = "QBrush"; break;
    case QMetaType::QRegion: typeStr = "QRegion"; break;
    case QMetaType::QBitArray: typeStr = "QBitArray"; break;
    case QMetaType::QImage: typeStr = "QImage"; break;
    case QMetaType::QKeySequence: typeStr = "QKeySequence"; break;
    case QMetaType::QSizePolicy: typeStr = "QSizePolicy"; break;
    case QMetaType::QPixmap: typeStr = "QPixmap"; break;
    case QMetaType::QLocale: typeStr = "QLocale"; break;
    case QMetaType::QBitmap: typeStr = "QBitmap"; break;
    case QMetaType::QTransform: typeStr = "QTransform"; break;
    case QMetaType::QMatrix4x4: typeStr = "QMatrix4x4"; break;
    case QMetaType::QVector2D: typeStr = "QVector2D"; break;
    case QMetaType::QVector3D: typeStr = "QVector3D"; break;
    case QMetaType::QVector4D: typeStr = "QVector4D"; break;
    case QMetaType::QQuaternion: typeStr = "QQuaternion"; break;
    case QMetaType::QEasingCurve: typeStr = "QEasingCurve"; break;
    case QMetaType::QJsonValue: typeStr = "QJsonValue"; break;
    case QMetaType::QJsonObject: typeStr = "QJsonObject"; break;
    case QMetaType::QJsonArray: typeStr = "QJsonArray"; break;
    case QMetaType::QJsonDocument: typeStr = "QJsonDocument"; break;
    case QMetaType::QCborValue: typeStr = "QCborValue"; break;
    case QMetaType::QCborArray: typeStr = "QCborArray"; break;
    case QMetaType::QCborMap: typeStr = "QCborMap"; break;
    case QMetaType::QCborSimpleType: typeStr = "QCborSimpleType"; break;
    case QMetaType::QModelIndex: typeStr = "QModelIndex"; break;
    case QMetaType::QPersistentModelIndex: typeStr = "QPersistentModelIndex"; break;
    case QMetaType::QUuid: typeStr = "QUuid"; break;
    case QMetaType::QByteArrayList: typeStr = "QByteArrayList"; break;
    case QMetaType::QVariant: typeStr = "QVariant"; break;
    case QMetaType::User: typeStr = "User"; break;
    case QMetaType::UnknownType:
    default:
      typeStr = "UnknownType"; break;
    }

    printf("  %*s:  %s\n",
           static_cast<int>(flmax), qPrintable(name),
           qPrintable(typeStr));
  }
  printf("    }\n");
  printf("  }\n");
  fflush(stdout);
}

//////////////////////////////////////////////////////////////////////
// GT Fri Apr  2 21:26:45 2021

void MatlabIO::writeHeader(void) {
  printf("  writeHeader() {\n");
  printf("    pos = %d\n",static_cast<uint32_t>(_fid.tellp()));

  // uint32_t written_bytes = 0;

  _fid.write(header_, sizeof(char)*HEADER_LENGTH);
  // written_bytes += sizeof(char)*HEADER_LENGTH;

  _fid.write(subsys_, sizeof(char)*SUBSYS_LENGTH);
  // written_bytes += sizeof(char)*SUBSYS_LENGTH;

  int16_t version = 0x0;
  if(_version==VERSION_5 ) version = 0x0100;
  if(_version==VERSION_73) version = 0x0200;
  _fid.write((char *)&version, sizeof(int16_t));
  // written_bytes += sizeof(int16_t);

  // char endian[ENDIAN_LENGTH+1];
  QByteArray endianArray(ENDIAN_LENGTH+1,0x0);
  char* endian = endianArray.data();
  if(_byte_swap)
    snprintf(endian,ENDIAN_LENGTH+1,"MI");
  else
    snprintf(endian,ENDIAN_LENGTH+1,"IM");
  _fid.write(endian, sizeof(char)*ENDIAN_LENGTH);
  // written_bytes += sizeof(char)*ENDIAN_LENGTH;

  // printf("    written_bytes = %d\n",written_bytes);
  printf("    pos = %d\n",static_cast<uint32_t>(_fid.tellp()));
  printf("  }\n");
}

//////////////////////////////////////////////////////////////////////
bool MatlabIO::writeString(const QString & name, const QString & value ) {

  printf("writeString(\"%s\",\"%s\") {\n",
         name.toStdString().c_str(),value.toStdString().c_str());
  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));

  // collateMatrixFields() {
  //   data_type   = MAT_MATRIX
  //   data_size   = 64
  //   complex     = false
  //   data_type   = MAT_CHAR_CLASS
  //   pre_wbytes  = 16
  //        [i] 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
  //   preamble 06 00 00 00 08 00 00 00 04 00 00 00 00 00 00 00
  //   dim_type    = MAT_INT32
  //   dim_dbytes  = 8
  //   dim_wbytes  = 16
  //   dims[2]     = [ 1 3 ]
  //   name_type   = MAT_INT8
  //   name_dbytes = 12
  //   name_wbytes = 24
  //   name        = "format_image"
  //   real_type   = MAT_UTF8
  //   real_dbytes = 3
  //   real_wbytes = 8
  //   real_size   = 3
  //   imag_size   = 0
  //   varName      = "format_image"
  //   varData_type = "QString"
  // }

  int cols = value.length();
  int rows = 1;
  // matlab::MatlabMatrixType type = matlab::RowMajor;

  // writeBlock()
  
  uint32_t pre_wbytes    = 16;
  uint32_t dim_wbytes    = 8+8;
  uint32_t name_length   = name.length();
  uint32_t name_padding  = (name_length%8==0)?0:(8-name_length%8);
  uint32_t name_wbytes   = 8+name_length+name_padding;
  uint32_t value_length  = value.length();
  uint32_t data_padding  = (value_length%8==0)?0:(8-value_length%8);
  uint32_t data_wbytes   = 8+value_length+data_padding;
  uint32_t block_dbytes  = pre_wbytes+dim_wbytes+name_wbytes+data_wbytes;
  uint32_t block_wbytes  = block_dbytes;

  char buff8[16];
  uint32_t written_bytes = 0;

  printf("  writing block tag ...\n");

  // write block tag
  uint32_t *buff32 = reinterpret_cast<uint32_t *>(buff8);
  buff32[0] = MAT_MATRIX;
  buff32[1] = block_wbytes;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing preamble ...\n");

  static char preamble[16] = {
    0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  _fid.write(preamble,16);
  written_bytes += 16;

  printf("  writing dimensions block tag+bytes ...\n");

  buff32[0] = MAT_INT32;
  buff32[1] = 8;
  buff32[2] = rows; // dim[0]
  buff32[3] = cols; // dim[1]

  _fid.write(buff8,16);
  written_bytes += 16;

  printf("  writing name block tag ...\n");

  buff32[0] = MAT_INT8;
  buff32[1] = name_length;
  _fid.write(buff8,8);
  written_bytes += 8;
  
  printf("  writing name block bytes ...\n");

  QByteArray name_array;
  name_array.append(name.toStdString().c_str());
  _fid.write(name_array.data(),name_length);
  written_bytes += name_length;
  
  if(name_padding>0) {
    
    printf("  writing name block padding bytes ...\n");

    QByteArray padding(name_padding,0x0);
    _fid.write(padding.data(),name_padding);
    written_bytes += name_padding;
  }

  printf("  writing data block tag ...\n");

  buff32[0] = MAT_UTF8;
  buff32[1] = value_length;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing data block bytes ...\n");

  QByteArray value_array;
  value_array.append(value.toStdString().c_str());
  _fid.write(value_array.data(),value_length);
  written_bytes += value_length;
  
  if(written_bytes%8!=0) {
    
    printf("  writing data block padding bytes ...\n");

    QByteArray padding(data_padding,0x0);
    _fid.write(padding.data(),data_padding);
    written_bytes += data_padding;
  }
  
  printf("  written_bytes = %d (8*%d+%d)\n",
         written_bytes,written_bytes/8,written_bytes%8);

  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));
  printf("}\n");
  return true;
}

//////////////////////////////////////////////////////////////////////
bool MatlabIO::writeInt(const QString & name, const int32_t & value ) {
  printf("writeInt(\"%s\",%d) {\n", name.toStdString().c_str(),value);
  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));

  // collateMatrixFields() {
  //   data_type   = MAT_MATRIX
  //   data_size   = 56
  //   complex     = false
  //   data_type   = MAT_DOUBLE_CLASS
  //   pre_wbytes  = 16
  //        [i] 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
  //   preamble 06 00 00 00 08 00 00 00 06 00 00 00 00 00 00 00
  //   dim_type    = MAT_INT32
  //   dim_dbytes  = 8
  //   dim_wbytes  = 16
  //   dims[2]     = [ 1 1 ]
  //   name_type   = MAT_INT8
  //   name_dbytes = 5
  //   name_wbytes = 16
  //   name        = "n_ima"
  //   real_type   = MAT_UINT8
  //   real_dbytes = 1
  //   real_wbytes = 8
  //   real_size   = 1
  //   imag_size   = 0
  //   varName      = "n_ima"
  //   varData_type = "qulonglong"
  // }

  uint32_t pre_wbytes   = 16;
  uint32_t dim_wbytes   = 8+8;
  uint32_t name_length  = name.length();
  uint32_t name_padding = (name_length%8==0)?0:(8-name_length%8);
  uint32_t name_wbytes  = 8+name_length+name_padding;
  uint32_t data_wbytes  = 8+8;
  uint32_t block_dbytes = pre_wbytes+dim_wbytes+name_wbytes+data_wbytes;
  uint32_t block_wbytes = block_dbytes;
  
  char buff8[16];
  uint32_t written_bytes = 0;

  printf("  writing block tag ...\n");

  // write block tag
  uint32_t *buff32 = reinterpret_cast<uint32_t *>(buff8);
  buff32[0] = MAT_MATRIX;
  buff32[1] = block_wbytes;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing preamble ...\n");

  // write preamble
  static char preamble[16] = {
    0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  _fid.write(preamble,16);
  written_bytes += 16;

  printf("  writing dimensions block tag+bytes ...\n");

  // write dim block
  buff32[0] = MAT_INT32;
  buff32[1] = 8;
  buff32[2] = 1; // dim[0]
  buff32[3] = 1; // dim[1]
  _fid.write(buff8,16);
  written_bytes += 16;

  printf("  writing name block tag ...\n");

  // write name block
  buff32[0] = MAT_INT8;
  buff32[1] = name_length;
  _fid.write(buff8,8);
  written_bytes += 8;
  
  printf("  writing name block bytes ...\n");

  QByteArray name_array;
  name_array.append(name.toStdString().c_str());
  _fid.write(name_array.data(),name_length);
  written_bytes += name_length;
  
  if(name_padding>0) {
    
    printf("  writing name block padding bytes ...\n");

    QByteArray padding(name_padding,0x0);
    _fid.write(padding.data(),name_padding);
    written_bytes += name_padding;
  }

  printf("  writing data block tag ...\n");

  // write data block
  buff32[0] = MAT_INT32;
  buff32[1] = 4;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing data block bytes ...\n");

  _fid.write(reinterpret_cast<const char*>(&value),4);
  written_bytes += 4;

  if(written_bytes%8!=0) {
    
    printf("  writing data block padding bytes ...\n");

    uint32_t padding_bytes = 8-(written_bytes%8);
    QByteArray padding(padding_bytes,0x0);
    _fid.write(padding.data(),padding_bytes);
    written_bytes += padding_bytes;
  }
  
  printf("  written_bytes = %d (8*%d+%d)\n",
         written_bytes,written_bytes/8,written_bytes%8);

  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));
  printf("}\n");
  return true;
}

//////////////////////////////////////////////////////////////////////
bool MatlabIO::writeDouble(const QString & name, const double & value ) {

  printf("writeInt(\"%s\",%f) {\n",name.toStdString().c_str(),value);
  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));

  // collateMatrixFields() {
  //   data_type   = MAT_MATRIX
  //   data_size   = 56
  //   complex     = false
  //   data_type   = MAT_DOUBLE_CLASS
  //   pre_wbytes  = 16
  //        [i] 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
  //   preamble 06 00 00 00 08 00 00 00 06 00 00 00 00 00 00 00
  //   dim_type    = MAT_INT32
  //   dim_dbytes  = 8
  //   dim_wbytes  = 16
  //   dims[2]     = [ 1 1 ]
  //   name_type   = MAT_INT8
  //   name_dbytes = 2
  //   name_wbytes = 8
  //   name        = "dX"
  //   real_type   = MAT_DOUBLE
  //   real_dbytes = 8
  //   real_wbytes = 16
  //   real_size   = 8
  //   imag_size   = 0
  //   varName      = "dX"
  //   varData_type = "qulonglong"
  // }

  uint32_t pre_wbytes   = 16;
  uint32_t dim_wbytes   = 8+8;
  uint32_t name_length  = name.length();
  uint32_t name_padding = (name_length%8==0)?0:(8-name_length%8);
  uint32_t name_wbytes  = 8+name_length+name_padding;
  uint32_t data_wbytes  = 8+8;
  uint32_t block_dbytes = pre_wbytes+dim_wbytes+name_wbytes+data_wbytes;
  uint32_t block_wbytes = block_dbytes;
  
  char buff8[16];
  uint32_t written_bytes = 0;

  printf("  writing block tag ...\n");

  // write block tag
  uint32_t *buff32 = reinterpret_cast<uint32_t *>(buff8);
  buff32[0] = MAT_MATRIX;
  buff32[1] = block_wbytes;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing preamble ...\n");

  // write preamble
  static char preamble[16] = {
    0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  _fid.write(preamble,16);
  written_bytes += 16;

  printf("  writing dimensions block tag+bytes ...\n");

  // write dim block
  buff32[0] = MAT_INT32;
  buff32[1] = 8;
  buff32[2] = 1; // dim[0]
  buff32[3] = 1; // dim[1]
  _fid.write(buff8,16);
  written_bytes += 16;

  printf("  writing name block tag ...\n");

  // write name block
  buff32[0] = MAT_INT8;
  buff32[1] = name_length;
  _fid.write(buff8,8);
  written_bytes += 8;
  
  printf("  writing name block bytes ...\n");

  QByteArray name_array;
  name_array.append(name.toStdString().c_str());
  _fid.write(name_array.data(),name_length);
  written_bytes += name_length;
  
  if(name_padding>0) {
    
    printf("  writing name block padding bytes ...\n");

    QByteArray padding(name_padding,0x0);
    _fid.write(padding.data(),name_padding);
    written_bytes += name_padding;
  }

  printf("  writing data block tag ...\n");

  // write data block
  buff32[0] = MAT_DOUBLE;
  buff32[1] = 8;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing data block bytes ...\n");

  _fid.write(reinterpret_cast<const char*>(&value),8);
  written_bytes += 8;

  // if(written_bytes%8!=0) {
  //
  //   printf("  writing data block padding bytes ...\n");
  //
  //   uint32_t padding_bytes = 8-(written_bytes%8);
  //   QByteArray padding(padding_bytes,0x0);
  //   _fid.write(padding.data(),padding_bytes);
  //   written_bytes += padding_bytes;
  // }
  
  printf("  written_bytes = %d (8*%d+%d)\n",
         written_bytes,written_bytes/8,written_bytes%8);

  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));
  printf("}\n");
  return true;
}

//////////////////////////////////////////////////////////////////////
bool MatlabIO::writeMatrix
(const QString & name, const MatlabMatrixType & value,
 const matlab::MatrixType type) {

  printf("writeMatrix(\"%s\") {\n", name.toStdString().c_str());
  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));

  if(value.size()!=1) {
    printf("  ERROR : value.size()=%llu != 1\n}\n",value.size());
    return false;
  }

  const QVector<QVector<double> > & mat = value[0];
  int cols = 0;
  int rows = 0;

  if(type==matlab::RowMajor) {
    cols = mat.size();
    if(cols<1) {
      printf("  ERROR : cols=%d < 1\n}\n",cols);
      return false;
    }
    printf("  cols = %d\n",cols);
    rows = mat[0].size();
    if(rows<1) {
      printf("  ERROR : rows=%d < 1\n}\n",rows);
      return false;
    }
    printf("  rows = %d\n",rows);
    for(int col=1;col<cols;col++) {
      if(mat[col].size()!=rows) {
        printf("  ERROR : rows[%d]=%llu != rows[0]=%d\n}\n",col,mat[col].size(),rows);
        return false;
      }
    }
  } else /* if(type==matlab::ColMajor) */ {
    rows = mat.size();
    if(rows<1) {
      printf("  ERROR : rows=%d < 1\n}\n",rows);
      return false;
    }
    printf("  rows = %d\n",rows);
    cols = mat[0].size();
    if(cols<1) {
      printf("  ERROR : cols=%d < 1\n}\n",cols);
      return false;
    }
    printf("  cols = %d\n",cols);
    for(int row=1;row<rows;row++) {
      if(mat[row].size()!=cols) {
        printf("  ERROR : cols[%d]=%llu != cols[0]=%d\n}\n",row,mat[row].size(),cols);
        return false;
      }
    }
  }

  // writeBlock()
  
  uint32_t pre_wbytes   = 16;
  uint32_t dim_wbytes   = 8+8;
  uint32_t name_length  = name.length();
  uint32_t name_padding = (name_length%8==0)?0:(8-name_length%8);
  uint32_t name_wbytes  = 8+name_length+name_padding;
  uint32_t data_wbytes  = 8+rows*cols*8;
  uint32_t block_dbytes = pre_wbytes+dim_wbytes+name_wbytes+data_wbytes;
  uint32_t block_wbytes = block_dbytes;

  char buff8[16];
  uint32_t written_bytes = 0;

  printf("  writing block tag ...\n");

  // write block tag
  uint32_t *buff32 = reinterpret_cast<uint32_t *>(buff8);
  buff32[0] = MAT_MATRIX;
  buff32[1] = block_wbytes;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing preamble ...\n");

  // write preamble
  static char preamble[16] = {
    0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  _fid.write(preamble,16);
  written_bytes += 16;

  printf("  writing dimensions block tag+bytes ...\n");

  // write dim block
  buff32[0] = MAT_INT32;
  buff32[1] = 8;
  if(type==matlab::ColMajor) {
    buff32[2] = cols; // dim[0]
    buff32[3] = rows; // dim[1]
  } else /* if(type==matlab::RowMajor) */ {
    buff32[2] = rows; // dim[0]
    buff32[3] = cols; // dim[1]
  }
  _fid.write(buff8,16);
  written_bytes += 16;

  printf("  writing name block tag ...\n");

  // write name block
  buff32[0] = MAT_INT8;
  buff32[1] = name_length;
  _fid.write(buff8,8);
  written_bytes += 8;
  
  printf("  writing name block bytes ...\n");

  QByteArray name_array;
  name_array.append(name.toStdString().c_str());
  _fid.write(name_array.data(),name_length);
  written_bytes += name_length;
  
  if(name_padding>0) {
    
    printf("  writing name block padding bytes ...\n");

    QByteArray padding(name_padding,0x0);
    _fid.write(padding.data(),name_padding);
    written_bytes += name_padding;
  }

  printf("  writing data block tag ...\n");

  // write data block
  buff32[0] = MAT_DOUBLE;
  buff32[1] = rows*cols*8;
  _fid.write(buff8,8);
  written_bytes += 8;

  printf("  writing data block bytes ...\n");

  if(type==matlab::RowMajor) {
    for(int row=0;row<rows;row++) {
      _fid.write(reinterpret_cast<const char*>(mat[row].data()),cols*8);
      written_bytes += cols*8;
    }
  } else /* if(type==matlab::ColMajor) */ {
    for(int col=0;col<cols;col++) {
      _fid.write(reinterpret_cast<const char*>(mat[col].data()),rows*8);
      written_bytes += rows*8;
    }
  }

  if(written_bytes%8!=0) {
    uint32_t padding_bytes = 8-(written_bytes%8);
    QByteArray padding(padding_bytes,0x0);
    _fid.write(padding.data(),padding_bytes);
    written_bytes += padding_bytes;
  }
  
  printf("  written_bytes = %d (8*%d+%d)\n",
         written_bytes,written_bytes/8,written_bytes%8);

  printf("  pos = %d\n",static_cast<uint32_t>(_fid.tellp()));
  printf("}\n");
  return true;
}

#ifdef WRITE_VAR_MAP
//////////////////////////////////////////////////////////////////////
// GT Fri Apr  2 22:45:41 2021
//
// inverse of
// collateMatrixFields(data_type, /*nbytes,*/ data, varName, varData);

bool
MatlabIO::convertVarToByteArray
(const QString & varName, const QVariant & varData, QByteArray& data) {

  printf("convertVarToByteArray() {\n");
  printf("  varName      = \"%s\"\n",varName.toStdString().c_str());
  printf("  varData_type = \"%s\"\n",varData.typeName());

  bool success = false;

  data.clear();

  // preamble //////////////////////////////////////////////////////////

  uint32_t pre_wbytes  = 16;
  uint8_t preamble[16]= {
    0x06, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08,
    0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  // uint32_t enc_data_type = preamble[8];
  // bool complx = preamble[8]&(1<<3); // == false;
  for(int i=0;i<16;i++)
    data.push_back(preamble[i]);
  
  // dimensions ////////////////////////////////////////////////////////
  QVector<uint32_t> dim;
  // ...
  uint32_t nDims = dim.size();
  uint32_t dim_type    = MAT_INT32;
  uint32_t dim_dbytes  = 4*dim.size();
  uint32_t dim_wbytes  = dim_dybtes+8;
  // data.push_back(dim_type);
  // data.push_back(dim_dbytes);

  // name //////////////////////////////////////////////////////////////
  uint32_t    name_type   =  MAT_INT8;
  uint32_t    name_dbytes =  varName.length();
  uint32_t    name_wbytes =  24;
  std::string name        = varName.toStdString();
  const char* name_data   = name.c_str();

  // data //////////////////////////////////////////////////////////////
  uint32_t dat_wbytes  =  0;
  
  printf("  success = %s\n",(success)?"true":"false");
  printf("}\n");

  return success;
}

//////////////////////////////////////////////////////////////////////
// GT Fri Apr  2 22:17:02 2021

bool
MatlabIO::writeBlock
(const QString & varName, const QVariant & varData) {

  // readBlock()
  //
  // // get the data type and number of bytes consumed
  // // by this variable. Check to see if it's using
  // // the small data format (seriously, who thought of that? You save
  // // at best 8 bytes...)
  //
  // char buf[8];
  // _fid.read(buf, sizeof(char)*8);
  // uint32_t data_type;
  // uint32_t dbytes;
  // uint32_t wbytes;
  // readVariableTag(data_type, dbytes, wbytes, buf);
  //
  // // read the binary data block
  // //printf("\nReading binary data block...\n"); fflush(stdout);
  // QByteArray data(dbytes, 0);
  // _fid.read(data.data(), sizeof(char)*data.size());
  //
  // // move the seek head position to the next 64-bit boundary
  // // (but only if the data is uncompressed. Saving yet another 8 tiny bytes...)
  // if (data_type != MAT_COMPRESSED) {
  //   //printf("Aligning seek head to next 64-bit boundary...\n");
  //   std::streampos head_pos = _fid.tellg();
  //   int padding = head_pos % 8;
  //   _fid.seekg(padding, std::fstream::cur);
  // }
  //
  // // now read the variable contained in the block
  // return readVariable(data_type, dbytes, data, varName, varData);

  QByteArray data;

  if(convertVarToByteArray(varName,varData,data)==false)
    return false;
  
  char      buff8[8];
  uint32_t* buff32 = reinterpret_cast<uint32_t*>(buff8);
  buff32[0] = MAT_MATRIX;  // data_type
  buff32[1] = data.size(); // data_bytes 

  // write buff
  _fid.write(buff8, sizeof(char)*8);
  // write data
  _fid.write(data.data(), sizeof(char)*data.size());

  return true;
}


//////////////////////////////////////////////////////////////////////
// GT Fri Apr 2 13:07:04 2021

bool MatlabIO::write(QMap<QString,QVariant> const& varMap) {
  bool success = false;

  // if(debug)
  whos(varMap);

  writeHeader();

  auto nameList = varMap.keys();
  foreach(QString varName, nameList) {
    const QVariant& varData = varMap[varName];

    bool success = writeBlock(varName, varData);

    // if(success==false) break;
  }
  
  return success;
}

#endif // WRITE_VAR_MAP
