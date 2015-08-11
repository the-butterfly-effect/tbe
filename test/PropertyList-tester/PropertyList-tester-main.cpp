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
	check(myPL.getPropertyNoDefault("Key1") == "abcde", "correct retrieval of property 1\n");
	check(myPL.getPropertyNoDefault("Key2") == "xyz", "correct retrieval of property 2\n");
	check(myPL.getPropertyCount() == 2, "still 2 properties\n");
	myPL.removeProperty("Key2");
	check(myPL.getPropertyCount() == 1, "after removal, 1 property remains\n");
	check(myPL.getPropertyNoDefault("Key1") == "abcde", "correct retrieval of property 1\n");
	check(myPL.getPropertyNoDefault("Key2") == "", "correct (failed) retrieval of property 2\n");
	myPL.removeProperty("Key2");
	myPL.removeProperty("Ridiculus String");
	check(myPL.getPropertyCount() == 1, "after failed removals, still 1 property remains\n");

	// property2Bool parsing - no default substitutions
	testmsg("propertyToBool tests, no default substitutions\n");
	bool myBool = 0;
	myPL.setProperty("Bool1", "true");
	myPL.setProperty("Bool2", "True");
	myPL.setProperty("Bool3", "Yes");
	myPL.setProperty("Bool4", "Positive"); // fail
	myPL.setProperty("Bool5", "No");
	myPL.setProperty("Bool6", "none");  // fail
	myPL.setProperty("Bool7", "1");   // fail

	check(myPL.property2Bool("Bool1",&myBool,false)==true && myBool==true,   "Bool1 was correctly retrieved\n");
	check(myPL.property2Bool("NoBool",&myBool,false)==false && myBool==true, "Failed key lookup doesn't touch variable\n");
	check(myPL.property2Bool("Bool5",&myBool,false)==true && myBool==false,  "Bool5 was correctly retrieved\n");
	check(myPL.property2Bool("Bool2",&myBool,false)==true && myBool==true,   "Bool2 was correctly retrieved\n");
	check(myPL.property2Bool("Bool6",&myBool,false)==false && myBool==true,  "Bool6 was correctly ignored\n");
	check(myPL.property2Bool("Bool5",&myBool,false)==true && myBool==false,  "Bool5 was correctly retrieved again\n");
	check(myPL.property2Bool("Bool4",&myBool,false)==false && myBool==false, "Bool4 was correctly ignored\n");
	check(myPL.property2Bool("Bool7",&myBool,false)==false && myBool==false, "Bool7 was correctly ignored\n");
	check(myPL.property2Bool("Bool3",&myBool,false)==true && myBool==true,   "Bool3 was correctly retrieved\n");

	myPL.setProperty("Bool8", true);
	myPL.setProperty("Bool9", false);
	myPL.setProperty("Bool5", true);

	check(myPL.property2Bool("Bool9",&myBool, false)==true && myBool==false,   "Bool9 was correctly retrieved\n");
	check(myPL.property2Bool("Bool8",&myBool, false)==true && myBool==true,   "Bool8 was correctly retrieved\n");
	check(myPL.property2Bool("Bool8",&myBool, false)==true && myBool==true,   "Bool8 was correctly retrieved\n");


	// propertyToFloat parsing of float parameters
	myPL.setProperty("Float1", "68.4");
	myPL.setProperty("Float2", "-6.84");
	myPL.setProperty("Float3", "-1.03e-3");
	myPL.setProperty("Float4", "Positive"); // fail
	myPL.setProperty("Float5", "23");
	myPL.setProperty("Float6", "45k");		// fail
	myPL.setProperty("Float7", "23+45.4");   // fail
	myPL.setProperty("Float8", 6.84f);

	testmsg("propertyToFloat tests, no default substitutions\n");
	float myFloat = 0;
	check(myPL.property2Float("Float1",&myFloat,false)==true && floatcompare(myFloat,68.4),
		  "Float1 was correctly retrieved\n");
	check(myPL.property2Float("Float2",&myFloat,false)==true && floatcompare(myFloat,-6.84),
		  "Float2 was correctly retrieved\n");
	check(myPL.property2Float("Float3",&myFloat,false)==true && floatcompare(myFloat,-1.03e-3),
		  "Float3 was correctly retrieved\n");
	check(myPL.property2Float("Float4",&myFloat,false)==false,
		  "Float4 was correctly retrieved\n");
	check(myPL.property2Float("Float5",&myFloat,false)==true && floatcompare(myFloat,23),
		  "Float5 was correctly retrieved\n");
	check(myPL.property2Float("Float6",&myFloat,false)==false,
		  "Float6 was correctly retrieved\n");
	check(myFloat==23, "failed retrieval doesn't affect float value\n");
	check(myPL.property2Float("Float7",&myFloat,false)==false,
		  "Float7 was correctly retrieved\n");
	check(myPL.property2Float("Float8",&myFloat,false)==true && floatcompare(myFloat,6.84),
		  "Float8 was correctly retrieved\n");

	// property2String parsing - no default substitutions
	testmsg("propertyToString tests, no default substitutions\n");
	myPL.setProperty("String1", "sixeightfour");
	// no String2

	QString myString;
	check(myPL.property2String("String1",&myString,false)==true, "String1 was retrieved\n");
	check(myString=="sixeightfour", "and String1 was correctly retrieved\n");
	myString = "lala";
	check(myPL.property2String("String2",&myString,false)==false && myString=="lala",
		  "String2 doesn't exist and myString wasn't touched\n");

	// propertyToVector (note that this relies heavily on Vector,
	// so we have not much to test for here...)

		// FIXME/TODO: TODO

	// propertyToObjectPtr ???

		// FIXME/TODO: TODO

	return true;
}


class TestPropertyWithDefaults : public TestChapter
{
public:
	TestPropertyWithDefaults() : TestChapter("Test Property With Defaults")	{}
	virtual bool runTests();

	bool floatcompare(float a, float b)
	{ return (a*a-b*b < 0.0001)?true:false; }
};


bool TestPropertyWithDefaults::runTests(void)
{
	// property2Bool parsing - with default substitution
	testmsg("propertyToBool tests, with default substitutions\n");
	PropertyList myPL;
	bool myBool = 0;
	myPL.setProperty("Bool1", "true");   // no default
	// no Bool2, default present
	// no Bool3, default present
	// Bool4 is true, default is false
	myPL.setProperty("Bool4", "true");   // no default
	// no Bool5, default is invalid
	myPL.setDefaultPropertiesString("Bool2:true/Bool3:false/Bool4:false/Bool5:wrong/");

	check(myPL.getDefaultProperty("Bool2")=="true", "default for Bool2 is present\n");
	check(myPL.getDefaultProperty("Bool5")=="wrong", "default for Bool5 is present and wrong\n");
	check(myPL.getDefaultProperty("Bool1").isEmpty(), "no default for Bool1\n");

	check(myPL.property2Bool("Bool1",&myBool,true)==true && myBool==true,   "Bool1 was correctly retrieved\n");
	check(myPL.property2Bool("NoBool",&myBool,true)==false && myBool==true, "Failed key lookup doesn't touch variable\n");
	check(myPL.property2Bool("Bool2",&myBool,true)==true && myBool==true,   "default value Bool2 was correctly retrieved\n");
	check(myPL.property2Bool("Bool3",&myBool,true)==true && myBool==false,   "default value Bool3 was correctly retrieved\n");
	check(myPL.property2Bool("Bool4",&myBool,true)==true && myBool==true,   "real Bool4 gets preference over default\n");
	check(myPL.property2Bool("Bool5",&myBool,true)==false && myBool==true,   "Bool5 default is invalid, variable untouched\n");

	myPL.removeProperty("Bool4");
	check(myPL.property2Bool("Bool4",&myBool,true)==true && myBool==false,   "default Bool4 gets chosen once real value is gone\n");


	// propertyToFloat parsing of float parameters
	myPL.setProperty("Float1", "68.4");	// no default
	myPL.setProperty("Float2", "bad");	// default = 6.84
	// no Float3, default = 3.42
	// no Float4, default is bad
	myPL.setDefaultPropertiesString("Float2:-6.84/Float3:3.42/Float4:bad/");

	testmsg("propertyToFloat tests\n");
	float myFloat = 0;
	check(myPL.property2Float("Float1",&myFloat, true)==true && floatcompare(myFloat,68.4),
		  "Float1 was correctly retrieved\n");
	check(myPL.property2Float("Float2",&myFloat, true)==true && floatcompare(myFloat,-6.84),
		  "bad Float2 ignored, default was correctly retrieved\n");
	check(myPL.property2Float("Float3",&myFloat, true)==true && floatcompare(myFloat,3.42),
		  "default for Float3 was correctly retrieved\n");
	myFloat = -100.01;
	check(myPL.property2Float("Float4",&myFloat, true)==false,
		  "Float4 was correctly ignored\n");
	check(floatcompare(myFloat, -100.01), "for Float4, float was correctly left untouched\n");


	// property2String parsing - with default substitutions
	testmsg("propertyToString tests, with default substitutions\n");
	myPL.setProperty("String1", "threefourtwo");
	// no String2, no default string2
	myPL.setDefaultPropertiesString("String1:sixeightfour/String3:onetwothree/");

	QString myString;
	check(myPL.property2String("String1",&myString,true)==true, "String1 was retrieved\n");
	check(myString=="threefourtwo", "and String1 was correctly retrieved\n");
	myString = "lala";
	check(myPL.property2String("String2",&myString,true)==false && myString=="lala",
		  "String2 doesn't exist and myString wasn't touched\n");
	check(myPL.property2String("String3",&myString,true)==true && myString=="onetwothree",
		  "String3 taken from default correctly\n");



	return true;
}



int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	myFramework.add( new TestPropertyList );
	myFramework.add( new TestPropertyWithDefaults );

	myFramework.run();

	return 0;
}
