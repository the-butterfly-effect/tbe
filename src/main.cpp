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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#include "MainWindow.h"
#include <QtGui>

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
bool theDrawDebug = false;

// this variable defines whether DrawPolyOutline draws the polygons
// over the image
// switching this option is possible through the theDrawOutlineActionPtr in MainWindow
bool theDrawPolyOutline = false;

// This variable defines whether TBE caches bitmaps of objects to speed
// up rendering. Actual implementation in DrawObject::setupCache() and
// ResizingGraphicsView::updatePixelsPerUnit() .
bool theTBECaching = true;

// This variable defines whether TBE displays a frame refresh counter or not
// TODO/FIXME: this should go into preferences or so at some point
// (enabled in debug builds/disabled in release builds)
#ifdef NDEBUG
bool theDisplayFramerate = false;
#else
bool theDisplayFramerate = true;
#endif

QString theStartFileName;


// -----------------------------------------------------------------------
// ------------------------- Command Line Parsing ------------------------
// -----------------------------------------------------------------------


static bool displayHelp(QString /*anArgument*/ )
{
	printf(APPNAME " " APPRELEASE "" APPFLAVOUR "\n\nhelp text\n\n");
	printf(" --help              gives this help text\n");
	printf(" -h                  gives this help text\n");
	printf(" --level-creator     start in level creator mode\n");
	printf(" -L                  start in level creator mode\n");
#if !defined(NDEBUG)
	printf(" --verbosity <lvl>   set verbosity, 1=little (default), %d=all\n", MAX_VERBOSITY);
	printf(" -v <lvl>            set verbosity\n");
#endif
	printf(" --windowed          display in a window (default is fullscreen)\n");
	printf(" -W                  display in a window (default is fullscreen)\n");
	printf("\n");
	exit(1);
}

static bool goLevelCreator( QString /*anArgument*/ )
{
	theIsLevelEditor = true;
	return true;
}

#if !defined(NDEBUG)
static bool setVerbosity(QString anArgument)
{
	// the argument should be a number. Let's parse.
	int myNumber = anArgument.toInt();
	if (myNumber < 1 || myNumber > MAX_VERBOSITY)
	{
		printf("ERROR: verbosity argument should be between 1 and %d\n", MAX_VERBOSITY);
		return false;
	}
	theVerbosity = myNumber;
	DEBUG2("set verbosity level to %d\n", theVerbosity);
	return true;
}
#endif

// local variable
static bool theIsMaximized = true;
static bool setWindowed( QString /*anArgument*/ )
{
	theIsMaximized = false;
	return true;
}


// this struct is used to list all long and short arguments
// it also contains a function pointer to a static function below
// that actually acts on the argument
struct s_args
{
	QString theFullCommand;
	QString theShortCommand;
	bool needsArgument;
	bool (*theFunctionPtr)(QString anArgument);
};


static struct s_args theArgsTable[] =
{
// keep sorted alphabetically, please
	{ "help",          "h", false, displayHelp, },
	{ "level-creator", "L", false, goLevelCreator, },
#if !defined(NDEBUG)
	{ "verbosity",     "v", true,  setVerbosity, },
#endif
	{ "windowed",      "W", false, setWindowed, },
//	keep this one last:
	{ "\0", "\0", false, NULL, },
};


// -----------------------------------------------------------------------
// ---------------------------------- main() -----------------------------
// -----------------------------------------------------------------------


int main(int argc, char **argv)
{
	// init Qt (graphics toolkit) - www.qtsoftware.com
	QApplication app(argc, argv);

	// read the locale from the environment and set the language...
	// TODO: This is fairly basic - I probably need to "borrow"
	//       the (maybe too flexible) code from umtsmon
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(I18N_DIRECTORY + "/tbe_" + locale);
	app.installTranslator(&translator);

	QCoreApplication::setOrganizationName("the-butterfly-effect.org");
	QCoreApplication::setOrganizationDomain("the-butterfly-effect.org");
	QCoreApplication::setApplicationName("The Butterfly Effect");

	QStringList myCmdLineList = app.arguments();

	bool isParsingSuccess=true;
	// we can skip argument zero - that's the tbe executable itself
	for (int i=1; i<myCmdLineList.size() && isParsingSuccess; i++)
	{
		QString myArg = myCmdLineList[i];
		if (myArg.startsWith("-"))
		{
			// remove one or two dashes - we're slightly more flexible than usual
			myArg.remove(0,1);
			if (myArg.startsWith("-"))
				myArg.remove(0,1);

			// extract value with = if there is one
			QStringList myExp = myArg.split("=");

			// is it matching with short or long?
			int j=0;
			bool isMatch=false;
			while(theArgsTable[j].theFunctionPtr != NULL)
			{
				if (myExp[0] == theArgsTable[j].theFullCommand
					|| myExp[0] == theArgsTable[j].theShortCommand)
				{
					isMatch=true;
					QString myVal;
					if (theArgsTable[j].needsArgument == true)
					{
						// was it '='?
						if (myExp.count()==2)
							myVal = myExp[1];
						else
						{
							// or is it ' ' -> which means we need to grab next arg
							if (i+1<myCmdLineList.size())
							{
								myVal = myCmdLineList[i+1];
								i++;
							}
							else
							{
								isParsingSuccess=false;
								break;
							}
						}
					}
					if (theArgsTable[j].theFunctionPtr(myVal)==false)
						isParsingSuccess=false;
				}
				++j;
			}
			if (isMatch==false)
			{
				isParsingSuccess=false;
				break;
			}
		}
		else
		{
			// if it is a single string, it probably is a file name
			theStartFileName = myArg;
		}

	}

	if (isParsingSuccess==false)
		displayHelp("");

	DEBUG3("SUMMARY:\n");
	DEBUG3("Verbosity is: %d / Fullscreen is %d\n", theVerbosity, theIsMaximized);
	DEBUG3("Start file name is: '%s'\n", ASCII(theStartFileName));

	// which settings file is used can be confusing to the user...
	{
		QSettings mySettings;
		DEBUG3("using settings from: \"%s\"\n", ASCII(mySettings.fileName()));
	}

	// setup main window
	MainWindow myMain(theIsMaximized);
	myMain.show();

	// run the main display loop    
	int myReturn=app.exec();
    
	return myReturn;
}


const char* ASCII(const QString& aQString)
{
	return aQString.toAscii().constData();
}
