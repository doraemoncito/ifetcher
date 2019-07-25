#-------------------------------------------------
#
# Project created by QtCreator 2010-09-27T20:41:30
#
#-------------------------------------------------

QT += core widgets gui network xml webengine webenginewidgets

TARGET = iFetcher
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    httpget.cpp \
    metadata.cpp \
    download.cpp \
    confirmdownloaddialog.cpp \
    programmeinfo.cpp

HEADERS  += mainwindow.h \
    httpget.h \
    metadata.h \
    download.h \
    confirmdownloaddialog.h \
    programmeinfo.h

FORMS += mainwindow.ui \
    confirmdownloaddialog.ui
