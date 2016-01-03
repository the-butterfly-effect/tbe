#-------------------------------------------------
#
# Project 
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += --std=gnu++0x

TARGET = level-i18n
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QT   += xml

SOURCES += main.cpp test.cpp

HEADERS += \
	tbe_global.h \
	test.h

 TRANSLATIONS = tbe_nl.ts