QT += xml gui testlib
TARGET = cleanlevel
CONFIG += qt \
    warn_on \
    debug
TEMPLATE = app
DEFINES += QT_NO_ASCII_CAST
QMAKE_CXXFLAGS_DEBUG += "-ggdb3 -Wall -Wextra"
QMAKE_CXXFLAGS += "--std=gnu++0x"

SOURCES += \
    cleanlevels.cpp
HEADERS += \
    ../src/tbe_global.h \
    ../src/loadsave/LocalString.h

INCLUDEPATH += \
    ../src \
    ../src/model \
    ../src/view \
    ../src/loadsave
