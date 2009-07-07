TEMPLATE = app
LANGUAGE = C++
CONFIG += qt \
    warn_on \
    release
LIBS += -lbox2d \
    -L3rdParty/libs
DEFINES += QT_NO_ASCII_CAST
INCLUDEPATH += 3rdParty/Box2D_v2.0.1/Box2D/Include/ \
    src \
    src/model \
    src/view \
    src/base

# CXXFLAGS += -ggdb3 -O0
QT += core \
    gui \
    xml \
    svg
HEADERS += src/tbe_global.h \
    src/base/UndoDeleteCommand.h \
    src/base/UndoMoveCommand.h \
    src/model/BaseObject.h \
    src/model/BowlingBall.h \
    src/model/BowlingPin.h \
    src/model/Floor.h \
    src/model/Level.h \
    src/model/MovingObject.h \
    src/model/Position.h \
    src/model/Ramp.h \
    src/model/World.h \
    src/view/Anchors.h \
    src/view/DrawObject.h \
    src/view/DrawRamp.h \
    src/view/DrawWorld.h \
    src/view/ImageStore.h \
    src/view/MainWindow.h \
    src/view/PieMenu.h \
    src/view/Popup.h \
    src/view/resizinggraphicsview.h \
    src/view/SplashScreen.h \
    src/view/StartStopReset.h \
    src/model/VolleyBall.h \
    src/base/UndoResizeCommand.h \
    src/model/ToolBoxItemListModel.h
NOT_USED_HEADERS += src/model/ImmovableObject.h \
    src/model/Wall.h
SOURCES += src/main.cpp \
    src/base/UndoDeleteCommand.cpp \
    src/base/UndoMoveCommand.cpp \
    src/model/BaseObject.cpp \
    src/model/BowlingBall.cpp \
    src/model/BowlingPin.cpp \
    src/model/Floor.cpp \
    src/model/Level.cpp \
    src/model/MovingObject.cpp \
    src/model/Position.cpp \
    src/model/Ramp.cpp \
    src/model/VolleyBall.cpp \
    src/model/World.cpp \
    src/view/Anchors.cpp \
    src/view/DrawObject.cpp \
    src/view/DrawRamp.cpp \
    src/view/DrawWorld.cpp \
    src/view/ImageStore.cpp \
    src/view/MainWindow.cpp \
    src/view/PieMenu.cpp \
    src/view/SplashScreen.cpp \
    src/view/StartStopReset.cpp \
    src/base/UndoResizeCommand.cpp \
    src/model/ToolBoxItemListModel.cpp
NOT_USED_SOURCE = src/model/ImmovableObject.cpp \
    src/model/Wall.cpp
FORMS = src/view/MainWindow.ui
DISTFILES = README \
    TODO \
    AUTHORS \
    COPYING
unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
OTHER_FILES += TODO
TRANSLATIONS = i18n/tbe_nl.ts
