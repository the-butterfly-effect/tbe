TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -lode -L3rdParty/libs

DEFINES	+= QT_NO_ASCII_CAST

INCLUDEPATH	+= 3rdParty/include src src/model src/view src/base

#CXXFLAGS += -ggdb3 -O0

HEADERS	+= src/tbe_global.h \
	src/model/BaseObject.h \
	src/model/MovingObject.h \
	src/model/Position.h \
	src/view/Popup.h \
	src/view/MainWindow.h \
	
	
SOURCES	+= src/main.cpp \
	src/model/BaseObject.cpp \
	src/model/MovingObject.cpp \
	src/model/Position.cpp \
	src/view/MainWindow.cpp \
	

FORMS	= \
	src/view/MainWindow.ui \


DISTFILES = \
	README \
	TODO \
	AUTHORS \
	COPYING

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

