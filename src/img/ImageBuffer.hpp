// Time-stamp: <2021-05-04 20:44:54 taubin>
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

#ifndef __ImageBuffer_hpp__
#define __ImageBuffer_hpp__

// #include <stdlib.h>
// #include <cstdint>
#include <cstdlib>
#include <memory>

namespace img  {
  inline void NoDelete(void *) {
    //do nothing
  }
}

class ImageBuffer {
  
public:

  int                      rows;
  int                      cols;
  int                      format;
  uint8_t*                 data;
  std::shared_ptr<uint8_t> sharedData;
  int                      bytes;
  int                      stride; //TODO

  enum FormatType {
    UnknownFormat=0, 
    YU12, // "Planar YUV 4:2:0"
    YUYV, // "YUYV 4:2:2"
    RGB3, // "24-bit RGB 8-8-8" // RGB24,
    JPEG, // "JFIF JPEG"
    H264, // "H.264"
    MJPG, // "Motion-JPEG"
    YVYU, // "YVYU 4:2:2" // YUV422 (deprecated)
    VYUY, // "VYUY 4:2:2"
    UYVY, // "UYVY 4:2:2"
    BGR3, // "24-bit BGR 8-8-8"
    YV12, // "Planar YVU 4:2:0"
    NV12, // "Y/CbCr 4:2:0"
    NV21, // "Y/CrCb 4:2:0"
    RX24, // "32-bit XBGR 8-8-8-8" // BGR32,
    BA24, // "32-bit XRGB 8-8-8-8" // RGB32,
    FormatTypeCount
  };

  static FormatType formatType(const char * fmt4) {
         if(strncmp(fmt4,"YU12",4)==0) return YU12;
    else if(strncmp(fmt4,"YUYV",4)==0) return YUYV;
    else if(strncmp(fmt4,"RGB3",4)==0) return RGB3;
    else if(strncmp(fmt4,"JPEG",4)==0) return JPEG;
    else if(strncmp(fmt4,"H264",4)==0) return H264;
    else if(strncmp(fmt4,"MJPG",4)==0) return MJPG;
    else if(strncmp(fmt4,"YVYU",4)==0) return YVYU;
    else if(strncmp(fmt4,"VYUY",4)==0) return VYUY;
    else if(strncmp(fmt4,"UYVY",4)==0) return UYVY;
    else if(strncmp(fmt4,"BGR3",4)==0) return BGR3;
    else if(strncmp(fmt4,"YV12",4)==0) return YV12;
    else if(strncmp(fmt4,"NV12",4)==0) return NV12;
    else if(strncmp(fmt4,"NV21",4)==0) return NV21;
    else if(strncmp(fmt4,"RX24",4)==0) return RX24;
    else if(strncmp(fmt4,"BA24",4)==0) return BA24;
    else return UnknownFormat;
  }
  
  ImageBuffer(int rows=0, int cols=0, int format=0, uint8_t *data=0) :
    rows(rows),
    cols(cols),
    format(format),
    data(data),
    sharedData(),
    bytes(0),
    stride(cols*sizeOfFormat(format)) {
  }

  void init(int rows=0, int cols=0, int format=0, unsigned char *data=0,
            int bytes=0, bool mustFree=false) {
    this->rows = rows;
    this->cols = cols;
    this->format = format;
    this->data = data;
    if (mustFree) { sharedData.reset(data); }
    this->bytes = bytes;
    this->stride = cols;
  }
  
  void init2(int rows=0, int cols=0, int format=0, unsigned char *data=0,
             int stride = 0, int bytes=0, bool mustFree=false) {
    this->rows = rows;
    this->cols = cols;
    this->format = format;
    this->data = data;
    if (mustFree) { sharedData.reset(data); }
    this->bytes = bytes;
    this->stride = stride;
  }

  const char * formatName(int f = -1) const {
    switch((f<0?format:f)) {
    // case BGR24: return "BGR24";
    // case BGR32: return "BGR32";
    // case RGB24: return "RGB24";
    // case RGB32: return "RGB32";
    // // case YUV422: return "YUV422"; //deprecated
    // case YUYV: return "YUVV";
    // case NV12: return "NV12";
    // case JPEG: return "JPEG";
    case YU12: return "YU12";
    case YUYV: return "YUYV";
    case RGB3: return "RGB3";
    case JPEG: return "JPEG";
    case H264: return "H264";
    case MJPG: return "MJPG";
    case YVYU: return "YVYU";
    case VYUY: return "VYUY";
    case UYVY: return "UYVY";
    case BGR3: return "BGR3";
    case YV12: return "YV12";
    case NV12: return "NV12";
    case NV21: return "NV21";
    case RX24: return "RX24";
    case BA24: return "BA24";
    default: break;
    }
    return "UNKN";
  }
  
  static int sizeOfFormat(int format) {
    switch(format) {
    // case BGR24:  return 3;
    // case BGR32:  return 4;
    // case RGB24:  return 3;
    // case RGB32:  return 4;
    // case YUV422: //deprecated
    // case YUYV:   return 2;
    // case NV12:   return 1;
    case YU12: return 1;
    case YUYV: return 2;
    case RGB3: return 3;
    case YVYU: return 2;
    case VYUY: return 2;
    case UYVY: return 2;
    case BGR3: return 3;
    case YV12: return 1;
    case NV12: return 1;
    case NV21: return 1;
    case RX24: return 4;
    case BA24: return 4;
    case JPEG:
    default: break;
    }
    return -1;
  }
  
};

#endif //__ImageBuffer_hpp__
