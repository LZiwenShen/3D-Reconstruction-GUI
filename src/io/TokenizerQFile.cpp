//------------------------------------------------------------------------
// Copyright (C) Gabriel Taubin
// Time-stamp: <2021-04-24 23:01:01 taubin>
//------------------------------------------------------------------------
//
// TokenizerQFile.cpp
//
// Software developed for the Fall 2015 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2021, Gabriel Taubin
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
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "TokenizerQFile.hpp"

TokenizerQFile::TokenizerQFile(QFile & qf):
  Tokenizer(),
  _bufPos(0),
  _bufCap(0),
  _qf(qf) {

}

int TokenizerQFile::getchar() {
  
  if(_bufPos>=_bufCap) {
    _bufCap = _qf.read(_buf,BUF_LEN);
    _bufPos = 0;
  }

  if(_bufCap==0)
    return EOF;
  else if(_bufPos<_bufCap)
    return _buf[_bufPos++];
  else
    return 0x00;
}
