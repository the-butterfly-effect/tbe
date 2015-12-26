/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011  Klaas van Gend
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

#ifndef GOAL_H
#define GOAL_H

#include "Property.h"
#include "GoalSerializer.h"

// forward declarations
class World;
class AbstractObject;


/** Interface for all Goal classes. Goal classes can either result in a
 *  success or in a fail - in case the flag 'isFail' is set on the Goal.
 */
class Goal
{
public:
	Goal();
	virtual ~Goal();

	virtual const QString getGoalType(void) const = 0;

	/** Pure virtual member - check if this goal is met.
	  *
	  * @note it is up to the class to decide if a goal is met forever or needs to be met
	  * (that makes a difference if you have more than one goal)
	  *
	  * @note if isFail is set, the definition of 'success' changes - it still returns true
	  * if all conditions are met, but in that case it is a fail ;-)
	  *
	  * @returns true if this goal has been met
	  */
	virtual bool checkForSuccess(void) = 0;

	bool isFail;

	/** after all properties are set, call this function
	  * to have the goal actually try to understand the
	  * properties.
	  * @param aWorldPtr
	  */
	virtual bool parseProperties(World* aWorldPtr) = 0;

protected:
	/** generates a QString representing this goal
	  * the returned QString will have the following format:
	  * Variable;ObjectID;Condition;Value;ObjectID2  (ObjectID2 is optional)
	  * @returns the string
	  *
	  * @note only for access by GoalSerializer...
	  */
	virtual QString goalToStringList() const = 0;


protected:
	PropertyList theProps;
	friend class GoalSerializer;
};

/// implemented Goal - distance between two objects grows larger than or is less than a limit
class GoalDistance : public Goal
{
public:
	enum DistanceType
	{
		NOTYPE,
		LESSTHAN,
		MORETHAN,
		// future?: MORETHANRELATIVE
	};

	/** constructor
	  * NOTE: this class is only fully usable after parseProperties has been run
	  */
	GoalDistance();
	virtual ~GoalDistance();

	virtual const QString getGoalType(void) const
	{	return "distance";	};

	virtual bool parseProperties(World* aWorldPtr);

	virtual bool checkForSuccess(void);

protected:
	virtual QString goalToStringList() const;

private:
	DistanceType theType;
	float theLimit;
	AbstractObjectPtr theFirstPtr;
	AbstractObjectPtr theSecondPtr;

	friend class GoalSerializer;
};

/// implemented Goal - win if Position of an Object changes
class GoalPositionChange : public Goal
{
public:
	enum PositionType
	{
		NOTYPE          = 255,
		XCHANGED        = 4*GoalSerializer::POSITIONX + 1,
		XBELOW          = 4*GoalSerializer::POSITIONX  + 2,
		XOVER           = 4*GoalSerializer::POSITIONX   + 3,
		YCHANGED        = 4*GoalSerializer::POSITIONY + 1,
		YBELOW          = 4*GoalSerializer::POSITIONY  + 2,
		YOVER           = 4*GoalSerializer::POSITIONY   + 3,
		ANGLECHANGED    = 4*GoalSerializer::ANGLE     + 1,
		ANYTHINGCHANGED = 4*GoalSerializer::ANYTHING  + 1
	};

	/** constructor
	  * NOTE: this class is only fully usable after parseProperties has been run
	  */
	GoalPositionChange();
	virtual ~GoalPositionChange();

	virtual const QString getGoalType(void) const
	{	return "positionchange";	};

	virtual bool parseProperties(World* aWorldPtr);

	virtual bool checkForSuccess(void);

protected:
	virtual QString goalToStringList() const;

private:
	PositionType theType;
	AbstractObjectPtr theBOPtr;
	float theLimit;
};

/// implemented Goal - win if the State of an Object changes
class GoalStateChange : public Goal
{
public:
	enum StateType
	{
		NOTYPE          = 255,
		STATECHANGE     = 4*GoalSerializer::STATE + 1,
		STATEOVER	    = 4*GoalSerializer::STATE + 2
	};

	/** constructor
	  * NOTE: this class is only fully usable after parseProperties has been run
	  */
	GoalStateChange();
	virtual ~GoalStateChange();

	virtual const QString getGoalType(void) const
	{	return "statechange";	};

	virtual bool parseProperties(World* aWorldPtr);

	virtual bool checkForSuccess(void);

protected:
	virtual QString goalToStringList() const;

private:
	StateType theType;
	AbstractObjectPtr theBOPtr;
	int theState;
};


/// implemented Goal - win if a number of Pingus survived by escaping
class GoalEscapedPingusCounter : public Goal
{
public:
	/** constructor
	  * NOTE: this class is only fully usable after parseProperties has been run
	  */
	GoalEscapedPingusCounter();
	virtual ~GoalEscapedPingusCounter();

	virtual const QString getGoalType(void) const
	{	return "escapedPingusCount";	};

	virtual bool parseProperties(World* aWorldPtr);

	virtual bool checkForSuccess(void);

protected:
	virtual QString goalToStringList() const;

private:
	float theLimit;
};


#endif // GOAL_H
