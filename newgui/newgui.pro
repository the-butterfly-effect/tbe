#-------------------------------------------------
#
# Project created by QtCreator 2011-08-16T22:25:11
#
#-------------------------------------------------

QT       += core gui xml svg

TARGET = tbe
TEMPLATE = app

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
