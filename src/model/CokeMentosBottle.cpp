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

#include "CokeMentosBottle.h"
#include "tbe_global.h"
#include "Box2D.h"


//// this class' ObjectFactory
class CokeMentosObjectFactory : public ObjectFactory
{
public:
	CokeMentosObjectFactory(void)
	{	announceObjectType("Coke+Mentos Bottle", this); }
	virtual BaseObject* createObject(void) const
	{	return new CokeMentosBottle(); }
};
static CokeMentosObjectFactory theCMBottleObjectFactory;


CokeMentosBottle::CokeMentosBottle()
		: RectObject()
{
	setProperty(IMAGE_NAME_STRING, "CokeMentosBottle");
	setProperty(DESCRIPTION_STRING, QObject::tr("This is a prepared Coke bottle with a Mentos in it.\nLook: it blows if triggered!"));
	setTheWidth(0.166);
	setTheHeight(0.501);

	b2PolygonDef* my5PointPinDef = new b2PolygonDef();
	my5PointPinDef->vertexCount = 5;
	my5PointPinDef->vertices[0].Set( 0    ,  0.25);
	my5PointPinDef->vertices[1].Set(-0.083,  0.12);
	my5PointPinDef->vertices[2].Set(-0.075, -0.251);
	my5PointPinDef->vertices[3].Set( 0.075, -0.251);
	my5PointPinDef->vertices[4].Set( 0.083,  0.12);
	// approximation of the initial mass - we'll fix it later on...
	my5PointPinDef->density = 1.6 / (0.166*0.501);
	
	// delete any shapes on the body
	// and create a new shape from the above polygon def
	theShapeList.push_back(my5PointPinDef);
	setTheBounciness(0.3);
	setBottleStatus(UNTRIGGERED);
}


void CokeMentosBottle::setBottleStatus(BottleStatus aNewStat)
{
	switch(aNewStat)
	{
	case UNTRIGGERED:
		theCokeAmount = 1.5;
		break;
	case TRIGGERED:
		break;
	case BLOWING:
		break;
	case EMPTY:
		break;
	}
}

void CokeMentosBottle::updateMass(void)
{
	b2MassData myMass;
	myMass.center = theB2BodyPtr->GetLocalCenter();
	myMass.I      = theB2BodyPtr->GetInertia();
	myMass.mass   = theCokeAmount + 0.1;
	theB2BodyPtr->SetMass(&myMass);

	// test
	myMass.mass = 0;
}
