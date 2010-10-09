TEMPLATE = app
LANGUAGE = C++
CONFIG += qt \
    warn_on \
    debug
LIBS += -lbox2d \
    -L3rdParty/lib
DEFINES += QT_NO_ASCII_CAST
INCLUDEPATH += 3rdParty/Box2D_v2.0.1/Box2D/Include/ \
    src \
    src/model \
    src/view \
    src/base
QMAKE_CXXFLAGS_DEBUG += -ggdb3 \
    -O0 \
    -Wextra \
    -Wpointer-arith \
    -Wlogical-op

# -Wfloat-equal -Wshadow -Wcast-qual
QMAKE_CXXFLAGS_RELEASE += -Wextra \
    -DNDEBUG
QT += core \
    gui \
    xml \
    svg
HEADERS += src/tbe_global.h \
    src/base/BaseObjectSerializer.h \
    src/base/GoalSerializer.h \
    src/base/LocalString.h \
    src/base/Property.h \
    src/base/UndoDeleteCommand.h \
    src/base/UndoInsertCommand.h \
    src/base/UndoManualCommand.h \
    src/base/UndoMoveCommand.h \
    src/base/UndoResizeCommand.h \
    src/base/UndoRotateCommand.h \
    src/model/AbstractBall.h \
    src/model/BalloonCactus.h \
    src/model/BaseJoint.h \
    src/model/BaseObject.h \
    src/model/Butterfly.h \
    src/model/CokeMentosBottle.h \
    src/model/Floor.h \
    src/model/Goal.h \
    src/model/Level.h \
    src/model/Link.h \
    src/model/PivotPoint.h \
    src/model/PolyObject.h \
    src/model/Position.h \
    src/model/PostIt.h \
    src/model/RectObject.h \
    src/model/Scenery.h \
    src/model/TranslationGuide.h \
    src/model/World.h \
    src/view/Anchors.h \
    src/view/ChooseLevel.h \
    src/view/DrawAbstractBall.h \
    src/view/DrawButterfly.h \
    src/view/DrawObject.h \
    src/view/DrawPolyObject.h \
    src/view/DrawPostIt.h \
    src/view/DrawWorld.h \
    src/view/GoalEditor.h \
    src/view/EditLevelProperties.h \
    src/view/EditObjectDialog.h \
    src/view/ImageStore.h \
    src/view/LevelInfoDialog.h \
    src/view/MainWindow.h \
    src/view/Popup.h \
    src/view/PostItEditor.h \
    src/view/resizinggraphicsview.h \
    src/view/SaveLevelInfo.h \
    src/view/SplashScreen.h \
    src/view/StartStopWatch.h \
    src/view/toolbox.h
SOURCES += src/main.cpp \
    src/base/BaseObjectSerializer.cpp \
    src/base/GoalSerializer.cpp \
    src/base/LocalString.cpp \
    src/base/Property.cpp \
    src/base/UndoDeleteCommand.cpp \
    src/base/UndoInsertCommand.cpp \
    src/base/UndoManualCommand.cpp \
    src/base/UndoMoveCommand.cpp \
    src/base/UndoResizeCommand.cpp \
    src/base/UndoRotateCommand.cpp \
    src/model/AbstractBall.cpp \
    src/model/BalloonCactus.cpp \
    src/model/BaseJoint.cpp \
    src/model/BaseObject.cpp \
    src/model/Butterfly.cpp \
    src/model/CokeMentosBottle.cpp \
    src/model/Floor.cpp \
    src/model/Goal.cpp \
    src/model/Level.cpp \
    src/model/Link.cpp \
    src/model/PivotPoint.cpp \
    src/model/PolyObject.cpp \
    src/model/Position.cpp \
    src/model/PostIt.cpp \
    src/model/RectObject.cpp \
    src/model/Scenery.cpp \
    src/model/TranslationGuide.cpp \
    src/model/World.cpp \
    src/view/Anchors.cpp \
    src/view/ChooseLevel.cpp \
    src/view/DrawAbstractBall.cpp \
    src/view/DrawButterfly.cpp \
    src/view/DrawObject.cpp \
    src/view/DrawPolyObject.cpp \
    src/view/DrawPostIt.cpp \
    src/view/DrawWorld.cpp \
    src/view/EditLevelProperties.cpp \
    src/view/EditObjectDialog.cpp \
    src/view/GoalEditor.cpp \
    src/view/ImageStore.cpp \
    src/view/LevelInfoDialog.cpp \
    src/view/MainWindow.cpp \
    src/view/PostItEditor.cpp \
    src/view/resizinggraphicsview.cpp \
    src/view/SaveLevelInfo.cpp \
    src/view/SplashScreen.cpp \
    src/view/StartStopWatch.cpp \
    src/view/toolbox.cpp
FORMS = src/view/MainWindow.ui \
    src/view/PostItViewer.ui \
    src/view/SaveLevelInfo.ui \
    src/view/ChooseLevel.ui \
    src/view/EditLevelProperties.ui \
    src/view/EditObjectDialog.ui \
    src/view/LevelInfoDialog.ui \
    src/view/GoalEditor.ui \
    src/view/PostItEditor.ui
DISTFILES = README \
    AUTHORS \
    COPYING
unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
TRANSLATIONS = i18n/tbe_nl.ts \
    i18n/tbe_es.ts
RESOURCES += images/illustrations/tbe-icon.qrc
