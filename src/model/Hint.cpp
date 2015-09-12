/* The Butterfly Effect
 * This file copyright (C) 2015  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "tbe_global.h"
#include "AbstractObject.h"
#include "Hint.h"
#include "Position.h"
#include "Property.h"
#include "World.h"

#include <QStringList>

const char* Hint::WIDTH_STRING="width";
const char* Hint::HEIGHT_STRING="height";
const char* Hint::ANGLE_STRING="angle";
const char* Hint::XPOS_STRING="X";
const char* Hint::YPOS_STRING="Y";


Hint::Hint() : theHintIndex(0), theObjectName("-")
{
	// nothing to do here
}

Hint::~Hint()
{
	// nothing to do here
}

bool Hint::updateFromHint(qreal &aFloat, const char *aString)
{
	bool myOK = true;
	if (theParams.doesPropertyExists(aString))
	{
		aFloat = theParams.getPropertyNoDefault(aString).toFloat(&myOK);
		if (myOK)
			DEBUG1("Read updated property %s: %f", aString, aFloat);
	}
	return myOK;
}

