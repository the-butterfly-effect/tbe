/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef PIEMENU_H
#define PIEMENU_H

#include <QtSvg/QGraphicsSvgItem>
#include <QtGui/QGraphicsWidget>
#include <QtCore/QStateMachine>
#include <QtGui/QGraphicsEffect>

#include "tbe_global.h"

// forward declarations
class ViewObject;
class AbstractObject;

/// This direct QState derivative was created to have meaningful
/// debug messages when SimulationControls switches state
class NamedState : public QState
{
	Q_OBJECT
public:
	explicit NamedState(QState* parent = 0, const QString& aStateName = "")
		: QState(parent), theName(aStateName) {}

	virtual ~NamedState()
	{}

protected:
	virtual void onEntry ( QEvent * event );

private:
	QString theName;
};



/** this class handles the icon, its movement and its actions as part of
  * the pie menu mechanism
  */
class ActionIcon : public QGraphicsSvgItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
	/// this enum identifies what type of action to create. By accident,
	/// each ActionType also corresponds to 45 degrees rotation around the
	/// center of our pie menu
	enum ActionType
	{
// no action on position 0 yet
		ACTION_ROTATE = 1,
		ACTION_RESIZE = 2,
		ACTION_MOVE = 3,
		ACTION_DELETE = 4,
//		ACTION_EDITPROPERTIES,
		ACTION_EDITSPECIAL = 6,
//		ACTION_SETPHONE
		ACTION_INSERT = 32,
	};

private:

	ActionIcon(ActionType anActionType,
			   const QString & aFileName,
			   bool  isEnabled,
			   QGraphicsWidget* aParentPtr = NULL);

	/// @returns the type of this icon
	ActionType getActionType()
	{ return theActionType; }

protected:
	virtual void mousePressEvent (QGraphicsSceneMouseEvent*)
	{ DEBUG1ENTRY; emit clicked(this);}

signals:
	void clicked(ActionIcon*);

	void moveBack(void);

private:
	QStateMachine theIconStateMachine;

	ActionType theActionType;

	friend class PieMenu;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( ActionIcon )
};


/// The PieMenu class plays intermediate between all ActionIcons and the
/// ViewObject. Only PieMenuSingleton can create PieMenus.
class PieMenu : public QGraphicsWidget
{
	Q_OBJECT

public:
	static const qreal theRadius = 40.0;

signals:
	void moveToPositions( void );

private slots:
	void startMove(void)
	{ emit moveToPositions(); }

	void iconClicked(ActionIcon* anIconPtr);

private:
	ActionIcon* theCurrentInnerIconPtr;
	AbstractObject* theAOPtr;
	ViewObject* theVOPtr;

	/// private constructor
	/// @param aParentPtr pointer to the ViewObject to stick a PieMenu on
	/// @param aPositionInObjectCoord position of the mouse click on the
	///        object in item coordinates
	PieMenu(ViewObject* aParentPtr,
			const QPointF& aPositionInObjectCoord);

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
	static ViewObject* getPieMenuParent(void);

	/// Puts a PieMenu on top of the selected ViewObject.
	/// @param aViewObjectPtr pointer to the ViewObject to stick a PieMenu
	///        on or NULL if you don't want a PieMenu - you can also call
	///        clearPieMenu() in that case.
	/// @param aPositionInObjectCoord position of the mouse click on the
	///        object in item coordinates
	static void addPieMenuToViewObject(ViewObject* aViewObjectPtr,
									   const QPointF& aPositionInObjectCoord);

	static void clearPieMenu(void)
	{ addPieMenuToViewObject(NULL, QPointF(0,0)); }

private:
	/// private constructor - this is a singleton class!
	explicit PieMenuSingleton(void);

	/// @returns pointer to the existing singleton.
	static PieMenuSingleton* me(void);

	/// pointer to the currently existing PieMenu (if any)
	PieMenu* theCurrentPieMenuPtr;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( PieMenuSingleton )
};

#endif // PIEMENU_H
