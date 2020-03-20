#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T17:03:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimilarImageSearch
TEMPLATE = app

LIBS         += -lexiv2 -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

SOURCES += main.cpp\
        mainwindow.cpp \
    addfiles.cpp \
    fileinfo.cpp \
    imageviewer.cpp \
    searchwindow.cpp \
    comparators/imagecomparator.cpp \
    comparators/metacomparator.cpp \
    comparators/contentcomparator.cpp \
    searchimages.cpp \
    matcachegenerator.cpp \
    settings.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    addfiles.h \
    fileinfo.h \
    imageviewer.h \
    searchwindow.h \
    queue.h \
    comparators/imagecomparator.h \
    comparators/metacomparator.h \
    comparators/contentcomparator.h \
    searchimages.h \
    matcachegenerator.h \
    settings.h \
    fastpairqueue.h \
    settingswindow.h

FORMS    += mainwindow.ui \
    imageviewer.ui \
    searchwindow.ui \
    invalidfiles.ui \
    settingswindow.ui

RESOURCES += \
    files.qrc
