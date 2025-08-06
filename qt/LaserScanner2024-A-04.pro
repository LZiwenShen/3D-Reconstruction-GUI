NAME = LaserScanner2024-A-04

#
# vim:filetype=qmake sw=4 ts=4 expandtab nospell
#

# QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0

# Windows
win32:OPENCV_DIR = "C:\opencv\build"
win32:OPENCV_LIB_DIR = $${OPENCV_DIR}/x64/vc16/lib
win32:OPENCV_BIN_DIR = $${OPENCV_DIR}/x64/vc16/bin
win32:OPENCV_INC_DIR = $${OPENCV_DIR}/include
win32:CV_VER = 4100

# MacOS Intel + OpenCV 4.10.0
macx:OPENCV_DIR = "/usr/local/opt/opencv"
# MacBookAir M1/M2 + OpenCV 4.10.0
# macx:OPENCV_DIR = "/opt/homebrew/opt/opencv"
macx:OPENCV_LIB_DIR = $$OPENCV_DIR/lib
macx:OPENCV_INC_DIR = $$OPENCV_DIR/include/opencv4
macx:CV_VER = 410

unix:!macx:CONFIG += USE_UNIX_DAEMONIZE
CONFIG += sdk_no_version_check

##########################################################################

BASEDIR = ..
TOPDIR = $$BASEDIR/..

win32:EIGEN_DIR = C:\Users\16693\OneDrive\Desktop\3D\Eigen
macx:EIGEN_DIR  = /Users/taubin/GitHub/libeigen/eigen
# macx:EIGEN_DIR  = /Users/gtaubin/GitHub/libeigen/eigen

UI_DIR = GeneratedFiles
DESTDIR = $$BASEDIR/bin
FORMSDIR = $$BASEDIR/forms
SOURCEDIR = $$BASEDIR/src
ASSETSDIR = $$BASEDIR/assets

CONFIG += qt
QT += core
# QT += serialport
QT += widgets
QT += openglwidgets
QT += multimedia

DEFINES += HAVE_HOMEWORK
DEFINES += HAVE_QTMULTIMEDIA

win32 {
  CV_LIB_NAMES = world
  CONFIG(release, debug|release) {
    CV_LIBS += -lopencv_world$${CV_VER}
  } else {
    CV_LIBS += -lopencv_world$${CV_VER}d
  }
}
macx {
  CV_LIB_NAMES = core imgcodecs imgproc highgui calib3d features2d flann
  for(lib, CV_LIB_NAMES) {
  # CV_LIBS += -lopencv_$${lib}.$${CV_VER}
  CV_LIBS += -lopencv_$${lib}
  }
}

DEFINES += HAVE_OPENCV

win32 {
    DEFINES += NOMINMAX _CRT_SECURE_NO_WARNINGS
    DEFINES += _SCL_SECURE_NO_WARNINGS _USE_MATH_DEFINES
    QMAKE_CXXFLAGS_WARN_ON += -W3 -wd4396 -wd4100 -wd4996
    QMAKE_LFLAGS += /INCREMENTAL:NO
    DSHOW_LIBS = -lStrmiids -lVfw32 -lOle32 -lOleAut32 -lopengl32
}

unix:!macx {
    QMAKE_LFLAGS += -Wl,-rpath=$$OPENCV_DIR/lib
    # QMAKE_CXXFLAGS += -g
}

macx {
    # OTHER_CPLUSPLUSFLAGS += -std=gnu++11
    # QMAKE_CXXFLAGS += -std=gnu++11
    # LIBS += -framework Foundation -framework QTKit 
    LIBS += -framework CoreFoundation -framework IOkit
}

CONFIG(release, debug|release) {
    TARGET = $$NAME
} else {
    TARGET = $${NAME}_d
}
CONFIG += console

LIBS += -L$$OPENCV_LIB_DIR  -L$$OPENCV_BIN_DIR $$CV_LIBS
message(LIBS = $$LIBS)

include($${NAME}.pri)
