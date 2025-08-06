#
# vim:filetype=qmake sw=4 ts=4 expandtab nospell
#

FORMS = \
        $$FORMSDIR/AboutDialog.ui \
        $$FORMSDIR/PanelCalibrate.ui \
        $$FORMSDIR/PanelCapture.ui \
        $$FORMSDIR/MainWindow.ui \
        $$FORMSDIR/PanelScan.ui \
        $$(NULL)

RESOURCES = \
	$$ASSETSDIR/assets.qrc

APP_DIR  = $$SOURCEDIR/app
UTL_DIR  = $$SOURCEDIR/util
IMG_DIR  = $$SOURCEDIR/img
MAT_DIR  = $$SOURCEDIR/matlab-io
IO_DIR   = $$SOURCEDIR/io
HWK_DIR  = $$SOURCEDIR/homework
GEO_DIR  = $$SOURCEDIR/geo
WRL_DIR  = $$SOURCEDIR/wrl

SOURCES += \
        $$APP_DIR/AboutDialog.cpp \
        $$APP_DIR/Application.cpp \
        $$APP_DIR/CalibrationData.cpp \
        $$APP_DIR/ScannerCalibData.cpp \
        $$APP_DIR/ChessboardCorners.cpp \
        $$APP_DIR/ImgGLWidget.cpp \
        $$APP_DIR/MainWindow.cpp \
        $$APP_DIR/PanelCapture.cpp \
        $$APP_DIR/PanelCalibrate.cpp \
        $$APP_DIR/PanelScan.cpp \
        $$APP_DIR/PlotMaker.cpp \
        $$APP_DIR/RpiScanner3D.cpp \
        $$APP_DIR/TurntableCalib.cpp \
        $$APP_DIR/WrlGLBuffer.cpp \
        $$APP_DIR/WrlGLHandles.cpp \
        $$APP_DIR/WrlGLShader.cpp \
        $$APP_DIR/WrlGLWidget.cpp \
        $$APP_DIR/WrlQtLogo.cpp \
        $$APP_DIR/WrlViewerData.cpp \
        $$APP_DIR/main.cpp \
	\
        $$IMG_DIR/Image.cpp \
        $$IMG_DIR/ImageBuffer.cpp \
        $$IMG_DIR/Img.cpp \
        $$IMG_DIR/ImgArgb.cpp \
        $$IMG_DIR/ImgBit.cpp \
        $$IMG_DIR/ImgBitplane.cpp \
        $$IMG_DIR/ImgBuckets.cpp \
        $$IMG_DIR/ImgCheckerboards.cpp \
        $$IMG_DIR/ImgDraw.cpp \
        $$IMG_DIR/ImgFloat.cpp \
        $$IMG_DIR/ImgInt.cpp \
        $$IMG_DIR/util.cpp \
        $$IMG_DIR/yuv.cpp \
	\
        $$IO_DIR/AppLoader.cpp \
        $$IO_DIR/AppSaver.cpp \
        $$IO_DIR/LoaderWrl.cpp \
        $$IO_DIR/SaverWrl.cpp \
        $$IO_DIR/Tokenizer.cpp \
        $$IO_DIR/TokenizerFILE.cpp \
        $$IO_DIR/TokenizerQFile.cpp \
	\
        $$MAT_DIR/MatlabIO.cpp \
        $$MAT_DIR/matlab_io_util.cpp \
	\
        $$UTL_DIR/CircleNeighborhood.cpp \
        $$UTL_DIR/GuiUtil.cpp \
        $$UTL_DIR/LineDetector6.cpp \
        $$UTL_DIR/MatrixUtil.cpp \
        $$UTL_DIR/Partition.cpp \
        $$UTL_DIR/PartitionLists.cpp \
        $$UTL_DIR/StaticRotation.cpp \
	\
        $$GEO_DIR/BBox.cpp \
        $$GEO_DIR/Graph.cpp \
        $$GEO_DIR/GraphFaces.cpp \
        $$GEO_DIR/GraphPlanar.cpp \
        $$GEO_DIR/GraphTraversal.cpp \
        $$GEO_DIR/Heap.cpp \
        $$GEO_DIR/Mesh.cpp \
        $$GEO_DIR/MeshSelection.cpp \
        $$GEO_DIR/Polylines.cpp \
        $$GEO_DIR/Vec.cpp \
	\
        $$WRL_DIR/Appearance.cpp \
        $$WRL_DIR/Group.cpp \
        $$WRL_DIR/HexahedralMesh.cpp \
        $$WRL_DIR/ImageTexture.cpp \
        $$WRL_DIR/IndexedFaceSet.cpp \
        $$WRL_DIR/IndexedLineSet.cpp \
        $$WRL_DIR/IsoSurf.cpp \
        $$WRL_DIR/Material.cpp \
        $$WRL_DIR/Node.cpp \
        $$WRL_DIR/PixelTexture.cpp \
        $$WRL_DIR/Rotation.cpp \
        $$WRL_DIR/SceneGraph.cpp \
        $$WRL_DIR/SceneGraphProcessor.cpp \
        $$WRL_DIR/SceneGraphTraversal.cpp \
        $$WRL_DIR/Shape.cpp \
        $$WRL_DIR/SimpleGraphMap.cpp \
        $$WRL_DIR/Transform.cpp \
	\
        $$HWK_DIR/homework1.cpp \
        $$HWK_DIR/homework2.cpp \
        $$HWK_DIR/homework3.cpp \
        $$HWK_DIR/homework4.cpp \
	\
        $$(NULL)

HEADERS += \
        $$APP_DIR/AboutDialog.hpp \
        $$APP_DIR/Application.hpp \
        $$APP_DIR/CalibrationData.hpp \
        $$APP_DIR/ScannerCalibData.hpp \
        $$APP_DIR/ChessboardCorners.hpp \
        $$APP_DIR/CSocket.hpp \
        $$APP_DIR/ImgGLWidget.hpp \
        $$APP_DIR/Log.hpp \
        $$APP_DIR/MainWindow.hpp \
        $$APP_DIR/PanelCapture.hpp \
        $$APP_DIR/PanelCalibrate.hpp \
        $$APP_DIR/PanelScan.hpp \
        $$APP_DIR/PlotMaker.hpp \
        $$APP_DIR/RpiScanner3D.hpp \
        $$APP_DIR/TurntableCalib.hpp \
        $$APP_DIR/WrlGLBuffer.hpp \
        $$APP_DIR/WrlGLHandles.hpp \
        $$APP_DIR/WrlGLShader.hpp \
        $$APP_DIR/WrlGLWidget.hpp \
        $$APP_DIR/WrlQtLogo.hpp \
        $$APP_DIR/WrlViewerData.hpp \
	\
        $$IMG_DIR/ARGB.hpp \
        $$IMG_DIR/Image.hpp \
        $$IMG_DIR/ImageBuffer.hpp \
        $$IMG_DIR/Img.hpp \
        $$IMG_DIR/ImgArgb.hpp \
        $$IMG_DIR/ImgBit.hpp \
        $$IMG_DIR/ImgBitplane.hpp \
        $$IMG_DIR/ImgBuckets.hpp \
        $$IMG_DIR/ImgCheckerboards.hpp \
        $$IMG_DIR/ImgDraw.hpp \
        $$IMG_DIR/ImgFloat.hpp \
        $$IMG_DIR/ImgInt.hpp \
        $$IMG_DIR/util.hpp \
        $$IMG_DIR/yuv.hpp \
	\
        $$IO_DIR/AppLoader.hpp \
        $$IO_DIR/AppSaver.hpp \
        $$IO_DIR/Loader.hpp \
        $$IO_DIR/LoaderWrl.hpp \
        $$IO_DIR/Saver.hpp \
        $$IO_DIR/SaverWrl.hpp \
        $$IO_DIR/Tokenizer.hpp \
        $$IO_DIR/TokenizerFILE.hpp \
        $$IO_DIR/TokenizerQFile.hpp \
	\
        $$MAT_DIR/EFStream.hpp \
        $$MAT_DIR/MatlabIO.hpp \
        $$MAT_DIR/matlab_io_util.hpp \
        $$MAT_DIR/typetraits.hpp \
	\
        $$UTL_DIR/CircleNeighborhood.hpp \
        $$UTL_DIR/GuiUtil.hpp \
        $$UTL_DIR/LineDetector6.hpp \
        $$UTL_DIR/MatrixUtil.hpp \
        $$UTL_DIR/Partition.hpp \
        $$UTL_DIR/PartitionLists.hpp \
        $$UTL_DIR/StaticRotation.hpp \
        $$UTL_DIR/StrException.hpp \
	\
        $$GEO_DIR/BBox.hpp \
        $$GEO_DIR/Graph.hpp \
        $$GEO_DIR/GraphEdge.hpp \
        $$GEO_DIR/GraphFaces.hpp \
        $$GEO_DIR/GraphPlanar.hpp \
        $$GEO_DIR/GraphTraversal.hpp \
        $$GEO_DIR/Heap.hpp \
        $$GEO_DIR/Mesh.hpp \
        $$GEO_DIR/MeshSelection.hpp \
        $$GEO_DIR/Polylines.hpp \
        $$GEO_DIR/Vec.hpp \
	\
        $$WRL_DIR/Appearance.hpp \
        $$WRL_DIR/Group.hpp \
        $$WRL_DIR/HexahedralMesh.hpp \
        $$WRL_DIR/ImageTexture.hpp \
        $$WRL_DIR/IndexedFaceSet.hpp \
        $$WRL_DIR/IndexedLineSet.hpp \
        $$WRL_DIR/IsoSurf.hpp \
        $$WRL_DIR/Material.hpp \
        $$WRL_DIR/Node.hpp \
        $$WRL_DIR/PixelTexture.hpp \
        $$WRL_DIR/Rotation.hpp \
        $$WRL_DIR/SceneGraph.hpp \
        $$WRL_DIR/SceneGraphProcessor.hpp \
        $$WRL_DIR/SceneGraphTraversal.hpp \
        $$WRL_DIR/Shape.hpp \
        $$WRL_DIR/SimpleGraphMap.hpp \
        $$WRL_DIR/Transform.hpp \
	\
        $$HWK_DIR/homework1.hpp \
        $$HWK_DIR/homework2.hpp \
        $$HWK_DIR/homework3.hpp \
        $$HWK_DIR/homework4.hpp \
	\
	$$(NULL)

INCLUDEPATH += $$SOURCEDIR $$UI_DIR $$APP_DIR $$HWK_DIR
INCLUDEPATH += $$IMG_DIR $$IO_DIR $$MAT_DIR $$UTL_DIR $$WRL_DIR
INCLUDEPATH += $$EIGEN_DIR $$OPENCV_INC_DIR

message(INCLUDEPATH = $$INCLUDEPATH)
message(PATH = $$PATH)

