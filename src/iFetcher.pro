#-------------------------------------------------
#
# Project created by QtCreator 2010-09-27T20:41:30
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = iFetcher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qtwin.cpp \
    httpget.cpp \
    metadata.cpp \
    download.cpp \
    confirmdownloaddialog.cpp \
    programmeinfo.cpp

HEADERS  += mainwindow.h \
    qtwin.h \
    httpget.h \
    metadata.h \
    download.h \
    confirmdownloaddialog.h \
    programmeinfo.h

FORMS    += mainwindow.ui \
    confirmdownloaddialog.ui
