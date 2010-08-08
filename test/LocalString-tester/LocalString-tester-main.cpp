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
#include "LocalString.h"

// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 4;

class TestLocalString : public TestChapter
{
public:
	TestLocalString() : TestChapter("Test LocalString class")	{}
	virtual bool runTests();
};

bool TestLocalString::runTests(void)
{
	// setup the string for mining...
	LocalString myLS;
	myLS.add("one","");
	myLS.add("een", "nl");
	myLS.add("einsz", "de_AT"); // yeah, I know it's wrong - but I need to see the difference
	myLS.add("eins", "de");
	myLS.add("un", "fr");
	myLS.add("eene", "nl_BE");  // this one is also deliberately wrong!

	// and now, let's check...
	myLS.the5Char = "en";
	check (myLS.result() == "one", "test 1\n");
	myLS.the5Char = "nl";
	check (myLS.result() == "een", "test 2\n");
	myLS.the5Char = "nl_BE";
	check (myLS.result() == "eene", "test 3\n");
	myLS.the5Char = "nl_NL";
	check (myLS.result() == "een", "test 4\n");
	myLS.the5Char = "de_DE";
	check (myLS.result() == "eins", "test 5\n");
	myLS.the5Char = "de";
	check (myLS.result() == "eins", "test 6\n");
	myLS.the5Char = "de_AT";
	check (myLS.result() == "einsz", "test 7\n");


	return true;
}


int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	myFramework.add( new TestLocalString );

	myFramework.run();

	return 0;
}
