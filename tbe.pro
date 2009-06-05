TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -lode -L3rdParty/libs

DEFINES	+= QT_NO_ASCII_CAST

INCLUDEPATH	+= 3rdParty/include src src/model src/view src/base

#CXXFLAGS += -ggdb3 -O0

QT += core gui xml svg

HEADERS	+= src/tbe_global.h \
	src/model/BaseObject.h \
	src/model/BowlingBall.h \
	src/model/BowlingPin.h \
	src/model/ImmovableObject.h \
	src/model/Level.h \
	src/model/MovingObject.h \
	src/model/Position.h \
	src/model/Ramp.h \
	src/model/World.h \
	src/view/Popup.h \
	src/view/DrawObject.h \
	src/view/DrawRamp.h \
	src/view/DrawWorld.h \
	src/view/MainWindow.h \
	src/view/SplashScreen.h \
	src/view/StartStopReset.h \
	
	
SOURCES	+= src/main.cpp \
	src/model/BaseObject.cpp \
	src/model/BowlingBall.cpp \
	src/model/BowlingPin.cpp \
	src/model/ImmovableObject.cpp \
	src/model/Level.cpp \
	src/model/MovingObject.cpp \
	src/model/Position.cpp \
	src/model/Ramp.cpp \
	src/model/World.cpp \
	src/view/DrawObject.cpp \
	src/view/DrawRamp.cpp \
	src/view/DrawWorld.cpp \
	src/view/MainWindow.cpp \
	src/view/SplashScreen.cpp \
	src/view/StartStopReset.cpp \
	

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

