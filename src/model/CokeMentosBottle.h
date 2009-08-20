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

#ifndef COKEMENTOSBOTTLE_H
#define COKEMENTOSBOTTLE_H

#include "RectObject.h"

/** this class implements the famous coke+mentos trick:
  * insert a mentos in a coke bottle and it will blow a
  * huge fountain - until the bottle is empty.
  */
class CokeMentosBottle : public RectObject
{
public:
    CokeMentosBottle();

	enum BottleStatus
	{
		UNTRIGGERED,
		TRIGGERED,
		BLOWING,
		EMPTY
	};

	/// returns the Name of the object.
	/// overridden from parent
	virtual const QString getName ( ) const
	{
		return QObject::tr("Coke+Mentos Bottle");
	}

	/// returns true if the object can be rotated by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual bool isRotatable ( ) const
	{	return true;	}

	/// returns whether the object can be resized by the user
	/// FIXME: overridden from RectObject, whereas we should *use* rectobject
	virtual SizeDirections isResizable ( ) const
	{	return NORESIZING;	}


	/// one of the two ways to trigger the blowing:
	void setBottleStatus(BottleStatus aNewStat);

private:
	// Private things

	/// adjusts the object's mass based on theCokeAmount variable
	void updateMass(void);


	// Things from RectObject that need adjustments:

	/// overridden to remove
	virtual void adjustParameters(void)
	{ ; };


private:
	BottleStatus theBottleStatus;

	/// the amount of liquid still in the bottle
	/// 1.5l at start, reducing fast whilst blowing.
	/// unit: kg or l (who cares)
	qreal	theCokeAmount;
};

#endif // COKEMENTOSBOTTLE_H
