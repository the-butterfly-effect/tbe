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

#include <QStringList>


// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 6;


class TestPropertyList : public TestChapter
{
public:
	TestPropertyList() : TestChapter("Test PropertyList")	{}
	virtual bool runTests();

	bool floatcompare(float a, float b)
	{ return (a*a-b*b < 0.0001)?true:false; }
};


bool TestPropertyList::runTests(void)
{
	// adding/removing/retrieving properties
	testmsg("Adding/Removing/Retrieving tests\n");
	PropertyList myPL;
	myPL.setProperty("Key1", "abcde");
	myPL.setProperty("Key2", "xyz");
	check(myPL.getPropertyCount() == 2, "proper count of properties\n");
	check(myPL.getProperty("Key1") == "abcde", "correct retrieval of property 1\n");
	check(myPL.getProperty("Key2") == "xyz", "correct retrieval of property 2\n");
	check(myPL.getPropertyCount() == 2, "still 2 properties\n");
	myPL.removeProperty("Key2");
	check(myPL.getPropertyCount() == 1, "after removal, 1 property remains\n");
	check(myPL.getProperty("Key1") == "abcde", "correct retrieval of property 1\n");
	check(myPL.getProperty("Key2") == "", "correct (failed) retrieval of property 2\n");
	myPL.removeProperty("Key2");
	myPL.removeProperty("Ridiculus String");
	check(myPL.getPropertyCount() == 1, "after failed removals, still 1 property remains\n");

	// propertyToBool parsing
	testmsg("propertyToBool tests\n");
	bool myBool = 0;
	myPL.setProperty("Bool1", "true");
	myPL.setProperty("Bool2", "True");
	myPL.setProperty("Bool3", "Yes");
	myPL.setProperty("Bool4", "Positive"); // fail
	myPL.setProperty("Bool5", "No");
	myPL.setProperty("Bool6", "none");  // fail
	myPL.setProperty("Bool7", "1");   // fail

	check(myPL.propertyToBool("Bool1",&myBool)==true && myBool==true,   "Bool1 was correctly retrieved\n");
	check(myPL.propertyToBool("NoBool",&myBool)==false && myBool==true, "Failed key lookup doesn't touch variable\n");
	check(myPL.propertyToBool("Bool5",&myBool)==true && myBool==false,  "Bool5 was correctly retrieved\n");
	check(myPL.propertyToBool("Bool2",&myBool)==true && myBool==true,   "Bool2 was correctly retrieved\n");
	check(myPL.propertyToBool("Bool6",&myBool)==false && myBool==true,  "Bool6 was correctly ignored\n");
	check(myPL.propertyToBool("Bool5",&myBool)==true && myBool==false,  "Bool5 was correctly retrieved again\n");
	check(myPL.propertyToBool("Bool4",&myBool)==false && myBool==false, "Bool4 was correctly ignored\n");
	check(myPL.propertyToBool("Bool7",&myBool)==false && myBool==false, "Bool7 was correctly ignored\n");
	check(myPL.propertyToBool("Bool3",&myBool)==true && myBool==true,   "Bool3 was correctly retrieved\n");

	myPL.setProperty("Bool8", true);
	myPL.setProperty("Bool9", false);
	myPL.setProperty("Bool5", true);

	check(myPL.propertyToBool("Bool9",&myBool)==true && myBool==false,   "Bool9 was correctly retrieved\n");
	check(myPL.propertyToBool("Bool8",&myBool)==true && myBool==true,   "Bool8 was correctly retrieved\n");
	check(myPL.propertyToBool("Bool8",&myBool)==true && myBool==true,   "Bool8 was correctly retrieved\n");


	// propertyToFloat parsing of float parameters
	myPL.setProperty("Float1", "68.4");
	myPL.setProperty("Float2", "-6.84");
	myPL.setProperty("Float3", "-1.03e-3");
	myPL.setProperty("Float4", "Positive"); // fail
	myPL.setProperty("Float5", "23");
	myPL.setProperty("Float6", "45k");		// fail
	myPL.setProperty("Float7", "23+45.4");   // fail
	myPL.setProperty("Float8", 6.84f);

	testmsg("propertyToFloat tests\n");
	float myFloat = 0;
	check(myPL.propertyToFloat("Float1",&myFloat)==true && floatcompare(myFloat,68.4),
		  "Float1 was correctly retrieved\n");
	check(myPL.propertyToFloat("Float2",&myFloat)==true && floatcompare(myFloat,-6.84),
		  "Float2 was correctly retrieved\n");
	check(myPL.propertyToFloat("Float3",&myFloat)==true && floatcompare(myFloat,-1.03e-3),
		  "Float3 was correctly retrieved\n");
	check(myPL.propertyToFloat("Float4",&myFloat)==false,
		  "Float4 was correctly retrieved\n");
	check(myPL.propertyToFloat("Float5",&myFloat)==true && floatcompare(myFloat,23),
		  "Float5 was correctly retrieved\n");
	check(myPL.propertyToFloat("Float6",&myFloat)==false,
		  "Float6 was correctly retrieved\n");
	check(myFloat==23, "failed retrieval doesn't affect float value\n");
	check(myPL.propertyToFloat("Float7",&myFloat)==false,
		  "Float7 was correctly retrieved\n");
	check(myPL.propertyToFloat("Float8",&myFloat)==true && floatcompare(myFloat,6.84),
		  "Float8 was correctly retrieved\n");



	// propertyToVector (note that this relies heavily on Vector,
	// so we have not much to test for here...)

		// FIXME/TODO: TODO

	// propertyToObjectPtr ???

		// FIXME/TODO: TODO

	return true;
}


int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	myFramework.add( new TestPropertyList );

	myFramework.run();

	return 0;
}
