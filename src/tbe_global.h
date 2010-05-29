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

#ifndef TBE_GLOBAL
#define TBE_GLOBAL

#define APPNAME "The Butterfly Effect"
#define APPRELEASE "0.0.3"
/* for your special versioning needs, normally empty */
#ifndef APPFLAVOUR
#define APPFLAVOUR ""
#endif
#include <QtGlobal>
#include <QString>

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
#include <stdio.h>
#include <time.h>
extern int theVerbosity;	// actually defined in main.cpp
#define MAX_VERBOSITY 6
// note: the below is actually GCC-specific, see "info:/gcc/Variadic Macros"
#define DEBUG1(format, ...)	{if (theVerbosity>=1) printf("**** t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG2(format, ...)	{if (theVerbosity>=2) printf("***  t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG3(format, ...)	{if (theVerbosity>=3) printf("**   t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG4(format, ...)	{if (theVerbosity>=4) printf("*    t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG4NT(format, ...)	{if (theVerbosity>=4) printf(format, ## __VA_ARGS__);}
#define DEBUG5(format, ...)	{if (theVerbosity>=5) printf("     t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
#define DEBUG6(format, ...)	{if (theVerbosity>=6) printf("     t=%03ld: " format, time(NULL)%1000, ## __VA_ARGS__);}
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
extern bool theIsLevelEditor;
extern bool theDrawDebug;
extern bool theTBECaching;
extern bool theDisplayFramerate;

#endif // TBE_GLOBAL
