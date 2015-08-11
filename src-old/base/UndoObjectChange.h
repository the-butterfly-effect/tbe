/* The Butterfly Effect
 * This file copyright (C) 2010  Klaas van Gend
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

#ifndef UNDOOBJECTCHANGE_H
#define UNDOOBJECTCHANGE_H

#include "BaseObject.h"

#include <QUndoCommand>

/** class UndoObjectChange
  * The basic activator/ de/reactivator for changes to objects
  *
  * Lifetime considerations - note that these are handled OUTSIDE this class,
  * only the commit() member is within the class.
  *   * mouse move (hotspot on object) - user clicks object and moves it around.
  *     Once mousebutton is released, check if object is ok. if so: commit
  *     if not: (go to last known good or revert completely) & commit.
  *   * mouse resize (anchors) - user clicks anchor and moves it around.
  *     Once mouse button is released, check if object is ok. if so: commit
  *     if not: (go to last known good or revert completely) & commit.
  *   * mouse rotate (anchors) - user clicks anchor and moves it around.
  *     Once mouse button is released, check if object is ok. if so: commit
  *     if not: (go to last known good or revert completely) & commit.
  *   * position/size changes in EditObjectDialog
  *     User clicks on any of the spin controls. As long as the object
  *     remains selected, the EditObjectDialog will use a single Undo.
  *     Upon unfocus of a spin control, commit. No checks for validity
  *     needed, we are in the Level Creator anyway.
  *   * property changes in EditObjectDialog (ticket:148)
  *     After any change of a property, commit.
  */
class UndoObjectChange : public QUndoCommand
{
private:

	// All Constructors are private - use the
	// "factory method" CreateUndoObject below, please

	/// Constructor
	UndoObjectChange (BaseObject* aBOPtr);

	/// kill possibility for copy constructor - do not implement
	UndoObjectChange(const UndoObjectChange&);

	/// kill possibility for assignment operator - do not implement
	const UndoObjectChange& operator=(const UndoObjectChange&);

public:
	/**
	 * Empty (public) Destructor
	 */
	virtual ~UndoObjectChange ( );

public:
	/** Factory Method
	 * @return UndoObjectChange*
	 * @param  aBOPtr		pointer to a base object
	 */
	static UndoObjectChange* createUndoObject (BaseObject* aBOPtr);


	const Position& getOldPosition(void) const
	{ return theOldCenter; }

	const Vector&   getOldSize(void) const
	{ return theOldSize; }

	/**
	 * @return bool
	 */
	bool isChanged ( ) const;

	/// @returns true if the Object is currently not colliding
	bool isGood(void) const
	{ return isNowColliding == false; }

	/** commit the changes to the QUndoStack (static var in DrawObject.cpp)
	  *  - if and only if there are changes
	  *  - and we'll revert to last known good if we're wrong.
	  * @returns true if actually pushed - you should delete the object if false
	  *    is returned
	  */
	bool pushYourself(void);

	/** (only useful in step-by-step changes)
	  * return to the last known good state
	  *
	  * Automatically calls redo() to effectuate the changes.
	  */
	void revertToLastGood(void);


	/** Update the Undo with new position/size data.
	  * If in the new state the object is colliding, "last good" is
	  * not updated.
	  *
	  * TODO: isn't it better to just supply the new parameters here
	  * and have this member update the DrawObject/Anchors/BaseObject/etc ???
	  *
	  * Note:
	  *   Will update the BaseObject/DrawObject and trigger a redraw
	  *   Will trigger update of EditObjectDialog
	  *   Will trigger update of Anchors
	  */
	void update(const Position& aNewPos, const Vector& aNewSize);

	/// see full-featured update, will only update the angle, though.
	void update(qreal anAngle);

	/// see full-featured update, will only update a property, though
	void update(const QString& aKey, const QString& aValue);

	/// see full-featured update, will only update the ID, though
	void update(const QString& anID);

	/// see full-featured update, will only update the Position x and y, though
	void update(const Vector& aNewPos);

	/// implemented from QUndoCommand, set the object to "new" state
	/// this will trigger a redraw for all affected areas
	virtual void redo ();

	/// implemented from QUndoCommand, set the object to "old" state
	/// this will trigger a redraw for all affected areas
	virtual void undo ();

protected:
	/// invalidate both the AABB of the old and new positions (including
	/// Anchors) in DrawWorld
	void requestSceneRefresh(void);

	/// @returns a (translated) description of the change in the object
	///	or empty string if none...
	QString getChangeString(void) const;

private:
	BaseObject* theBaseObjectPtr;

	Position	theOldCenter;
	Position	theLastGoodCenter;
	Position	theNewCenter;

	PropertyList theOldProperties;
	PropertyList theNewProperties;

	Vector      theOldSize;
	Vector      theLastGoodSize;
	Vector      theNewSize;

	QString		theOldID;
	QString		theNewID;

	bool isNowColliding;
};

#endif // UNDOOBJECTCHANGE_H
