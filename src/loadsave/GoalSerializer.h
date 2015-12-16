/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef GOALSERIALIZER_H
#define GOALSERIALIZER_H

// forward declarations:
class Goal;
class World;
class QDomElement;
class QDomNode;

/** This Serializer is used for Goals
 *  Once created, if you feed it a QDomElement pointer, it will
 *  create a QDomElement representing itself and add it to the
 *  forementioned element.
 *
 *  The second usage of this class is to create the right Goals
 *  from the XML DOM node.
 *
 *  INSTANCES OF THIS CLASS SHOULD NEVER EXIST
 */
class GoalSerializer
{
public:
	/// always have a virtual destructor
	virtual ~GoalSerializer() {};


	/** create a Goal from the information in the provided Dom
	 *  @param q  the QDomNode containing the goal definition
	 *  @returns  nullptr if failed or a pointer to a valid Goal if successful.
	 *            Note that that Goal is still on its own - not attached to a World yet.
	 */
	static Goal* createObjectFromDom(const QDomNode& q);

	/** Create a Goal from the information provided in the aString.
	  * This member is used by the GoalEditor to fill the Goals from the array.
	  * format of the aString is:
	  *    Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
	  * @param    aWorldPtr pointer to the World instance, used to check ObjectIDs
	  * @param    aString the string to parse into a Goal
	  * @returns  nullptr if failed or a pointer to a valid Goal if successful.
	  *           Note that that Goal is still on its own - not attached to a World yet.
	  */
	static Goal* createObjectFromString(World* aWorldPtr, const QString& aString);

	/** create a QDomElement of the Goal anGoalPtr and add it to aParent
	  * @param aGoalPtr pointer to the goal to serialize (=save)
	  * @param aParent QDomElement pointer to add the serialized data to
	  * @returns true if successful
	  */
	static bool serialize(const Goal* aGoalPtr, QDomElement& aParent);

	/** creates a QStringList from a goal
	  * the returned QStringList will have the following format:
	  * Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
	  * @param aGoalPtr the Goal instance to build a string from
	  * @returns the string list describing *aGoalPtr
	  */
	static QStringList goalToStringList(const Goal* aGoalPtr);


	/// the Goal Types indicate the type of goal.
	/// 0-3 (POSITIONX-ANYTHING) are subtypes of GoalPositionChange,
	/// 4 (DISTANCE) is a GoalDistance
	/// 5 (STATE) is a GoalStateChange
	/// 6 (ESCPINGUS) is a GoalEscapedPingusCounter
	/// don't change existing numbers, quite some code depends on the order
	enum GoalTypes
	{
		POSITIONX = 0,
		POSITIONY,
		ANGLE,
		ANYTHING,
		DISTANCE = 4,
		STATE = 5,
		ESCPINGUS = 6,
	};

	/// @returns a sparsely populated QStringList with all possible
	/// entries for column zero. They follow the enum GoalTypes.
	static QStringList getColumnZero(void);

private:
	/// constructor only called by Goal
	// (not implemented yet)
	//GoalSerializer(const Goal* anObjectPtr);

	/// Neutralize copy constructor and assignment operator
	GoalSerializer(GoalSerializer&) = delete;
	/// Neutralize copy constructor and assignment operator
	GoalSerializer& operator=(const GoalSerializer&) = delete;
};

#endif // GOALSERIALIZER_H
