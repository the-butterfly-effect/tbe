TEMPLATE = app
LANGUAGE = C++
QT += xml svg testlib
CONFIG += qt \
    warn_on \
    debug
DEFINES += QT_NO_ASCII_CAST
QMAKE_CXXFLAGS_DEBUG += -ggdb3 \
    -O0 \
	-Wextra \
	-DTESTCODE
SOURCES += \
    ImageRendererStore-tester.cpp \
    ../../src/view/ImageStore.cpp
HEADERS += \
    ../../src/tbe_global.h \
    ../../src/view/ImageStore.h
INCLUDEPATH += \
    ../../src \
    ../../src/model \
    ../../src/view \
    ../../src/base

# for code coverage
QMAKE_CXXFLAGS_DEBUG += -ftest-coverage -fprofile-arcs
QMAKE_CFLAGS_DEBUG   += -ftest-coverage -fprofile-arcs
QMAKE_LFLAGS_DEBUG   += -fprofile-arcs
