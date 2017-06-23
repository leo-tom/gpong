#-------------------------------------------------
#
# Project created by QtCreator 2017-05-11T18:41:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gpong

CONFIG += qt debug

SUBDIRS = lapi/


SOURCES += main.cpp

LIBS += lapi/liblapi.a

