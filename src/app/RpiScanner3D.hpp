// Time-stamp: <2024-11-23 13:08:47 taubin>
//
// Software developed for the Spring 2021 Brown University course
// ENGN 2502 3D Photography
// Copyright (c) 2021 Gabriel Taubin
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

#ifndef _RPI_SCANNER_3D_HPP_
#define _RPI_SCANNER_3D_HPP_

#include <QString>
#include <QThread>
#include <QByteArray>

#ifndef PATH_LENGTH
#define PATH_LENGTH 4096
#endif
#ifndef MAX_MSG_LENGTH
#define MAX_MSG_LENGTH 1024
#endif
#ifndef MAX_RESPONSE_LINES
#define MAX_RESPONSE_LINES 64
#endif // MAX_RESPONSE_LINES

class RpiScanner3D : public QThread {
  Q_OBJECT

private:

  char        _hostname[PATH_LENGTH];
  char        _ip[32];
  int         _port;
  char        _clientScansDir[PATH_LENGTH];
  char        _serverScansDir[PATH_LENGTH];
  char        _cwd[PATH_LENGTH];
  char        _path[PATH_LENGTH];
  char        _prtbuf[PATH_LENGTH];
  char        _message[MAX_MSG_LENGTH];
  char        _response[MAX_MSG_LENGTH];
  char*       _responseLine[MAX_RESPONSE_LINES];
  bool        _serverSaveImages;
  bool        _verbose;
  char        _pixelFormat[5];
  char        _frameSize[64];
  int         _laserMode;
  int         _stepsPerFrame;
  int         _numberOfFrames;
  int         _usecWait;
  bool        _stop;
  bool        _preview;
  int         _fdSocket;

public:
  
  RpiScanner3D(QObject* parent=0);
  ~RpiScanner3D();

  void        setVerbose(const bool value);
  QString     hostname() const;
  int         port() const;
  QString     ipAddress() const;
  bool        setHostname(const QString & hostname);
  void        setPort(const int port);
  bool        setIpAddress(const uint32_t ipv4);
  QString     getServerInfo(const char* request);
  QString     getServerTime();

  QString     getResponse();
  void        setLaserMode(int laserMode);
  void        setStepsPerFrame(int stepsPerFrame);
  void        setNumberOfFrames(int numberOfFrames);
  void        setUsecWait(int usecWait);
  bool        setPixelFormat(const char* pixelFormat);
  bool        setFrameSize(const char* frameSize);

  void        setServerSaveImages(const bool value);
  bool        serverSaveImages();
  bool        setClientScansDir(const char* scansDir);
  bool        setServerScansDir(const char* scansDir);

  void        setModeCapture();
  void        setModePreview();

  void        capture(const int laserMode, const int stepsPerFrame,
                      const int numberOfFrames, const int usecWait);
  void        capture();

  void        preview();
  
  void        stopCapturePreview();

protected:

  void        run();

private:

  void        _log(const char* msg);
  int         _hostnameToIpAddress(const char* hostname , char* ip);  

  bool        _connect();
  bool        _isConnected();
  void        _close();
  bool        _sendMessage();
  bool        _receiveResponse();

  int         _splitResponse();

  const char* _sendReceiveCommand(const char* cmd);

signals:

  void        captureStarted(const char* localScanDir);
  void        captureProgress(const char* imageFileName, QByteArray* imageBuffer);
  void        captureFinished();

  void        streamStarted();
  void        streamProgress(const int image_no, QByteArray* imageBuffer);
  void        streamFinished();  
  
};

#endif // _RPI_SCANNER_3D_HPP_
