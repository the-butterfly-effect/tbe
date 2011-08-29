#-------------------------------------------------
#
# Project created by QtCreator 2011-08-16T22:25:11
#
#-------------------------------------------------

QT       += core gui xml svg

TARGET = tbe
TEMPLATE = app

INCLUDEPATH += . view

SOURCES += main.cpp\
        view/MainWindow.cpp \
    view/ImageStore.cpp \
    view/resizinggraphicsview.cpp \
    view/DropDownWindow.cpp

HEADERS  += view/MainWindow.h \
    view/ImageStore.h \
    tbe_global.h \
    view/resizinggraphicsview.h \
    view/DropDownWindow.h

FORMS    += view/MainWindow.ui

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
}
