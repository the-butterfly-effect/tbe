# -------------------------------------------------
# Project created by QtCreator 2009-12-23T12:38:16
# -------------------------------------------------
QT += xml \
    testlib
TARGET = tester
CONFIG += qt warn_on debug
QMAKE_CXXFLAGS += "--std=gnu++0x"
TEMPLATE = app
SOURCES += ChooseLevel-tester-main.cpp \
    ../../src/view/animateddialog.cpp \
    ../../src/view/ChooseLevel.cpp \
    ../../src/loadsave/LocalString.cpp
HEADERS += ../../src/view/ChooseLevel.h \
    ../../src/tbe_global.h \
    ../../src/loadsave/LocalString.h \
    ../../src/view/animateddialog.h
FORMS += ../../src/view/ChooseLevel.ui
INCLUDEPATH += \
	../../src \
	../../src/model \
	../../src/view \
	../../src/loadsave
