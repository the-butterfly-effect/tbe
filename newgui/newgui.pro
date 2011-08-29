#-------------------------------------------------
#
# Project created by QtCreator 2011-08-16T22:25:11
#
#-------------------------------------------------

QT       += core gui xml svg

TARGET = tbe
TEMPLATE = app

INCLUDEPATH += . view model

SOURCES += main.cpp\
	model/BaseObject.cpp \
	view/DrawObject.cpp \
	view/DropDownWindow.cpp \
	view/ImageStore.cpp \
	view/MainWindow.cpp \
	view/resizinggraphicsview.cpp

HEADERS  += tbe_global.h \
	model/BaseObject.h \
	view/DrawObject.h \
	view/DropDownWindow.h \
	view/ImageStore.h \
	view/MainWindow.h \
	view/resizinggraphicsview.h

FORMS    += view/MainWindow.ui

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}
