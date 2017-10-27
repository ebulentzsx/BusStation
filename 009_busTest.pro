#-------------------------------------------------
#
# Project created by QtCreator 2017-08-17T22:28:27
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = 009_busTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    HttpFun.cpp \
    mycom.cpp \
    posix_qextserialport.cpp \
    qextserialbase.cpp \
    readcomthread.cpp \
    gethttpreturn.cpp \
    busline.cpp \
    busPublic.cpp \
    mainfunction.cpp \
    secondfunction.cpp \
    devicesetting.cpp

HEADERS += \
    HttpFun.h \
    mycom.h \
    posix_qextserialport.h \
    qextserialbase.h \
    readcomthread.h \
    gethttpreturn.h \
    busline.h \
    busPublic.h \
    mainfunction.h \
    secondfunction.h \
    devicesetting.h
