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

#ifndef TBE_GLOBAL
#define TBE_GLOBAL

#define APPNAME QObject::tr("The Butterfly Effect")
#define APPRELEASE "MilestoneA"
/* for your special versioning needs, normally empty */
#ifndef APPFLAVOUR
#define APPFLAVOUR ""
#endif
#include <QtCore/QtGlobal>
#include <QtCore/QString>

const float PI    = 3.14159265;
const float SQRT2 = 1.41421356;

/* Debug verbosity
 * level 1 = very little information, autodetect KILLING
 * level 2 = level 1 + autodetect GOOD/BAD + some general messages on behaviour
 * level 3 = level 2 + autodetect INFO
 * level 4 = level 3 + AT commands and answers
 * level 5 = all debugging, including entry of functions under scrutiny
 * level 6 = timing info on serial port
 */
#if !defined(NDEBUG)
#include <QtCore/QtDebug>
#include <ctime>
extern int theVerbosity;	// actually defined in main.cpp
#define MAX_VERBOSITY 6
// note: the below is actually GCC-specific, see "info:/gcc/Variadic Macros"
#define DEBUG1(format, ...)	{if (theVerbosity>=1) qDebug("**** t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG2(format, ...)	{if (theVerbosity>=2) qDebug("***  t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG3(format, ...)	{if (theVerbosity>=3) qDebug("**   t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG4(format, ...)	{if (theVerbosity>=4) qDebug("*    t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
//#define DEBUG4NT(format, ...)	{if (theVerbosity>=4) qDebug(format, ## __VA_ARGS__);}
#define DEBUG5(format, ...)	{if (theVerbosity>=5) qDebug("     t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG6(format, ...)	{if (theVerbosity>=6) qDebug("     t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
// only for use during debugging - print a backtrace at current position
extern void printBacktrace(void);
#define DEBUG1ENTRY			{if (theVerbosity>=1) qDebug("**** t=%03ld: entry %s", time(NULL)%1000, Q_FUNC_INFO);}
#define DEBUG3ENTRY			{if (theVerbosity>=3) qDebug("**   t=%03ld: entry %s", time(NULL)%1000, Q_FUNC_INFO);}
#define DEBUG4ENTRY			{if (theVerbosity>=4) qDebug("*    t=%03ld: entry %s", time(NULL)%1000, Q_FUNC_INFO);}
#define DEBUG5ENTRY			{if (theVerbosity>=5) qDebug("     t=%03ld: entry %s", time(NULL)%1000, Q_FUNC_INFO);}
#define DEBUG6ENTRY			{if (theVerbosity>=6) qDebug("     t=%03ld: entry %s", time(NULL)%1000, Q_FUNC_INFO);}
#else
#define DEBUG1(format, ...)     { /**/ }
#define DEBUG2(format, ...)     { /**/ }
#define DEBUG3(format, ...)     { /**/ }
#define DEBUG4(format, ...)     { /**/ }
#define DEBUG4NT(format, ...)     { /**/ }
#define DEBUG5(format, ...)     { /**/ }
#define DEBUG6(format, ...)     { /**/ }
#endif


#define UNUSED_ARG __attribute__((unused))
#define UNUSED_VAR(x) ((void)(x))

/// Converts a QString into a char* .
/// This function is NOT REENTRANT - make sure not to use it twice in the same set of arguments...
/// (implemented in main.cpp)
extern const char* ASCII(const QString& aQString);


/// defined in main.cpp
extern bool theDisplayFramerate;
extern bool theDrawDebug;
extern bool theDrawPolyOutline;
extern bool theIsCollisionOn;
extern bool theIsLevelEditor;
extern bool theTBECaching;
extern QString theStartFileName;

#endif // TBE_GLOBAL
