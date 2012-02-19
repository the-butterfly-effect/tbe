/* The Butterfly Effect
 * This file copyright (C) 2011,2012  Klaas van Gend
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


#if defined(Q_WS_X11) && !defined(NDEBUG)

#include <execinfo.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cxxabi.h>

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

void handleSEGV(int, siginfo_t *, void *)
{
	fprintf(stderr, "Oh boy, a SEGV happened...\n");
	printBacktrace();
	_exit(1);
}


void setupBacktrace(void)
{
	// setup signal handler for SEGV
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handleSEGV;
	sigaction(SIGSEGV, &sa, NULL);
}

#else
void setupBacktrace(void)
{
    ; // nothing to do here :-)
}

#endif // defined Q_WS_X11 && !defined NDEBUG
