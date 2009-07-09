/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#ifndef ANCHORS_H
#define ANCHORS_H

#include <QGraphicsItem>
#include <QGraphicsSvgItem>

#include "PieMenu.h"
#include "DrawObject.h"
#include "BaseObject.h"
#include "UndoResizeCommand.h"

// forward declarations
class Anchor;
class QGraphicsScene;
class QUndoCommand;

/// the Anchors class manages the resize/rotate anchors around a selected DrawObject
class Anchors : public QObject
{
	Q_OBJECT

public:
	Anchors(DrawObject* anObjectPtr);
	virtual ~Anchors();

	QGraphicsScene* getScenePtr();

	enum HPosition
	{
		LEFT=-1,
		HMIDDLE=0,
		RIGHT=1
	};
	enum VPosition
	{
		TOP=-1,
		VMIDDLE=0,
		BOTTOM=1
	};

	/// @returns the width of the DrawObject we're drawing anchors around
	qreal getWidth(void)
		{ return theDrawObjectPtr->getBaseObjectPtr()->getTheWidth(); }

	/// @returns the height of the DrawObject we're drawing anchors around
	qreal getHeight(void)
		{ return theDrawObjectPtr->getBaseObjectPtr()->getTheHeight(); }

	/// @returns the center Position of the DrawObject we're drawing anchors around
	Position getCenter(void)
		{ return theDrawObjectPtr->getBaseObjectPtr()->getOrigCenter(); }

	UndoResizeCommand* createUndoResize(void)
		{ return new UndoResizeCommand(theDrawObjectPtr, theDrawObjectPtr->getBaseObjectPtr()); }

	bool pushUndo(QUndoCommand* anUndo)
		{ return theDrawObjectPtr->pushUndo(anUndo); }

	/// iterates through all Anchor's to make them update their position
	void updatePosition();

private:
	DrawObject* theDrawObjectPtr;

	typedef QList<Anchor*> AnchorList;
	AnchorList theAnchorList;
};



/// Anchor is a single item used by the Anchors class
class Anchor : public QGraphicsSvgItem
{
	Q_OBJECT

public:
	/// Constructor
	Anchor(PieMenu::EditMode aDirection, Anchors::HPosition anHPos, Anchors::VPosition aVPos, Anchors* aParent);

public slots:
	/** overridden from QGraphicsItem
	 *  if the user drags the object around, this even will be called for each pixel.
	 *  let's actually adjust the coordinates!!!
	 *
	 *  @param event the even to handle
	 */
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

	/** overridden from QGraphicsItem
	 *  we want to know when the user clicks on this Anchor
	 *
	 *  @param event the even to handle
	 */
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

	/** overridden from QGraphicsItem
	 *  we want to know when the user clicks on this Anchor
	 *
	 *  @param event the even to handle
	 */
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

	/// slot that tells the position may have changed - called by the Anchors class.
	void updatePosition(void);

private:
	Anchors* theParentPtr;
	PieMenu::EditMode theDirection;
	Anchors::HPosition theHPos;
	Anchors::VPosition theVPos;
	static const int theIconSize = 16;
	qreal theDelta;

	qreal theOffset;
	QPointF thePrevMousePos;

	UndoResizeCommand* theUndoPtr;
};



#endif // ANCHORS_H
