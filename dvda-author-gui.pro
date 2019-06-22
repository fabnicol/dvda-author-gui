QT += core gui xml widgets
TEMPLATE = app

DEPENDPATH += .
DEFINES += QT_FILE_DIALOG

DEFINES += LOCAL WITHOUT_STARTSECTOR

QMAKE_CXXFLAGS += -static -static-libgcc -static-libstdc++ -pthread  -O3



equals(QT_MAJOR_VERSION, 5) {

TARGET = dvda-author-gui-qt5

QMAKE_LFLAGS += -Wl,-O2
QMAKE_LIBS +=   -lQt5Gui -lQt5Xml -lQt5Core -pthread

win32 {
INCLUDEPATH += . c:/msys64/mingw64/include
QMAKE_CXX = c:/msys64/mingw64/bin/g++
QMAKE_LINK = c:/msys64/mingw64/bin/g++
QMAKE_LIBS +=   C:/msys64/mingw64/lib/libjasper.a
# Windows: using MSYS2 under Mingw64
}

}


equals(QT_MAJOR_VERSION, 4) {

TARGET = dvda-author-gui-qt4

# Windows: using MSYS original 32-bit version under c:/MinGW and Qt4.8.7 under C:/Qt/4.8.7

win32:QMAKE_CXX = c:/MinGW//bin/g++
win32:QMAKE_CC = c:/MinGW//bin/gcc
linux:INCLUDEPATH += . ../qt-everywhere-opensource-src-4.8.7/includewin32:QMAKE_LINK = c:/MinGW//bin/g++
win32:INCLUDEPATH += . C:/Qt/4.8.7/include
linux:QMAKE_LFLAGS += -Wl,-O1 -Wl,-rpath,../qt-everywhere-opensource-src-4.8.7/qtbase/lib
win32:QMAKE_LFLAGS += -Wl,-O1 -Wl,-rpath,C:/Qt/4.8.7/lib
linux:LIBS +=  -L../qt-everywhere-opensource-src-4.8.7/qtbase/lib -lQtXml -lQtGui -L/usr/X11R6/lib -lQtCore -pthread  -lz -lm -ldl -lrt -ljpeg -lmng -ltiff   -lpng  -lfreetype -lSM -lICE -lXrender -lfontconfig -lfreetype -lXext -lX11  \
-L../qt-everywhere-opensource-src-4.8.7/qtbase/lib -lQtXml -lQtGui -L/usr/X11R6/lib -lQtCore -pthread  -lz -lm -ldl -lrt -ljpeg -lmng -ltiff   -lpng  -lfreetype -lSM -lICE -lXrender -lfontconfig -lfreetype -lXext -lX11
windows:QMAKE_LIBS +=  -LC:/Qt/4.8.7/lib -lQtGui -lQtXml -lQtCore -pthread

}


# Input
HEADERS += src/dvda-author-gui.h
SOURCES += src/dvda-author-gui.cpp src/main.cpp src/mainWindow.cpp src/options.cpp
RESOURCES += \
    images/about.png \
    images/application-exit.png \
    images/arrow-down.png \
    images/arrow-up.png \
    images/burn.png \
    images/configure.png \
    images/document-import.png \
    images/document-save.png \
    images/dvda-author.png \
    images/edit-delete.png \
    images/folder-new.png \
    images/help-contents.png \
    images/input.png \
    images/open-project.png \
    images/output.png \
    images/process-stop.png \
    images/retrieve.png \
    images/show-maximized.png \
    images/show-normal.png \
    images/tab-close-other.png \
    images/tab-new.png \
    images/video.png \
    images/view-list-tree.png


DISTFILES += \
    images/about.png \
    images/application-exit.png \
    images/arrow-down.png \
    images/arrow-up.png \
    images/burn.png \
    images/configure.png \
    images/decode.png \
    images/document-import.png \
    images/document-save.png \
    images/dvda-author.png \
    images/edit-delete.png \
    images/folder-new.png \
    images/help-contents.png \
    images/input.png \
    images/open-project.png \
    images/output.png \
    images/process-stop.png \
    images/retrieve.png \
    images/show-maximized.png \
    images/show-normal.png \
    images/tab-close-other.png \
    images/tab-new.png \
    images/video.png \
    images/view-list-tree.png
