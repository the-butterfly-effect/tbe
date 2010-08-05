# -------------------------------------------------
# Project created by QtCreator 2009-12-23T12:38:16
# -------------------------------------------------
QT += xml \
    testlib
TARGET = ../../ChooseLevel-tester
CONFIG += qt warn_on debug
TEMPLATE = app
SOURCES += ChooseLevel-tester-main.cpp \
    ../../src/view/ChooseLevel.cpp \
    ../../src/base/LocalString.cpp
HEADERS += ../../src/view/ChooseLevel.h \
    ../../src/tbe_global.h \
    ../../src/base/LocalString.h
FORMS += ../../src/view/ChooseLevel.ui
INCLUDEPATH += \
	../../src \
	../../src/model \
	../../src/view \
	../../src/base
