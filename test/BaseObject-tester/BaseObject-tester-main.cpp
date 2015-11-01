/* The Butterfly Effect
 * This file copyright (C) 2010,2015  Klaas van Gend
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

#include "AbstractObject.h"
#include "ObjectFactory.h"
#include "World.h"

#include <QStringList>


// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 6;

class AbstractObjectForTesting : public AbstractObject
{
public:
	AbstractObjectForTesting ()
	{ theProps.setDefaultPropertiesString("TestProperty1:one/TestProperty2:/Mass:6.84/Bounciness:2.0/"); }

	virtual const QString getName      () const {return "AbstractObjectForTesting";}
	virtual b2BodyType getObjectType   () const {return b2_staticBody;}
	virtual const QString getToolTip   () const {return "lala";}
	virtual bool isRotatable           () const {return false;}
	virtual SizeDirections isResizable () const {return NORESIZING;}

	friend class TestAbstractObjectInit;
	friend class TestAbstractObjectParse;
};

static JointInterface::JointStatus theLastStatus;

class BOFTJoint : public JointInterface
{
public:
	virtual	void physicsObjectStatus(JointStatus aStatus)
	{ theLastStatus = aStatus; }
	virtual void jointWasDeleted(void)
	{ ; }
};


class AbstractObjectForTesting2 : public AbstractObjectForTesting
{
public:
	AbstractObjectForTesting2 ()
	{ theProps.setDefaultPropertiesString("/-Mass:"); }

	virtual const QString getName      () const {return "AbstractObjectForTesting2";}

	friend class TestAbstractObjectParse;
};


class TestAbstractObjectInit : public TestChapter
{
public:
	TestAbstractObjectInit() : TestChapter("Test AbstractObject class - init")	{}
	virtual bool runTests();
};
class TestAbstractObjectParse : public TestChapter
{
public:
	TestAbstractObjectParse() : TestChapter("Test AbstractObject class - parseProperties")	{}
	virtual bool runTests();
};

bool TestAbstractObjectInit::runTests(void)
{
	World* myWorldPtr = new World;
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "No custom bodies registered yet\n");

	// we're going to follow the init in DrawWorld::dragEnterEvent here:
	AbstractObjectForTesting myBO;
		check(myBO.getB2WorldPtr() == myWorldPtr->theB2WorldPtr, "B2WorldPtr is set correctly\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "No new bodies registered\n");
		check(myBO.theB2BodyDefPtr!=NULL, "B2BodyDef is created\n");
		check(myWorldPtr->theObjectPtrList.count() == 0, "creating AbstractObject doesn't add it to World\n");

	myBO.setOrigCenter(Position(3.42, 6.84, 1.57));
		check(myBO.getOrigCenter() == Position(3.42,6.84,1.57), "Position is reported correctly\n");
		check(myBO.theB2BodyPtr==NULL, "No B2Body yet\n");
		 // because there is no movement, Temp should be Orig
		check(myBO.getTempCenter() == Position(3.42,6.84,1.57), "Position is reported correctly\n");

	testmsg("First createPhysicsObject\n");

	JointInterfacePtr myJointPtr = ObjectFactory::createChildObject<BOFTJoint>();
	theLastStatus = static_cast<JointInterface::JointStatus>(0);
	myBO.addJoint(myJointPtr);
	myBO.createPhysicsObject();
		check(myBO.theB2BodyPtr != NULL, "A Physics object now exists\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 2, "One body is now registered\n");
		 // however, once a physicsobject has been created, temp reports the position from the phys object
		 // which should be the same - within rounding
		check(myBO.getTempCenter().toString() == "(3.42,6.84)@1.57", "Position is reported correctly\n");
		check(myWorldPtr->theObjectPtrList.count() == 0, "create Physics body doesn't add AbstractObject to World\n");
		check(theLastStatus == JointInterface::CREATED, "joint notification of creation was done\n");
		b2Body* myBodyPtr = myBO.theB2BodyPtr;
	testmsg("First deletePhysicsObject\n");
	myBO.deletePhysicsObject();
		check(myBO.theB2BodyPtr == NULL, "Pointer to Physics object now NULL\n");
		check(myWorldPtr->theB2WorldPtr->GetBodyCount() == 1, "b2Body gone from World\n");
		check(theLastStatus == JointInterface::DELETED, "joint notification of deletion was done\n");

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


bool TestAbstractObjectParse::runTests(void)
{
	AbstractObjectForTesting  myBO1;
	AbstractObjectForTesting2 myBO2;

	// check default property behavior

	check(myBO1.theProps.getDefaultProperty(Property::ZVALUE_STRING)=="2.0",
		  "Default property from AbstractObject is present\n");
	check(myBO1.theProps.getDefaultProperty(Property::IMAGE_NAME_STRING)=="",
		  "Default property from AbstractObject is empty\n");
	check(myBO1.theProps.getDefaultProperty("DoesNotExist").isNull(),
		  "Nonexistant default property from AbstractObject is null\n");
	check(myBO1.theProps.getDefaultProperty(Property::BOUNCINESS_STRING)=="2.0",
		  "Default property from AbstractObjectTester is present\n");
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

	myFramework.add( new TestAbstractObjectInit );
	myFramework.add( new TestAbstractObjectParse);

	myFramework.run();

	return 0;
}
