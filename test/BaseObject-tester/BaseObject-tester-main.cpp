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

#include "TestFramework.h"
#include "TestChapter.h"
#include "tbe_global.h"

#include "BaseObject.h"
#include "World.h"

#include <QStringList>


// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 6;

class BaseObjectForTesting : public BaseObject
{
public:
	BaseObjectForTesting ()
	{ theProps.setDefaultPropertiesString("TestProperty1:one/TestProperty2:/Mass:6.84/Bounciness:2.0/"); }

	virtual const QString getName      () const {return "BaseObjectForTesting";}
	virtual const QString getToolTip   () const {return "lala";}
	virtual bool isRotatable           () const {return false;}
	virtual SizeDirections isResizable () const {return NORESIZING;}

	friend class TestBaseObjectInit;
	friend class TestBaseObjectParse;
};


class BOFTJoint : public JointInterface
{
public:
	JointStatus theLastStatus;
	virtual	void physicsObjectStatus(JointStatus aStatus)
	{ theLastStatus = aStatus; }
};


class BaseObjectForTesting2 : public BaseObjectForTesting
{
public:
	BaseObjectForTesting2 ()
	{ theProps.setDefaultPropertiesString("/-Mass:"); }

	virtual const QString getName      () const {return "BaseObjectForTesting2";}

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
	// because there is no refcounting, if the joint doesn't outlive
	// the BO, the app will crash. That's why we have to accept some
	// memory leakage here... (okok, I could have written a destructor)
	BOFTJoint* myJointPtr = new BOFTJoint();
	myJointPtr->theLastStatus = static_cast<JointInterface::JointStatus>(0);
	myBO.addJoint(myJointPtr);
	myBO.createPhysicsObject();
		check(myBO.theB2BodyPtr != NULL, "A Physics object now exists\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 2, "One body is now registered\n");
		 // however, once a physicsobject has been created, temp reports the position from the phys object
		 // which should be the same - within rounding
		check(myBO.getTempCenter().toString() == "(3.42,6.84)@1.57", "Position is reported correctly\n");
		check(myWorldPtr->theObjectPtrList.count() == 0, "create Physics body doesn't add BaseObject to World\n");
		check(myJointPtr->theLastStatus == JointInterface::CREATED, "joint notification of creation was done\n");
		b2Body* myBodyPtr = myBO.theB2BodyPtr;
	testmsg("First deletePhysicsObject\n");
	myBO.deletePhysicsObject();
		check(myBO.theB2BodyPtr == NULL, "Pointer to Physics object now NULL\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "b2Body gone from World\n");
		check(myJointPtr->theLastStatus == JointInterface::DELETED, "joint notification of deletion was done\n");

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

	// the last entry in DrawWorld::dragEnterEvent:
	// parseProperties() is in the next test...

	return true;
}


bool TestBaseObjectParse::runTests(void)
{
	BaseObjectForTesting  myBO1;
	BaseObjectForTesting2 myBO2;

	// check default property behavior

	check(myBO1.theProps.getDefaultProperty(Property::ZVALUE_STRING)=="2.0",
		  "Default property from BaseObject is present\n");
	check(myBO1.theProps.getDefaultProperty(Property::IMAGE_NAME_STRING)=="",
		  "Default property from BaseObject is empty\n");
	check(myBO1.theProps.getDefaultProperty("DoesNotExist").isNull(),
		  "Nonexistant default property from BaseObject is null\n");
	check(myBO1.theProps.getDefaultProperty(Property::BOUNCINESS_STRING)=="2.0",
		  "Default property from BaseObjectTester is present\n");
	check(myBO1.theProps.getDefaultProperty(Property::MASS_STRING)=="6.84",
		  "Default Mass property is set in first one\n");
	check(myBO2.theProps.getDefaultProperty(Property::MASS_STRING)=="",
		  "Default Mass property is correctly removed in second\n");

	QStringList myProps1 = myBO1.theProps.getDefaultPropertyList();
	QStringList myProps2 = myBO2.theProps.getDefaultPropertyList();
	check(myProps2.count()+1 == myProps1.count(),
		  "The default propertylist for BO2 has one property less\n");
	check(myProps2.indexOf("Mass") == -1, "Mass as a property is missing\n");
	check(myProps1.indexOf("Mass") > 0, "Indeed the missing property is Mass\n");

	// actually implement tests for parseProperties() here


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
