QT += xml
TARGET = tester
CONFIG += qt \
    warn_on \
    debug
TEMPLATE = app
DEFINES += QT_NO_ASCII_CAST
QMAKE_CXXFLAGS_DEBUG += -ggdb3 \
    -O0 \
    -Wextra
LIBS += -lbox2d \
    -L../../3rdParty/lib
SOURCES += ../src/TestFramework.cpp \
    ../src/TestChapter.cpp \
    LocalString-tester-main.cpp \
    ../../src/loadsave/LocalString.cpp
HEADERS += ../src/TestFramework.h \
    ../src/TestChapter.h \
    ../../src/tbe_global.h \
    ../../src/loadsave/LocalString.h
INCLUDEPATH += ../src \
    ../../3rdParty/Box2D_v2.0.1/Box2D/Include/ \
    ../../src \
    ../../src/model \
    ../../src/view \
    ../../src/loadsave
