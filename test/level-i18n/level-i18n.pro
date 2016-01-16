#-------------------------------------------------
#
# Project 
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += --std=gnu++0x

TARGET = level-i18n
CONFIG   += console debug
CONFIG   -= app_bundle

TEMPLATE = app

QT   += xml

SOURCES += main.cpp test.cpp \
    Translator.cpp

HEADERS += \
	tbe_global.h \
	test.h \
    Translator.h

 TRANSLATIONS = tbe_nl.ts
