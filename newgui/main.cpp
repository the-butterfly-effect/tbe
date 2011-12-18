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

#include <execinfo.h>
#include <cstdio>
#include <csignal>
#include <cxxabi.h>

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

#if !defined(NDEBUG)

/** Print a demangled stack backtrace of the caller function.
  *
  * Code 'borrowed' from https://idlebox.net/2008/0901-stacktrace-demangled/
  * License: it states "You can freely use it for whatever purpose: download stacktrace.h. I hope you find this utility function useful."
  * The actual code snippet says:
  *    // stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
  *    // published under the WTFPL v2.0
  */
void printBacktrace(void)
{
	static const int max_frames = 63;
	FILE* out = stderr;

	fprintf(out, "Stack trace:\n");

	// storage array for stack trace address data
	void* addrlist[max_frames+1];

	// retrieve current stack addresses
	int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

	if (addrlen == 0) {
		fprintf(out, "  <empty, possibly corrupt>\n");
		return;
	}

	// resolve addresses into strings containing "filename(function+address)",
	// this array must be free()-ed
	char** symbollist = backtrace_symbols(addrlist, addrlen);

	// allocate string which will be filled with the demangled function name
	size_t funcnamesize = 256;
	char* funcname = (char*)malloc(funcnamesize);

	// iterate over the returned symbol lines. skip the first, it is the
	// address of this function.
	for (int i = 1; i < addrlen; i++)
	{
		char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

		// find parentheses and +address offset surrounding the mangled name:
		// ./module(function+0x15c) [0x8048a6d]
		for (char *p = symbollist[i]; *p; ++p)
		{
			if (*p == '(')
				begin_name = p;
			else if (*p == '+')
				begin_offset = p;
			else if (*p == ')' && begin_offset) {
				end_offset = p;
				break;
			}
		}

		if (begin_name && begin_offset && end_offset
			&& begin_name < begin_offset)
		{
			*begin_name++ = '\0';
			*begin_offset++ = '\0';
			*end_offset = '\0';

			// mangled name is now in [begin_name, begin_offset) and caller
			// offset in [begin_offset, end_offset). now apply
			// __cxa_demangle():

			int status;
			char* ret = abi::__cxa_demangle(begin_name,
											funcname, &funcnamesize, &status);
			if (status == 0) {
				funcname = ret; // use possibly realloc()-ed string
				fprintf(out, "  %s : %s+%s\n",
						symbollist[i], funcname, begin_offset);
			}
			else {
				// demangling failed. Output function name as a C function with
				// no arguments.
				fprintf(out, "  %s : %s()+%s\n",
						symbollist[i], begin_name, begin_offset);
			}
		}
		else
		{
			// couldn't parse the line? print the whole line.
			fprintf(out, "  %s\n", symbollist[i]);
		}
	}

	free(funcname);
	free(symbollist);
}

static void handleSEGV(int, siginfo_t *, void *)
{
	fprintf(stderr, "Oh boy, a SEGV happened...\n");
	printBacktrace();
	_exit(1);
}

#endif // !defined NDEBUG

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

#if !defined(NDEBUG)
	// setup signal handler for SEGV
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handleSEGV;
	sigaction(SIGSEGV, &sa, NULL);
#endif

	return a.exec();
}

const char* ASCII(const QString& aQString)
{
		static char myString[256];
		strncpy(myString, aQString.toAscii().constData(), 255);
		return myString;
}
