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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#ifndef TESTCHAPTER_H
#define TESTCHAPTER_H

#include <QString>

class TestChapter
{
public:
	TestChapter(QString aTitle);
	virtual ~TestChapter();

	/// will contain the setup code
	/// init should only contain setup code that cannot go wrong
	/// otherwise: write a test for it
	virtual bool setUp();

	/// will contain the shutdown code
	/// if any teardown fails, return false
	virtual bool tearDown();
	
	/// will contain the actual tests, or calls to actual tests
	/// if you want to define paragraphs
	virtual bool runTests() = 0;



public:  // the below are only for TestChapter
	void finish(void);

	const QString& getTitle(void) const {return theTitle;}
	int getOKs(void) const {return theNumberOfOKs;}
	int getFAILs(void) const {return theNumberOfFAILs;}

protected:
	bool check(bool aCondition, const QString& aMessage, bool abortIfWrong=false);

	int theNumberOfOKs;
	int theNumberOfFAILs;

	QString theTitle;
};

#endif // TESTCHAPTER_H
