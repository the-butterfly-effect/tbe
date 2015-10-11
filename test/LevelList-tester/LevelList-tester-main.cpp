/* The Butterfly Effect
 * This file copyright (C) 2009  Klaas van Gend
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

#include <QCoreApplication>

#include "LevelList.h"
#include "tbe_global.h"

#include <unistd.h>

// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 4;


class TestLevelList1 : public TestChapter
{
public:
	TestLevelList1(const QString& aBasedir)
	    : TestChapter(QString("Test LevelList class - happy flow for '%1'").arg(aBasedir)),
	      theBaseDir(aBasedir + "/")
		{}
	virtual bool runTests();

	QString theBaseDir;
};

bool TestLevelList1::runTests(void)
{
	LevelList myList(theBaseDir, "levels.xml");
	QString myName1 = myList.getFirstLevel();
	check( myName1 == theBaseDir+"l/level1.xml", "Check first  level name is correct\n");
	QString myName2 = myList.getNextLevel(myName1);
	check( myName2 == theBaseDir+"l/level2.xml", "Check second level name is correct\n");
	QString myName3 = myList.getNextLevel(myName2);
	check( myName3 == theBaseDir+"l/level3.xml", "Check1 third level name is correct\n");
	check( myName3 != myName2,                   "Check2 third level name is correct\n");
	QString myName4 = myList.getNextLevel(myName3);
	check( myName4 == "", "Check there is no fourth name\n");
	
	LevelList::LevelMetaInfo myMeta1 = myList.getLevelMetaInfo(myName1);
	check (myMeta1.theFileName == myName1, "Check1 LMI filename for first level is correct\n");
	check (myMeta1.theFileName != myName2, "Check2 LMI filename for first level is correct\n");
	
	return true;
}


int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	// test with relative path
	myFramework.add( new TestLevelList1("./") );
	
	// test with more complicated relative path
//	myFramework.add( new TestLevelList1("../LevelList-tester/") );

	// test with absolute path
	char buf[256];
//	myFramework.add( new TestLevelList1(getcwd(buf, 255)));
	
	myFramework.run();

	return 0;
}


// const char* ASCII(const QString& aQString)
// {
// 		return aQString.toAscii().constData();
// }
