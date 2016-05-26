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

#include "TestChapter.h"
#include "tbe_global.h"

#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <cassert>

TestChapter::TestChapter(QString aTitle) : theTitle(aTitle)
{
    theNumberOfOKs = 0;
    theNumberOfFAILs = 0;
}

TestChapter::~TestChapter()
{
    // empty
}

bool TestChapter::check(bool aCondition, const QString &aMessage, bool abortIfWrong)
{
    if (aCondition) {
        QString myTotalLine = "* \033[1m\033[32mOK\033[m  : " + aMessage;
        printf ("%s", ASCII(myTotalLine));
        theNumberOfOKs++;
    } else {
        printf("* \033[1m\033[31mFAIL: %s \033[m", ASCII(aMessage));
        if (errno != EAGAIN)
            testmsg("the strerror might help: '%s'\n", strerror(errno));
        else
            testmsg("\n");
        theNumberOfFAILs++;

        if (abortIfWrong) {
            printf("*****************************\nFATAL FAIL\n");
            finish();
            exit(35);
        }
    }

    return aCondition;
}


void TestChapter::finish(void)
{
    int myTotal = theNumberOfOKs + theNumberOfFAILs;
    assert(myTotal > 0);
    printf("\nChapter summary:");
    printf("OKs  : % 3d, (% 4d %%)   /    ",
           theNumberOfOKs, (100 * theNumberOfOKs) / myTotal);
    printf("FAILs: % 3d, (% 4d %%)\n\n",
           theNumberOfFAILs, (100 * theNumberOfFAILs) / myTotal);
}


bool TestChapter::setUp()
{
    return true;
}

bool TestChapter::tearDown()
{
    return true;
}

