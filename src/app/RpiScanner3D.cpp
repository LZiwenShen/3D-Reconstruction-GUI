// Time-stamp: <2024-11-23 23:06:16 taubin>
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

#include <img/ImageBuffer.hpp>
#include "RpiScanner3D.hpp"

// TODO Sun Nov 10 10:24:10 2024
// - add support for windows using
//   winsock2.h instead of sys/socket.h

#ifdef Q_OS_MACOS
#include <CSocket.hpp>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif // Q_OS_MACOS

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
// #include <ctype.h>
#include <string.h>
#include <stdlib.h>
// #include <errno.h>
#include <fcntl.h>

#include <util/StrException.hpp>

RpiScanner3D::RpiScanner3D(QObject* parent):
  QThread(parent),
  _hostname(""),
  _ip(""),
  _port(5000),
  _clientScansDir(""),
  _serverScansDir("/Volumes/share02"), // TODO : expose to GUI
  _cwd(""),
  _path(""),
  _prtbuf(""),
  _message(""),
  _serverSaveImages(false),
  _verbose(false),
  _pixelFormat("JPEG"),
  _frameSize("1640x1232"),
  _laserMode(0),
  _stepsPerFrame(0),
  _numberOfFrames(0),
  _usecWait(0),
  _stop(false),
  _preview(true),
  _fdSocket(-1) {
}

RpiScanner3D::~RpiScanner3D() {
}

// static
void RpiScanner3D::setVerbose(const bool value) {
  _verbose = value;
}

// static
void RpiScanner3D::_log(const char* msg) {
  if(_verbose) {
    fprintf(stdout,"rpiScanner3D | %s\n",msg);
    fflush(stdout);
  }
}

int RpiScanner3D::port() const {
  return _port;
}

void RpiScanner3D::setPort(const int port) {
  _port = port;
}

QString RpiScanner3D::getServerInfo(const char* request) {
  const char* response = _sendReceiveCommand(request);
  return QString(response);
}

QString RpiScanner3D::getServerTime() {
  return getServerInfo("time");
  // const char* response = _sendReceiveCommand("time");
  // return QString(response);
}

QString RpiScanner3D::getResponse() {
  return QString(_response);
}

void RpiScanner3D::setLaserMode(int laserMode) {
  _laserMode = laserMode;
}

void RpiScanner3D::setStepsPerFrame(int stepsPerFrame) {
  _stepsPerFrame = stepsPerFrame;
}

void RpiScanner3D::setNumberOfFrames(int numberOfFrames) {
  _numberOfFrames = numberOfFrames;
}

void RpiScanner3D::setUsecWait(int usecWait) {
  _usecWait = usecWait;
}

bool RpiScanner3D::setPixelFormat(const char* pixelFormat) {
  if(pixelFormat==NULL) return false;
  if(strlen(pixelFormat)!=4) return false;
  strncpy(_pixelFormat,pixelFormat,4); _pixelFormat[4] = 0x0;
  return true;
}

bool RpiScanner3D::setFrameSize(const char* frameSize) {
  if(frameSize==NULL) return false;
  memset(_frameSize,0,sizeof(_frameSize));
  strcpy(_frameSize,frameSize);
  return true;
}

void RpiScanner3D::setServerSaveImages(const bool value) {
  _serverSaveImages = value;
}

bool RpiScanner3D::serverSaveImages() {
  return _serverSaveImages;
}

bool RpiScanner3D::setClientScansDir(const char* scansDir) {
  memset(_clientScansDir,0,PATH_LENGTH);
  if(scansDir==NULL) return true; // reset
  int len = strlen(scansDir);
  if(len>=PATH_LENGTH) return false; // error
  strcpy(_clientScansDir,scansDir);
  return true; // success
}

bool RpiScanner3D::setServerScansDir(const char* scansDir) {
  memset(_serverScansDir,0,PATH_LENGTH);
  if(scansDir==NULL) return true; // reset
  int len = strlen(scansDir);
  if(len>=PATH_LENGTH) return false; // error
  strcpy(_serverScansDir,scansDir);
  return true; // success
}

//////////////////////////////////////////////////////////////////////

QString RpiScanner3D::hostname() const {
  return _hostname;
}

bool RpiScanner3D::setHostname(const QString & hostname) {
  snprintf(_hostname,PATH_LENGTH,"%s",hostname.toStdString().c_str());
  strcpy(_ip,"");
  return (_hostnameToIpAddress(_hostname,_ip)<0)?false:true;
}

int RpiScanner3D::_hostnameToIpAddress(const char* hostname , char* ip) {
#ifdef Q_OS_MACOS
  struct hostent *he;
  struct in_addr **addr_list;
  int i;
  
  if((he=gethostbyname(hostname)) == NULL) {
    herror("gethostbyname");
    return -1; // ERROR
  }
  
  addr_list = (struct in_addr **) he->h_addr_list;
  
  for(i = 0; addr_list[i] != NULL; i++) {
    //Return the first one;
    strcpy(ip , inet_ntoa(*addr_list[i]) );
    return 0;
  }
#endif // Q_OS_MACOS
  return -1; // ERROR
}

QString RpiScanner3D::ipAddress() const {
  // const uint8_t* ip = reinterpret_cast<const uint8_t*>(&_ipv4);
  // return QString("%1.%2.%3.%3").arg(ip[0]).arg(ip[1]).arg(ip[2]).arg(ip[3]);
  return QString(_ip);
}

//////////////////////////////////////////////////////////////////////
void RpiScanner3D::_close() {
#ifdef Q_OS_MACOS
  if(_fdSocket<0) return; // already closed
  close(_fdSocket);
#endif // Q_OS_MACOS
  _fdSocket = -1;
}

//////////////////////////////////////////////////////////////////////
bool RpiScanner3D::_isConnected() {
  return (_fdSocket>=0);
}

//////////////////////////////////////////////////////////////////////
// connect to rpicamd at camserver_ip@_port

bool RpiScanner3D::_connect() {

#ifdef Q_OS_MACOS
  
  try {
    
    if(_isConnected()==false) {

      if((_fdSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw new StrException("ERROR : cannot create socket");
      }
      
      struct sockaddr_in camserver_addr; 
      memset(&camserver_addr,0x0,sizeof(camserver_addr)); 
      camserver_addr.sin_family = AF_INET;
      camserver_addr.sin_port = htons(_port); 
      if(inet_pton(AF_INET, _ip, &camserver_addr.sin_addr)<=0) {
        throw new StrException("ERROR : inet_pton()");
      }
      
      if(csocket::connect_
         (_fdSocket,(struct sockaddr *)&camserver_addr,sizeof(camserver_addr))<0) {
        throw new StrException("ERROR : connect()");
      }
    }
    
  } catch(StrException* e) {
    // _log(e->what());
    delete e;
    _close();
  }

#endif // Q_OS_MACOS

  return _isConnected();
}

//////////////////////////////////////////////////////////////////////
bool RpiScanner3D::_sendMessage() {
  bool success = false;
#ifdef Q_OS_MACOS
  try {

    if(_isConnected()==false) {
      throw new StrException("ERROR : not connected");
    }

    if((write(_fdSocket,_message,sizeof(_message)))
       < static_cast<int>(sizeof(_message))) {
      throw new StrException("ERROR : write(_fdSocket,command)");
    }

    success = true;
  } catch(StrException* e) {
    // _log(e->what());
    delete e;
    _close();
  }
#endif // Q_OS_MACOS
  return success;
}

//////////////////////////////////////////////////////////////////////
bool RpiScanner3D::_receiveResponse() {
  bool success = false;
#ifdef Q_OS_MACOS
  try {

    memset(_response, 0x0,sizeof(_response)); // char _response[1024];

    if(_connect()==false) {
      throw new StrException("ERROR : _connect()");
    }

    // read the response size
    int n_response = 0;
    if(read(_fdSocket,&n_response,sizeof(n_response))
       < static_cast<int>(sizeof(n_response))) {
      throw new StrException("ERROR : reading response size");
    }

    // read the response in chunks
    int i=0,n_read=0;
    while(n_response>0 && (n_read=read(_fdSocket,_response+i,n_response))>0)
      n_response -= n_read;

    if(strncmp("ERROR :",_response,7)==0) {
      snprintf(_prtbuf,PATH_LENGTH,"  \"%s\"",_response); _log(_prtbuf);
      throw new StrException("ERROR : received from server");
    }

    success = true;
    
  } catch(StrException* e) {
    // e->what();
    delete e;
  }

#endif // Q_OS_MACOS

  return success;
}

//////////////////////////////////////////////////////////////////////
int RpiScanner3D::_splitResponse() {
  int nLines = 0;

  // memset(_responseLine,0x0,sizeof(_responseLine)); // char _response[1024];
  for(int i=0;i<MAX_RESPONSE_LINES;i++) _responseLine[i] = (char*)0;

  int nResponse = strlen(_response);
  for(int i=0,j=1;j<=nResponse;j++) {
    char c = _response[j];
    if(c!='\n'&&c!='\0') continue;
    _response[j]='\0';
    if(nLines<MAX_RESPONSE_LINES) _responseLine[nLines] = _response+i;
    snprintf(_prtbuf,PATH_LENGTH,
             "    [%2d] \"%s\"",nLines,_response+i); _log(_prtbuf);
    nLines++;
    if(c=='\0') break; // EOL
    i=j+1;
  }
  
  if(nLines>MAX_RESPONSE_LINES) {
    snprintf(_prtbuf,PATH_LENGTH,
             "  WARNING : %d response lines",nLines); _log(_prtbuf);
  }

  return nLines;
}

//////////////////////////////////////////////////////////////////////
const char* RpiScanner3D::_sendReceiveCommand(const char* command) {

  char* response = (char*)0;

  snprintf(_prtbuf,PATH_LENGTH,
           "RpiScanner3D::_sendReceiveCommand(%s) {",command);
  _log(_prtbuf);

  try {

    if(_connect()==false) {
      throw new StrException("ERROR : _connect()");
    }
   
    // send command //////////////////////////////////////////////////////

    if(strlen(command)+1>MAX_MSG_LENGTH) {
      throw new StrException("ERROR : strlen(command)+1>MAX_MSG_LENGTH");
    }

    snprintf(_prtbuf,PATH_LENGTH,"  command"); _log(_prtbuf);
    snprintf(_prtbuf,PATH_LENGTH,"    \"%s\"",command); _log(_prtbuf);
    snprintf(_message,MAX_MSG_LENGTH,"%s",command);
    
    if(_sendMessage()==false) {
      throw new StrException("ERROR : _send(command)");
    }

    // receive response //////////////////////////////////////////////////

    if(_receiveResponse()==false) {
      throw new StrException("ERROR : _receiveResponse()");
    }

    int nLines = _splitResponse();
    if(nLines<1) {
      throw new StrException("ERROR : nLines(response)<1");
    }

    response = _response;

  } catch(StrException* e) {
    // response = e->what(); // ???
    delete e;
  }

  // close connection
  _close();
  
  _log("}");
  return response;
}

////////////////////////////////////////////////////////////////////////////////////
void RpiScanner3D::setModeCapture() {
  _preview = false;
}

////////////////////////////////////////////////////////////////////////////////////
void RpiScanner3D::setModePreview() {
  _preview = true;
}

////////////////////////////////////////////////////////////////////////////////////
void RpiScanner3D::run() {
  if(_preview)
    preview();
  else
    capture();
  quit();
}

//////////////////////////////////////////////////////////////////////
void RpiScanner3D::stopCapturePreview() {
  _stop = true;;
}

//////////////////////////////////////////////////////////////////////
void RpiScanner3D::capture() {
  capture(_laserMode,_stepsPerFrame,_numberOfFrames,_usecWait);
}

//////////////////////////////////////////////////////////////////////
void RpiScanner3D::capture
(const int laserMode, const int stepsPerFrame,
 const int numberOfFrames, const int usecWait) {
#ifdef Q_OS_MACOS
  strcpy(_response,"");

  if(laserMode<=0 || laserMode>7 || stepsPerFrame<1 ||
     numberOfFrames<1 || usecWait<0) {
    return;
  }

  _log("RpiScanner3D::capture() {");

  snprintf(_prtbuf,PATH_LENGTH,
           "    pixelFormat = \"%s\"",_pixelFormat); _log(_prtbuf);
  snprintf(_prtbuf,PATH_LENGTH,
           "    frameSize   = \"%s\"",_frameSize); _log(_prtbuf);

  char command[MAX_MSG_LENGTH]; memset(command,0,MAX_MSG_LENGTH);
  snprintf(command,MAX_MSG_LENGTH,
           "capture %s format=%s size=%s lasers=%d steps=%d frames=%d wait=%d",
           (_serverSaveImages?"save":"send"),_pixelFormat,_frameSize,
           laserMode,stepsPerFrame,numberOfFrames,usecWait);

  char local_frame_image[PATH_LENGTH];
  memset(local_frame_image,0,PATH_LENGTH);
  char local_scan_dir[PATH_LENGTH];
  memset(local_scan_dir,0,PATH_LENGTH);

  try {

    _stop = false;

    if(_connect()==false) {
      throw new StrException("ERROR : _connect()");
    }

    // send scan command ///////////////////////////////////////////////////////////

    if(strlen(command)+1>MAX_MSG_LENGTH) {
      throw new StrException("ERROR : strlen(command)+1>MAX_MSG_LENGTH");
    }

    snprintf(_prtbuf,PATH_LENGTH,"  command"); _log(_prtbuf);
    snprintf(_prtbuf,PATH_LENGTH,"    \"%s\"",command); _log(_prtbuf);
    snprintf(_message,MAX_MSG_LENGTH,"%s",command);

    if(_sendMessage()==false) {
      throw new StrException("ERROR : _send(command)");
    }

    // receive first response //////////////////////////////////////////////////////

    if(_receiveResponse()==false) {
      throw new StrException("ERROR : _receiveResponse()");
    }

    snprintf(_prtbuf,PATH_LENGTH,
             "    response received \"%s\"",_response);
    _log(_prtbuf);
    
    int len = strlen("started ");
    if(strncmp(_response,"started ",len)!=0) {
      throw new StrException("expecting \"started \" response");
    }
    char* scan_dir = _response+len;
    // scan_dir should always be a relative path
    snprintf(_prtbuf,PATH_LENGTH,"    scan_dir %s",scan_dir); _log(_prtbuf);

    if(_serverSaveImages) {
      // server sends images
      snprintf(local_scan_dir,PATH_LENGTH,"%s/%s",_serverScansDir,scan_dir);
      // directory should have been created by the server

    } else /* if(_serverSaveImages==false) */ {
      // server sends images
      snprintf(local_scan_dir,PATH_LENGTH,"%s/%s",_clientScansDir,scan_dir);
      // create a new sub directory of _clientScansDir
      if(mkdir(local_scan_dir, 0755)<0) {
        // throw new StrException("ERROR : ???");
        snprintf(_prtbuf,PATH_LENGTH,
                 "    mkdir(%s)<0",local_scan_dir);
        _log(_prtbuf);
        memset(local_scan_dir,0,PATH_LENGTH);
      } else {
        snprintf(_prtbuf,PATH_LENGTH,
                 "    created local_scan_dir %s",local_scan_dir);
        _log(_prtbuf);
      }
    }

    emit captureStarted(local_scan_dir);

    // main loop
    while(_fdSocket>=0) {

      // receive response
      if(_receiveResponse()==false) {
        throw new StrException("ERROR : _receiveResponse()");
      }

      // end of loop conditions
      if(_stop) break;
      len = strlen("finished");
      if(strncmp(_response,"finished",len)==0) break;

      // expecting "progress <frame_image_length> <frame_image_file_name>"
      len = strlen("progress ");
      if(strncmp(_response,"progress ",len)!=0) {
        throw new StrException("ERROR : expecting \"progress \" response");
      }
      int  frame_image_length = 0;
      char frame_image_file_name[128];
      memset(frame_image_file_name,0,sizeof(frame_image_file_name));
      if(sscanf(_response+len,"%d %s",
                &frame_image_length,frame_image_file_name)<2) {
        throw new StrException("ERROR : expecting <length> <name>");
      }

      if(frame_image_length>0) { // receive frame data

        QByteArray* imageByteArray = new QByteArray(frame_image_length,0);
        char* image_data = imageByteArray->data();
        
        if(_serverSaveImages==true) {
          
          // image data has alredy been saved by server on shared file

          // create absolute path file name

          memset(local_frame_image,0,sizeof(local_frame_image));          
          snprintf(local_frame_image,PATH_LENGTH,"%s/%s",
                   local_scan_dir,frame_image_file_name);
          snprintf(_prtbuf,PATH_LENGTH,"    reading %s ...",
                   local_frame_image); _log(_prtbuf);

          // load image data from file

          FILE* ifp = fopen(local_frame_image,"rb");
          if(ifp==(FILE*)0) {
            _log("    unable to open frame_image file for reading");
          } else {
            char*  p = image_data;
            size_t n_read      = 0;
            size_t n_remaining = frame_image_length;
            while(n_remaining>0) {
               n_read = fread(p,n_remaining,1,ifp);
              if(n_read==0) break;
              p += n_read;
              n_remaining -= n_read;
            }
            if(n_remaining>0) {
              _log("      unable to read frame_image from file");
            } else {
              _log("      read from file");
            }
          }
          fclose(ifp);
          
        } /* else ... */

        else /* if(_serverSaveImages==false) */ {

          // receive image data from server through pipe

          char*  p = image_data;
          size_t n_read      = 0;
          size_t n_remaining = frame_image_length;
          while(n_remaining>0) {
            n_read=(_fdSocket<0)?0:read(_fdSocket,p,n_remaining);
            // if(n_read==0) break;
            p += n_read;
            n_remaining -= n_read;
          }
          // if(n_remaining>0) ... report ERROR        

          // create absolute path file name

          memset(local_frame_image,0,sizeof(local_frame_image));          
          snprintf(local_frame_image,PATH_LENGTH,"%s/%s",
                   local_scan_dir,frame_image_file_name);
          snprintf(_prtbuf,PATH_LENGTH,"    writing %s ...",
                   local_frame_image); _log(_prtbuf);

          // save image data to file

          FILE* ofp = fopen(local_frame_image,"wb");
          if(ofp==(FILE*)0) {
            _log("    unable to open frame_image file for writing");
          } else {
            int n_written =
              fwrite(image_data,1,frame_image_length,ofp);
            if(n_written<frame_image_length) {
              snprintf(_prtbuf,PATH_LENGTH,"    only able to write %d of %d bytes",
                       n_written,frame_image_length);
              _log(_prtbuf);
            } else /* if(success) */ {
              _log("    success");
            }
          }
          fclose(ofp);
        }

        emit captureProgress(local_frame_image,imageByteArray);
        // delete imageByteArray;

      } // } if(frame_image_length>0)

      if(_serverSaveImages)
        snprintf(_prtbuf,PATH_LENGTH,"    %s [%d bytes on file]",
                 frame_image_file_name,frame_image_length);
      else
        snprintf(_prtbuf,PATH_LENGTH,"    %s [received %d bytes]",
                 frame_image_file_name,frame_image_length);
        
      _log(_prtbuf);

    } // } while(_fdSocket>=0)

  } catch(StrException* e) {
    _log(e->what());
    delete e;
  }

  // close connection
  _close();

  emit captureFinished();
  
  _log("}");

#endif // Q_OS_MACOS
}

////////////////////////////////////////////////////////////////////////////////////
void RpiScanner3D::preview() {
#ifdef Q_OS_MACOS
  strcpy(_response,"");

  _log("RpiScanner3D::preview() {");

  snprintf(_prtbuf,PATH_LENGTH,
           "    pixelFormat = \"%s\"",_pixelFormat); _log(_prtbuf);
  snprintf(_prtbuf,PATH_LENGTH,
           "    frameSize   = \"%s\"",_frameSize); _log(_prtbuf);

  char command[MAX_MSG_LENGTH]; memset(command,0,MAX_MSG_LENGTH);
  snprintf(command,MAX_MSG_LENGTH,
           "stream format=%s size=%s",_pixelFormat,_frameSize);

  try {

    _stop = false;

    if(_connect()==false) {
      throw new StrException("ERROR : _connect()");
    }

    // send scan command ///////////////////////////////////////////////////////////

    if(strlen(command)+1>MAX_MSG_LENGTH) {
      throw new StrException("ERROR : strlen(command)+1>MAX_MSG_LENGTH");
    }

    snprintf(_prtbuf,PATH_LENGTH,"  command"); _log(_prtbuf);
    snprintf(_prtbuf,PATH_LENGTH,"    \"%s\"",command); _log(_prtbuf);
    snprintf(_message,MAX_MSG_LENGTH,"%s",command);
    
    if(_sendMessage()==false) {
      throw new StrException("ERROR : _send(command)");
    }

    // receive first response //////////////////////////////////////////////////////

    if(_receiveResponse()==false) {
      throw new StrException("ERROR : _receiveResponse()");
    }
    int len = strlen("started");
    if(strncmp(_response,"started ",len)!=0) {
      throw new StrException("expecting \"started\" response");
    }

    emit streamStarted();

    for(int image_no=0;_fdSocket>=0;image_no++) {

      if(_receiveResponse()==false) {
        throw new StrException("ERROR : _receiveResponse()");
      }

      if(_stop) break;
      len = strlen("finished");
      if(strncmp(_response,"finished",len)==0) break;

      len = strlen("progress ");
      if(strncmp(_response,"progress ",len)!=0) {
        if(_fdSocket<0) break;
        throw new StrException("ERROR : expecting \"progress \" response");
      }

      int  frame_image_length = 0;
      sscanf(_response+len,"%d",&frame_image_length);

      if(frame_image_length>0) { // receive frame data

        QByteArray* imageByteArray = new QByteArray(frame_image_length,0);
        char* image_data = imageByteArray->data();

        char*  p           = image_data;
        size_t n_read      = 0;
        size_t n_remaining = frame_image_length;
        while(n_remaining>0) {
          n_read=(_fdSocket<0)?0:read(_fdSocket,p,n_remaining);
          if(n_read<=0) {
            throw new StrException("n_read<=0");
          }
          p += n_read;
          n_remaining -= n_read;
        }
        
        if(n_remaining>0) {
          snprintf(_prtbuf,PATH_LENGTH,"    %3d [%d/%d]",
                  image_no,(int)n_read,frame_image_length);
          _log(_prtbuf);
        }

        // process  image frame data
        emit streamProgress(image_no, imageByteArray);
        // delete imageByteArray;

      } // } if(frame_image_length>0)

      snprintf(_prtbuf,PATH_LENGTH,"    %3d [received %d bytes]",
              image_no,frame_image_length);
      _log(_prtbuf);

    } // } while(_fdSocket>=0)


  } catch(StrException* e) {
    // e->what();
    delete e;
  }

  // close connection
  _close();

  emit streamFinished();
  
  _log("}");

#endif // Q_OS_MACOS

}

