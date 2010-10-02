/* The Butterfly Effect
 * This file copyright (C) 2009, 2010  Klaas van Gend
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

#include "DrawObject.h"
#include "BaseObject.h"
#include "EditObjectDialog.h"

// forward declarations
class Anchor;
class QGraphicsScene;
class QUndoCommand;
class UndoRCommand;

/// the Anchors class manages the resize/rotate/delete anchors around a selected DrawObject
class Anchors : public QObject
{
	Q_OBJECT

public:
	Anchors(DrawObject* anObjectPtr);
	virtual ~Anchors();

	QGraphicsScene* getScenePtr();

	enum AnchorType
	{
		NONE,
		RESIZEHORI,
		RESIZEVERTI,
		ROTATE,
		DELETE
	};

	void          createUndoDelete(void);
	UndoRCommand* createUndoResize(void);
	UndoRCommand* createUndoRotate(const Vector& aHotspot);

	/// @returns true if anItem is one of the Anchors
	bool isAnchor(QGraphicsItem* anItem);

	bool pushUndo(QUndoCommand* anUndo)
		{ return theDrawObjectPtr->pushUndo(anUndo); }

	/// iterates through all Anchor's to make them update their position
	void updatePosition();

	static EditObjectDialog* getEditObjectDialogPtr(void)
	{ return theObjectDialogPtr; }
	static void clearEditObjectDialogPtr(void)
	{ delete theObjectDialogPtr; theObjectDialogPtr = NULL; }

private:
	DrawObject* theDrawObjectPtr;

	typedef QList<Anchor*> AnchorList;
	AnchorList theAnchorList;

	static EditObjectDialog* theObjectDialogPtr;
};



/// Anchor is a single item used by the Anchors class
class Anchor : public QGraphicsSvgItem
{
	Q_OBJECT

public:
	enum AnchorPosition
	{
		RIGHT       = 0,
		TOPRIGHT    = 1,
		TOP         = 2,
		TOPLEFT     = 3,
		LEFT        = 4,
		BOTTOMLEFT  = 5,
		BOTTOM      = 6,
		BOTTOMRIGHT = 7,
		TOPLEFTLEFT = 8
	};

	/// Constructor
	Anchor(Anchors::AnchorType aDirection, AnchorPosition anIndex, Anchors* aParent);

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
	void updatePosition(Position myC, qreal myW, qreal myH);

private:
	Anchors* theParentPtr;
	Anchors::AnchorType theDirection;
	AnchorPosition theIndex;
	static const int theIconSize;
	qreal theDelta;

	qreal theOffset;
	qreal theOldAngle;

	UndoRCommand* theUndoRPtr;

	int getDX();
	int getDY();
};



#endif // ANCHORS_H
