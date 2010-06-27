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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "TestFramework.h"
#include "TestChapter.h"
#include "tbe_global.h"

#include "BaseObject.h"
#include "World.h"

#include <cmath>


// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 6;

class BaseObjectForTesting : public BaseObject, public JointInterface
{
public:
	virtual const QString getName      () const {return "BaseObjectForTesting";}
	virtual const QString getToolTip   () const {return "lala";}
	virtual bool isRotatable           () const {return false;}
	virtual SizeDirections isResizable () const {return NORESIZING;}

	JointStatus theLastStatus;
	virtual	void physicsObjectStatus(JointStatus aStatus)
	{ theLastStatus = aStatus; }

	friend class TestBaseObjectInit;
	friend class TestBaseObjectParse;
};



class TestBaseObjectInit : public TestChapter
{
public:
	TestBaseObjectInit() : TestChapter("Test BaseObject class - init")	{}
	virtual bool runTests();
};
class TestBaseObjectParse : public TestChapter
{
public:
	TestBaseObjectParse() : TestChapter("Test BaseObject class - parseProperties")	{}
	virtual bool runTests();
};

bool TestBaseObjectInit::runTests(void)
{
	World* myWorldPtr = new World;
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "No custom bodies registered yet\n");

	// we're going to follow the init in DrawWorld::dragEnterEvent here:
	BaseObjectForTesting myBO;
		check(myBO.getB2WorldPtr() == myWorldPtr->theB2WorldPtr, "B2WorldPtr is set correctly\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "No new bodies registered\n");
		check(myBO.theB2BodyDefPtr!=NULL, "B2BodyDef is created\n");
		check(myWorldPtr->theObjectPtrList.count() == 0, "creating BaseObject doesn't add it to World\n");

	myBO.setOrigCenter(Position(3.42, 6.84, 1.57));
		check(myBO.getOrigCenter() == Position(3.42,6.84,1.57), "Position is reported correctly\n");
		check(myBO.theB2BodyPtr==NULL, "No B2Body yet\n");
		 // because there is no movement, Temp should be Orig
		check(myBO.getTempCenter() == Position(3.42,6.84,1.57), "Position is reported correctly\n");

	testmsg("First createPhysicsObject\n");
	myBO.theLastStatus = static_cast<JointInterface::JointStatus>(0);
	myBO.addJoint(&myBO);
	myBO.createPhysicsObject();
		check(myBO.theB2BodyPtr != NULL, "A Physics object now exists\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 2, "One body is now registered\n");
		 // however, once a physicsobject has been created, temp reports the position from the phys object
		 // which should be the same - within rounding
		check(myBO.getTempCenter().toString() == "(3.42,6.84)@1.57", "Position is reported correctly\n");
		check(myWorldPtr->theObjectPtrList.count() == 0, "create Physics body doesn't add BaseObject to World\n");
		check(myBO.theLastStatus == JointInterface::CREATED, "joint notification of creation was done\n");
		b2Body* myBodyPtr = myBO.theB2BodyPtr;
	testmsg("First deletePhysicsObject\n");
	myBO.deletePhysicsObject();
		check(myBO.theB2BodyPtr == NULL, "Pointer to Physics object now NULL\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "b2Body gone from World\n");
		check(myBO.theLastStatus == JointInterface::DELETED, "joint notification of deletion was done\n");

	// let's redo physics object creation - the old one should be deleted and the
	// new one should have taken the place, everywhere
	testmsg("Second createPhysicsObject - doubly\n");
	myBO.createPhysicsObject();
		myBodyPtr = myBO.theB2BodyPtr;
	myBO.createPhysicsObject();
		check(myBO.theB2BodyPtr != myBodyPtr, "Different body\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 2, "Still one body registered\n");

	myWorldPtr->addObject(&myBO);
		check(myWorldPtr->theObjectPtrList.count() == 1, "addObject succeeded\n");

	// parseProperties is in the next test...

	return true;
}


bool TestBaseObjectParse::runTests(void)
{
	// todo: insert test here...
	check(true, "DUMMY TEST: Chapter present\n");

	return true;
}


int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	myFramework.add( new TestBaseObjectInit );
	myFramework.add( new TestBaseObjectParse);

	myFramework.run();

	return 0;
}
