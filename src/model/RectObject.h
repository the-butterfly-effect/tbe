/* The Butterfly Effect
 * This file copyright (C) 2009,2012  Klaas van Gend
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

#ifndef RECTOBJECT_H
#define RECTOBJECT_H

#include "AbstractObject.h"
#include "ObjectFactory.h"

/// forward declarations
struct b2FixtureDef;

/**
  * class RectObject
  *
  * This class defines a rectangular object.
  *
  * Parameters that can be set through Properties are defined in Property.h
  */

class RectObject : public AbstractObject
{
public:

	// Constructors/Destructors
	//

	/**
	 * Empty Constructor
	 */
	RectObject ( );

	/// elaborate constructor, only used by the ConcreteRectObject thingies
	RectObject( const QString& aDisplayName,
				const QString& aTooltip,
				const QString& aImageName,
				qreal aWidth, qreal aHeight, qreal aMass, qreal aBounciness);

	/**
	 * Empty Destructor
	 */
	virtual ~RectObject ( );

	/// returns the Name of the object.
	virtual const QString getName ( ) const
	{
		return theNameString;
	}

	/// child objects must specify what type of body they are
	/// @returns b2_staticBody if this object has no mass
	///          or b2_dynamicBody if the property mass was set
	virtual b2BodyType getObjectType(void) const;

	/// Parse all properties.
	/// Partially overridden from AbstractObject
	virtual void  parseProperties(void);

protected:
	virtual void initRectAttributes ( );

	static const qreal ASPECT_RATIO;

	/// TODO/FIXME: promote this member to AbstractObject???
	/// TODO/FIXME: add documentation for this member!!!
	void setFriction(b2FixtureDef* aFixtureDef);

protected:
	QString theNameString;
};


/** the AbstractRectObjectFactory
 *  note that it is slightly more complex than usual, because it is generalised
 *  to create any type of rectobject. Below the declaration, there will be several
 *  global instances each identifying one rectobject type
 */
class AbstractRectObjectFactory : public ObjectFactory
{
	Q_OBJECT
public:
	AbstractRectObjectFactory(
		const QString& anInternalName,
		const char*    aDisplayName,
		const char*    aTooltip,
		const QString& anImageName,
		qreal aWidth,
		qreal aHeight,
		qreal aMass,
		qreal aBounciness)
			: theDisplayName(aDisplayName),	theTooltip(aTooltip),
			  theImageName(anImageName), theWidth(aWidth), theHeight(aHeight),
			  theMass(aMass), theBounciness(aBounciness)
	{	announceObjectType(anInternalName, this); }

	virtual AbstractObject* createObject(void) const
	{	return fixObject(new RectObject(tr(theDisplayName), tr(theTooltip),
										theImageName, theWidth, theHeight,
										theMass, theBounciness)); }
private:
		const char* theDisplayName;
		const char* theTooltip;
		QString theImageName;
		qreal theWidth;
		qreal theHeight;
		qreal theMass;
		qreal theBounciness;
};

#endif // RECTOBJECT_H
