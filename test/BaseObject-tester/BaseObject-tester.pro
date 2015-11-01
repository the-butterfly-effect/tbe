QT += xml \
    svg
TARGET = tester
CONFIG += qt \
    warn_on \
    debug
TEMPLATE = app
DEFINES += QT_NO_ASCII_CAST
QMAKE_CXXFLAGS_DEBUG += -ggdb3 \
    -O0 \
    -Wextra \
    -std=c++11
LIBS += -lbox2d \
    -L../../3rdParty/lib
SOURCES += ../src/TestFramework.cpp \
    ../src/TestChapter.cpp \
    BaseObject-tester-main.cpp \
    ../../src/loadsave/AbstractObjectSerializer.cpp \
    ../../src/model/AbstractObject.cpp \
    ../../src/model/Position.cpp \
    ../../src/model/Property.cpp \
    ../src/StubWorld.cpp \
    ../src/StubDrawWorld.cpp \
    ../src/StubDrawObject.cpp \
    ../src/StubPivotPoint.cpp \
    ../src/StubLevel.cpp
HEADERS += ../src/TestFramework.h \
    ../src/TestChapter.h \
    ../../src/tbe_global.h \
    ../../src/loadsave/AbstractObjectSerializer.h \
    ../../src/model/AbstractObject.h \
    ../../src/model/Position.h \
    ../../src/model/Property.h \
    ../src/StubWorld.h \
    ../src/StubDrawWorld.h \
    ../src/StubDrawObject.h \
    ../src/StubPivotPoint.h
INCLUDEPATH += ../src \
    ../../src/Box2D \
    ../../src \
    ../../src/loadsave \
    ../../src/model

