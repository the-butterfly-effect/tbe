#ifndef GAMERESOURCES_H
#define GAMERESOURCES_H

#include <QStateMachine>
#include <QWidget>

namespace Ui {
    class GameResources;
}

class AbstractObject;
class Level;
#include "LocalString.h"
class QMenuBar;
class QGraphicsScene;
#include "resizinggraphicsview.h"
#include "ToolboxGroup.h"
class ViewWorld;


/// This class holds the toolbox, the game info and (in the future)
/// the button for hints. It is displayed when you click on the appropriate
/// action in the menu bar and/or at the start of a level.
///
/// Usually this class is owned by the resizingsgraphicsview of MainWindow,
/// which this class is drawn on top of (like an overlay).
class GameResources : public QWidget
{
    Q_OBJECT

public:
    explicit GameResources(ResizingGraphicsView* aRSGVPtr);
    ~GameResources();

    /// Adds the object pointed to, to the Toolbox.
    /// If there is already an ToolboxGroup with the same name,
    /// it is added to the same group, otherwise a new group is made.
    /// @param anObjectGroupName name of the objects to group this object to.
    /// @param anAOPtr pointer to AbstractObject to add
    void addAbstractObjectToToolbox(const LocalString& anObjectGroupName,
                                    AbstractObject* anAOPtr);

    /// Called by the ResizingGraphicsView so we can accomodate for
    /// the change in scale of the toolboxview.
    /// @param aTransformMatrix the transform matrix used by the parent.
    void parentResize(const QTransform& aTransformMatrix);

    // TODO/FIXME: I don't like this
    void setLevelPtr(Level* aLevelPtr);
    // TODO/FIXME: I'd much rather implement these:
//    void setLevelName(const LocalString& aName);
//    void setLevelDescription(const LocalString& aDescription);

    void setup(QMenuBar* aMenuBarPtr);


    void updateToolbox(void);

protected:
    /// ill-named standard Qt member to change the i18n of the dialog
    virtual void changeEvent(QEvent *e);

private:
    Ui::GameResources *ui;

    typedef QMap<LocalString, ToolboxGroup*> ToolboxGroupList;
    ToolboxGroupList theToolboxList;

    QGraphicsScene* theToolboxPtr;

    /// pointer to Level instance, which contains level names and such
    Level* theLevelPtr;

    /// pointer to the ViewWorld i.e. the main scene of the game
    ViewWorld* theViewWorldPtr;

    /// GraphicsView holding the ViewWorld, we are an overlay to that View
    ResizingGraphicsView* theParentPtr;

	/// used for the animation
	QStateMachine theStateMachine;

};

#endif // GAMERESOURCES_H
