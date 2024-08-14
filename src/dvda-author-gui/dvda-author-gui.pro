QT += core gui xml widgets
TEMPLATE = app

DEPENDPATH += .
DEFINES += QT_FILE_DIALOG

DEFINES += LOCAL WITHOUT_STARTSECTOR


DEFINES += VERSION=\\\"24.08\\\"

QMAKE_CXXFLAGS += -static -static-libgcc -static-libstdc++ -pthread  -O3

equals(QT_MAJOR_VERSION, 6) {

TARGET = dvda-author-gui-qt6

QMAKE_LFLAGS += -Wl,-O2 -L/usr/local/lib
QMAKE_LIBS +=    -pthread
}


# Input
HEADERS += \
    src/dvda-author-gui.h
SOURCES += \
    src/dvda-author-gui.cpp \
    src/main.cpp \
    src/mainWindow.cpp \
    src/options.cpp
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
    images/player_play.png \
    images/player_stop.png \
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
    images/player_play.png \
    images/player_stop.png \
    images/process-stop.png \
    images/retrieve.png \
    images/show-maximized.png \
    images/show-normal.png \
    images/tab-close-other.png \
    images/tab-new.png \
    images/video.png \
    images/view-list-tree.png
