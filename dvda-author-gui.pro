QT += core gui xml widgets
QMAKE_CXX = /usr/bin/g++-8.3.0
QMAKE_CC = /usr/bin/gcc-8.3.0
QMAKE_LINK = /usr/bin/g++-8.3.0
TEMPLATE = app
TARGET = 
INCLUDEPATH += . ../qt-everywhere-opensource-src-4.8.7/include
DEPENDPATH += .
DEFINES += QT_FILE_DIALOG
QMAKE_CXXFLAGS += -static -static-libgcc -static-libstdc++ -pthread  -O3
QMAKE_LFLAGS += -Wl,-O1 -Wl,-rpath,../qt-everywhere-opensource-src-4.8.7/qtbase/lib
LIBS +=  -L../qt-everywhere-opensource-src-4.8.7/qtbase/lib -lQtXml -lQtGui -L/usr/X11R6/lib -lQtCore -pthread  -lz -lm -ldl -lrt -ljpeg -lmng -ltiff   -lpng  -lfreetype -lSM -lICE -lXrender -lfontconfig -lfreetype -lXext -lX11 -s


# Input
HEADERS += dvda-author-gui.h
SOURCES += dvda-author-gui.cpp main.cpp mainWindow.cpp options.cpp
RESOURCES += dvda-author-gui.qrc
