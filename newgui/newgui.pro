#-------------------------------------------------
#
# Project created by QtCreator 2011-08-16T22:25:11
#
#-------------------------------------------------

QT       += core gui xml svg

TARGET = tbe
TEMPLATE = app

####
# the following are all settings for debugging
# it will give more compiler warnings - which is good
CONFIG += qt \
    warn_on \
    debug
DEFINES += QT_NO_ASCII_CAST

QMAKE_CXXFLAGS_DEBUG += -ggdb3 \
    -O0 \
    -Wextra \
    -Wpointer-arith

!macx {
QMAKE_CXXFLAGS_DEBUG += -Wlogical-op
}
#### end of debugging improvements

INCLUDEPATH += . view model control box2d

include(main.pri)
include(box2d/box2d.pri)
include(control/control.pri)
include(loadsave/loadsave.pri)
include(model/model.pri)
include(view/view.pri)

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}
