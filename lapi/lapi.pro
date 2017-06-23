#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T10:34:45
#
#-------------------------------------------------

QT       += widgets

TARGET = lapi
TEMPLATE = lib

CONFIG += staticlib
CONFIG += qt debug

DEFINES += LAPI_LIBRARY

SOURCES += \
    soflayer.cpp \
    pictures.cpp \
    mainwindow.cpp \
    maiDrawingArea.cpp \
    main.cpp \
    leoapiphy.cpp \
    leoapiobj.cpp \
    leoapi.cpp \
    sofobject.cpp

HEADERS +=\
    soflayer.h \
    pictures.h \
    pong.h \
    sofobject.h \
    maiDrawingArea.h \
    mainwindow.h \
    leoapi.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    images.qrc

FORMS += \
    mainwindow.ui

DISTFILES += \
    defaultBall.png \
    defaultBack.png \
    defaultORacket.png \
    defaultPRacket.png \
    COPYING
