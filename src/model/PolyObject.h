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

#ifndef POLYOBJECT_H
#define POLYOBJECT_H

#include "AbstractObject.h"
#include "ObjectFactory.h"
/**
  * class PolyObject
  *
  * This class defines an object consisting of one or more convex polygons.
  * It can be resized, as long as all the polygons reach a minimum area.
  * It can have a mass, but it doesn't need to: If it does not have a mass, it won't move in the simulation.
  * Many things will use this class as a base.
  *
  * Parameters that can be set through Properties are defined in Property.h
  */

class PolyObject : public AbstractObject
{
public:

	// Constructors/Destructors
	//

	/// elaborate constructor, only one to use!
	explicit
	PolyObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& anImageName,
				const QString& anOutline,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness);

	/// Destructor
	virtual ~PolyObject ( );

	// Public attribute accessor methods
	//

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{	return theNameString;	}

	/// child objects must specify what type of body they are
	/// @returns b2_staticBody if this object has no mass
	///          or b2_dynamicBody if the property mass was set
	virtual b2BodyType getObjectType(void) const;

	/// parse all properties
	/// partially overridden from AbstractObject
	virtual void  parseProperties(void);

protected:
	/// TODO/FIXME: see same notes in RectObject...
	virtual void setFriction(b2FixtureDef* aFixtureDef);

	/** Parse the property POLYGONS_STRING into
	 *  shapes.
	 *  note: does not clear the shapelist!
	 */
	virtual void fillShapeList(void);

private:
	PolyObject(const PolyObject& aRef);
	PolyObject& operator = (const PolyObject& aRef);

protected:
	QString theNameString;

	/// AABB calculates the Axis-Aligned Bounding Box
	/// which we can use for scaling
	class AABB {
	public:
		// default constructor, just to make sure we can detect uninit usage
		AABB(void);

		/// constructor
		/// @param myPolygons string with the polygons definiton
		explicit AABB(QString& myPolygons);

		/// @returns the width of the unscaled object
		float getOrigWidth();
		/// @returns the height the unscaled object
		float getOrigHeight();
		/// @returns true if the AABB are initialised
		bool isInitialised();

	private:
		float theOrigWidth;
		float theOrigHeight;

        static constexpr float UNDEFINED = 99999.0f;
	};

	AABB theAABB;
};



/** the AbstractPolyObjectFactory
 *  Note that it is slightly more complex than usual, because it is generalised
 *  to create any type of PolyObject. Below the declaration, there will be several
 *  global instances each identifying one rectobject type
 */
class AbstractPolyObjectFactory : public ObjectFactory
{
	Q_OBJECT
public:
	AbstractPolyObjectFactory(
		const QString& anInternalName,
		const char*    aDisplayName,
		const char*    aTooltip,
		const QString& anImageName,
		const QString& anOutline,
		qreal aWidth,
		qreal aHeight,
		qreal aMass,
		qreal aBounciness)
			: theDisplayName(aDisplayName),	theTooltip(aTooltip),
			  theImageName(anImageName), theOutline(anOutline),
			  theWidth(aWidth), theHeight(aHeight),
			  theMass(aMass), theBounciness(aBounciness)
	{	announceObjectType(anInternalName, this); }

	virtual AbstractObject* createObject(void) const
	{	return fixObject(new PolyObject(tr(theDisplayName), tr(theTooltip),
										theImageName, theOutline,
										theWidth, theHeight, theMass,
										theBounciness)); }
private:
		const char* theDisplayName;
		const char* theTooltip;
		QString theImageName;
		QString theOutline;
		qreal theWidth;
		qreal theHeight;
		qreal theMass;
		qreal theBounciness;
};

#endif // POLYOBJECT_H
