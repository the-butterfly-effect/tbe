/* The Butterfly Effect
 * This file copyright (C) 2009,2011  Klaas van Gend
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

#include <QtGui/QApplication>
#include "MainWindow.h"

// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 4;

// this variable defines whether we are playing or a level editor
// switching to LevelEditor is possible through the "Edit" menu.
bool theIsLevelEditor = false;

// this variable defines whether in the level editor we are expecting collision
// detection to work or not - you want this on to align certain things
// TODO/FIXME: this should go into preferences or so at some point
bool theIsCollisionOn = false;

// this variable defines whether we are showing Box2D debugging info or not
// switching this option is possible through the theDrawDebugActionPtr in MainWindow
bool theDrawDebug = true;

// this variable defines whether DrawPolyOutline draws the polygons
// over the image
// switching this option is possible through the theDrawOutlineActionPtr in MainWindow
bool theDrawPolyOutline = false;

// This variable defines whether TBE displays a frame refresh counter or not
// TODO/FIXME: this should go into preferences or so at some point
// (enabled in debug builds/disabled in release builds)
#ifdef NDEBUG
bool theDisplayFramerate = false;
#else
bool theDisplayFramerate = true;
#endif

#if !defined(NDEBUG)
 extern void setupBacktrace(void);
#endif


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#if !defined(NDEBUG)
	setupBacktrace();
#endif

	MainWindow w;
	w.show();
	return a.exec();
}

const char* ASCII(const QString& aQString)
{
		static char myString[256];
		strncpy(myString, aQString.toAscii().constData(), 255);
		return myString;
}
