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

#include <QGraphicsSvgItem>
#include <QGraphicsWidget>
#include <QStateMachine>

// forward declarations
class ViewObject;


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

private:
	/// this enum identifies what type of action to create. By accident,
	/// each ActionType also corresponds to 45 degrees rotation around the
	/// center of our pie menu
	enum ActionType
	{
		ACTION_HRESIZE = 0,
		ACTION_ROTATE = 1,
//		ACTION_VRESIZE = 2,
		ACTION_MOVE = 3,
		ACTION_DELETE = 5,
//		ACTION_EDITPROPERTIES,
//		ACTION_EDITSPECIAL,
//		ACTION_SETPHONE
	};

	ActionIcon(ActionType anActionType,
			   const QString & aFileName,
			   bool  isEnabled,
			   QGraphicsWidget* aParentPtr = NULL);

protected:
	virtual void mousePressEvent (QGraphicsSceneMouseEvent*)
	{ emit clicked(this);}

signals:
	void clicked(ActionIcon*);

	void moveBack(void);

private:
	QStateMachine theIconStateMachine;

	friend class PieMenu;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( ActionIcon )
};


class PieMenu : public QGraphicsWidget
{
	Q_OBJECT

public:

	PieMenu(ViewObject* aParentPtr);

	void setup();

signals:
	void moveToPositions( void );

private slots:
	void startMove(void)
	{ emit moveToPositions(); }

	void iconClicked(ActionIcon* anIconPtr);

private:
	ActionIcon* theCurrentInnerIconPtr;

	// no copy constructor or assignment operators here!
	Q_DISABLE_COPY ( PieMenu )
};

#endif // PIEMENU_H
