QT += xml gui testlib
TARGET = tester
CONFIG += qt \
    warn_on \
    debug
TEMPLATE = app
DEFINES += QT_NO_ASCII_CAST
QMAKE_CXXFLAGS_DEBUG += "-ggdb3 -Wall -Wextra"
QMAKE_CXXFLAGS += "--std=gnu++0x"

SOURCES += ../src/TestFramework.cpp \
    ../src/TestChapter.cpp \
    LocalString-tester-main.cpp \
    ../../src/loadsave/LocalString.cpp \
    ../../src/view/ChooseLevel.cpp
HEADERS += ../src/TestFramework.h \
    ../src/TestChapter.h \
    ../../src/tbe_global.h \
    ../../src/loadsave/LocalString.h \
    ../../src/view/ChooseLevel.h
INCLUDEPATH += ../src \
    ../../3rdParty/Box2D_v2.0.1/Box2D/Include/ \
    ../../src \
    ../../src/model \
    ../../src/view \
    ../../src/loadsave
FORMS += \
    ../../src/view/ChooseLevel.ui
