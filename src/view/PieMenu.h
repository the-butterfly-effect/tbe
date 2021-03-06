/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef PIEMENU_H
#define PIEMENU_H

#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QStateMachine>
#include <QGraphicsEffect>
#include <QPointer>


#include "tbe_global.h"
#include "AbstractObjectPtr.h"

// forward declarations
class ViewObject;

/// This direct QState derivative was created to have meaningful
/// debug messages when SimulationControls switches state
class NamedState : public QState
{
    Q_OBJECT
public:
    explicit NamedState(QState *aParentPtr = 0, const QString &aStateName = "")
        : QState(aParentPtr), theName(aStateName) {}

    virtual ~NamedState()
    {}

protected:
    virtual void onEntry ( QEvent *event );

private:
    QString theName;
};



/** this class handles the icon, its movement and its actions as part of
  * the pie menu mechanism
  */
class ActionIcon : public QObject, QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

public:
    /// this enum identifies what type of action to create. By accident,
    /// each ActionType also corresponds to 45 degrees rotation around the
    /// center of our pie menu
    enum ActionType {
// no action on position 0 yet
        ACTION_ROTATE = 1,
        ACTION_RESIZE = 2,
        ACTION_MOVE = 3,
        ACTION_DELETE = 4,
        ACTION_EDITPROPERTIES = 5,
//      ACTION_EDITSPECIAL = 6,
        ACTION_SETPHONE = 7,
        ACTION_INSERT = 32,
    };

private:

    explicit ActionIcon(ActionType anActionType,
                        const QString &aFileName,
                        bool  isEnabled,
                        QGraphicsWidget *aParentPtr = nullptr);

    /// @returns the type of this icon
    ActionType getActionType()
    {
        return theActionType;
    }

protected:
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *)
    {
        DEBUG1ENTRY;
        emit clicked(this);
    }

signals:
    void clicked(ActionIcon *);

    void moveBack(void);

private:
    QStateMachine theIconStateMachine;

    ActionType theActionType;

    friend class PieMenu;

    QPixmap theSmallPixmap;
    QPixmap theCenterPixmap;

    // no copy constructor or assignment operators here!
    Q_DISABLE_COPY ( ActionIcon )
};


/// The PieMenu class plays intermediate between all ActionIcons and the
/// ViewObject. Only PieMenuSingleton can create PieMenus.
class PieMenu : public QGraphicsWidget
{
    Q_OBJECT

signals:
    void moveToPositions( void );

private slots:
    void startMove(void)
    {
        emit moveToPositions();
    }

    void iconClicked(ActionIcon *anIconPtr);

private:
    ActionIcon *theCurrentInnerIconPtr;
    AbstractObjectPtr theAOPtr;
    ViewObjectPtr theVOPtr;

    /// Set to true by iconClicked, monitored by
    /// PieMenuSingleton::startClickCheck() and endClickCheck()
    bool wasIconClicked;

    qreal theVOsOriginalZValue;

    /// private constructor
    /// @param aParentPtr pointer to the ViewObject to stick a PieMenu on
    /// @param aPositionInObjectCoord position of the mouse click on the
    ///        object in item coordinates
    explicit PieMenu(ViewObjectPtr aParentPtr);

    virtual ~PieMenu();

    void setup();

    // no copy constructor or assignment operators here!
    Q_DISABLE_COPY ( PieMenu )

    friend class PieMenuSingleton;
};


/// This is the PieMenu chef - it's the only one who can create or delete
/// PieMenus and will ensure there's only one (or none).
/// @note this is a singleton and thus accessible anywhere anytime, just call
///       as PieMenu::addPieMenuToViewObject()...
class PieMenuSingleton
{
public:
    /// @returns a pointer to the ViewObject currently owning a Pie menu
    static ViewObjectPtr getPieMenuParent(void);

    /// Puts a PieMenu on top of the selected ViewObject.
    /// @param aViewObjectPtr pointer to the ViewObject to stick a PieMenu
    ///        on or nullptr if you don't want a PieMenu - you can also call
    ///        clearPieMenu() in that case.
    /// @param aPositionInObjectCoord position of the mouse click on the
    ///        object in *scene* coordinates
    static void addPieMenuToViewObject(ViewObjectPtr aViewObjectPtr,
                                       QPointF aPositionInSceneCoord);

    static void clearPieMenu(void)
    {
        addPieMenuToViewObject(ViewObjectPtr(nullptr), QPointF(0, 0));
    }

    /// Sets up all state to check if an ActionIcon will be clicked.
    /// - see ViewWorld::mousePressEvent()
    static void startClickCheck();
    /// Checks the state to see if an ActionIcon was clicked
    /// and clears the pie menu if something else was clicked.
    /// - see ViewWorld::mousePressEvent()
    static void endClickCheck();

    /// This member is called whenever the ResizingGraphicsView resizes.
    /// It sets the view rect in scene coordinates, so we know what would move
    /// outside the screen.
    /// It's used to ensure that the PieMenu is shown fully.
    static void setViewInSceneCoords(const QPolygonF &aViewRect);

private:
    /// private constructor - this is a singleton class!
    explicit PieMenuSingleton(void);

    /// @returns pointer to the existing singleton.
    static PieMenuSingleton *me(void);

    /// pointer to the currently existing PieMenu (if any)
    QPointer<PieMenu> theCurrentPieMenuPtr;

    // no copy constructor or assignment operators here!
    Q_DISABLE_COPY ( PieMenuSingleton )
};

#endif // PIEMENU_H
