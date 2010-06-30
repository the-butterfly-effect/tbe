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
	TestPropertyList() : TestChapter("Test BaseObject class - init")	{}
	virtual bool runTests();
};


bool TestPropertyList::runTests(void)
{

	check(true, "addObject succeeded\n");

	// the last entry in DrawWorld::dragEnterEvent:
	// parseProperties() is in the next test...

	return true;
}


int main(int argc, char *argv[])
{
	TestFramework myFramework(argc, argv);

	myFramework.add( new TestPropertyList );

	myFramework.run();

	return 0;
}
