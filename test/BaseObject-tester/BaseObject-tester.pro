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
    -Wextra
LIBS += -lbox2d \
    -L../../3rdParty/lib
SOURCES += ../src/TestFramework.cpp \
    ../src/TestChapter.cpp \
    BaseObject-tester-main.cpp \
    ../../src/base/BaseObjectSerializer.cpp \
    ../../src/base/Property.cpp \
    ../../src/model/BaseObject.cpp \
    ../../src/model/Position.cpp \
    ../src/StubWorld.cpp \
    ../src/StubDrawWorld.cpp \
    ../src/StubDrawObject.cpp \
    ../src/StubPivotPoint.cpp \
    ../src/StubLevel.cpp
HEADERS += ../src/TestFramework.h \
    ../src/TestChapter.h \
    ../../src/tbe_global.h \
    ../../src/base/BaseObjectSerializer.h \
    ../../src/base/Property.h \
    ../../src/model/BaseObject.h \
    ../../src/model/Position.h \
    ../src/StubWorld.h \
    ../src/StubDrawWorld.h \
    ../src/StubDrawObject.h \
    ../src/StubPivotPoint.h
INCLUDEPATH += ../src \
    ../../3rdParty/Box2D_v2.0.1/Box2D/Include/ \
    ../../src \
    ../../src/base
