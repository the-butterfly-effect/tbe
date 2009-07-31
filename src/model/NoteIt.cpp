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

#include "NoteIt.h"
#include "tbe_global.h"
#include "Box2D.h"
#include "DrawNoteIt.h"

// this class' ObjectFactory
class NoteItObjectFactory : public ObjectFactory
{
public:
	NoteItObjectFactory(void)
	{	announceObjectType("NoteIt", this); }
	virtual BaseObject* createObject(void) const
	{	return new NoteIt(); }
};
static NoteItObjectFactory theFactory;



NoteIt::NoteIt( )
{
	// Post-its are 3x3 inch (i.e. 8x8 cm)
	// but because they are too small, we will triple each side and make them 22x22 cm...
	const int mySideLength = 0.22;

	setTheWidth(mySideLength);
	setTheHeight(mySideLength);

	// Note that NoteIt doesn't have a physics representation
	// it is only graphics

	DEBUG5("NoteIt::NoteIt done\n");
}


NoteIt::~NoteIt( )
{
	;
}



DrawObject*  NoteIt::createDrawObject(void)
{
	assert(theDrawObjectPtr==NULL);
	theDrawObjectPtr = new DrawNoteIt(this);
	return theDrawObjectPtr;
}
