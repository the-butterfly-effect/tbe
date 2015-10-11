/* The Butterfly Effect Test Framework
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
#include <QApplication>

#include <unistd.h>
#include <cstdio>
#include <cassert>

const char* ASCII(const QString& aQString)
{
		return aQString.toAscii().constData();
}

TestFramework::TestFramework(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.processEvents();
}


TestFramework::~TestFramework()
{
	ChapterList::iterator myPtr = theChapters.begin();
	while (myPtr != theChapters.end())
	{
		ChapterList::iterator myNextPtr = ++myPtr;
		// TODO: insert actual removal here
		myPtr = myNextPtr;
	}
}


void TestFramework::run(void)
{
	int myTotalOKs   = 0;
	int myTotalFAILs = 0;

	ChapterList::iterator myPtr = theChapters.begin();
	int myChapterNumber = 1;
	while (myPtr != theChapters.end())
	{
		printf("********** Chapter %d: `%s` *******\n\n", myChapterNumber, ASCII((*myPtr)->getTitle()));

		(*myPtr)->setUp();
		(*myPtr)->runTests();
		(*myPtr)->tearDown();
		(*myPtr)->finish();
		myTotalOKs   += (*myPtr)->getOKs();
		myTotalFAILs += (*myPtr)->getFAILs();

		++myPtr;
		++myChapterNumber;
	}

	int myTotal = myTotalOKs + myTotalFAILs;
	assert(myTotal > 0);
	printf("\nTotal summary:\n==============\n");
	printf("Total Chapters run : % 2d\n", myChapterNumber-1 );
	printf("Total OKs  : % 3d, (% 4d %%)\n",
		   myTotalOKs, (100*myTotalOKs)/myTotal);
	printf("Total FAILs: % 3d, (% 4d %%)\n\n",
		   myTotalFAILs, (100*myTotalFAILs)/myTotal);
}
