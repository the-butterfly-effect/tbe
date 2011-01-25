TEMPLATE = app
LANGUAGE = C++
CONFIG += qt \
    warn_on \
    debug
DEFINES += QT_NO_ASCII_CAST
INCLUDEPATH += src \
    src/box2d \
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
    src/box2d/b2BlockAllocator.h \
    src/box2d/b2Body.h \
    src/box2d/b2BroadPhase.h \
    src/box2d/b2CircleContact.h \
    src/box2d/b2CircleShape.h \
    src/box2d/b2Collision.h \
    src/box2d/b2Contact.h \
    src/box2d/b2ContactManager.h \
    src/box2d/b2ContactSolver.h \
    src/box2d/b2Distance.h \
    src/box2d/b2DistanceJoint.h \
    src/box2d/b2DynamicTree.h \
    src/box2d/b2Fixture.h \
    src/box2d/b2FrictionJoint.h \
    src/box2d/b2GearJoint.h \
    src/box2d/b2Island.h \
    src/box2d/b2Joint.h \
    src/box2d/b2LineJoint.h \
    src/box2d/b2Math.h \
    src/box2d/b2MouseJoint.h \
    src/box2d/b2PolygonAndCircleContact.h \
    src/box2d/b2PolygonContact.h \
    src/box2d/b2PolygonShape.h \
    src/box2d/b2PrismaticJoint.h \
    src/box2d/b2PulleyJoint.h \
    src/box2d/b2RevoluteJoint.h \
    src/box2d/b2Settings.h \
    src/box2d/b2Shape.h \
    src/box2d/b2StackAllocator.h \
    src/box2d/b2TimeOfImpact.h \
    src/box2d/b2TimeStep.h \
    src/box2d/b2TOISolver.h \
    src/box2d/b2WeldJoint.h \
    src/box2d/b2WorldCallbacks.h \
    src/box2d/b2World.h \
    src/box2d/Box2D.h \
    src/base/BaseObjectSerializer.h \
    src/base/GoalSerializer.h \
    src/base/LocalString.h \
    src/base/Property.h \
    src/base/UndoDeleteCommand.h \
    src/base/UndoInsertCommand.h \
    src/base/UndoObjectChange.h \
    src/model/AbstractBall.h \
    src/model/BalloonCactus.h \
    src/model/BaseJoint.h \
    src/model/BaseObject.h \
    src/model/Butterfly.h \
    src/model/CokeMentosBottle.h \
    src/model/Floor.h \
    src/model/Glue.h \
    src/model/Goal.h \
    src/model/Level.h \
    src/model/Link.h \
    src/model/PivotPoint.h \
    src/model/PolyObject.h \
    src/model/Position.h \
    src/model/PostIt.h \
    src/model/RatBread.h \
    src/model/RectObject.h \
    src/model/Scenery.h \
    src/model/Spring.h \
    src/model/TranslationGuide.h \
    src/model/TriggerExplosion.h \
    src/model/World.h \
    src/view/Anchors.h \
    src/view/ChooseLevel.h \
    src/view/ChoosePhoneNumber.h \
    src/view/DrawAbstractBall.h \
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
    src/box2d/b2BlockAllocator.cpp \
    src/box2d/b2Body.cpp \
    src/box2d/b2BroadPhase.cpp \
    src/box2d/b2CircleContact.cpp \
    src/box2d/b2CircleShape.cpp \
    src/box2d/b2CollideCircle.cpp \
    src/box2d/b2CollidePolygon.cpp \
    src/box2d/b2Collision.cpp \
    src/box2d/b2Contact.cpp \
    src/box2d/b2ContactManager.cpp \
    src/box2d/b2ContactSolver.cpp \
    src/box2d/b2Distance.cpp \
    src/box2d/b2DistanceJoint.cpp \
    src/box2d/b2DynamicTree.cpp \
    src/box2d/b2Fixture.cpp \
    src/box2d/b2FrictionJoint.cpp \
    src/box2d/b2GearJoint.cpp \
    src/box2d/b2Island.cpp \
    src/box2d/b2Joint.cpp \
    src/box2d/b2LineJoint.cpp \
    src/box2d/b2Math.cpp \
    src/box2d/b2MouseJoint.cpp \
    src/box2d/b2PolygonAndCircleContact.cpp \
    src/box2d/b2PolygonContact.cpp \
    src/box2d/b2PolygonShape.cpp \
    src/box2d/b2PrismaticJoint.cpp \
    src/box2d/b2PulleyJoint.cpp \
    src/box2d/b2RevoluteJoint.cpp \
    src/box2d/b2Settings.cpp \
    src/box2d/b2StackAllocator.cpp \
    src/box2d/b2TimeOfImpact.cpp \
    src/box2d/b2TOISolver.cpp \
    src/box2d/b2WeldJoint.cpp \
    src/box2d/b2WorldCallbacks.cpp \
    src/box2d/b2World.cpp \
    src/base/BaseObjectSerializer.cpp \
    src/base/GoalSerializer.cpp \
    src/base/LocalString.cpp \
    src/base/Property.cpp \
    src/base/UndoDeleteCommand.cpp \
    src/base/UndoInsertCommand.cpp \
    src/base/UndoObjectChange.cpp \
    src/model/AbstractBall.cpp \
    src/model/BalloonCactus.cpp \
    src/model/BaseJoint.cpp \
    src/model/BaseObject.cpp \
    src/model/Butterfly.cpp \
    src/model/CokeMentosBottle.cpp \
    src/model/Floor.cpp \
    src/model/Glue.cpp \
    src/model/Goal.cpp \
    src/model/Level.cpp \
    src/model/Link.cpp \
    src/model/PivotPoint.cpp \
    src/model/PolyObject.cpp \
    src/model/Position.cpp \
    src/model/PostIt.cpp \
    src/model/RatBread.cpp \
    src/model/RectObject.cpp \
    src/model/Scenery.cpp \
    src/model/Spring.cpp \
    src/model/TranslationGuide.cpp \
    src/model/TriggerExplosion.cpp \
    src/model/World.cpp \
    src/view/Anchors.cpp \
    src/view/ChooseLevel.cpp \
    src/view/ChoosePhoneNumber.cpp \
    src/view/DrawAbstractBall.cpp \
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
    src/view/PostItEditor.ui \
    src/view/ChoosePhoneNumber.ui
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
