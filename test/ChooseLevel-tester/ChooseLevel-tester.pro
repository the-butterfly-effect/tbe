# -------------------------------------------------
# Project created by QtCreator 2009-12-23T12:38:16
# -------------------------------------------------
QT += xml \
    xmlpatterns \
    testlib
TARGET = ../../ChooseLevel-tester
CONFIG += qt warn_on debug
TEMPLATE = app
SOURCES += ChooseLevel-tester-main.cpp \
    ../../src/view/ChooseLevel.cpp
HEADERS += ../../src/view/ChooseLevel.h \
    ../../src/tbe_global.h
FORMS += ../../src/view/ChooseLevel.ui
INCLUDEPATH += \
	../../src \
	../../src/model \
	../../src/view \
	../../src/base
