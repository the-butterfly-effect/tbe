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

#include <QApplication>

#include "ChooseLevel.h"
#include "tbe_global.h"


// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 4;



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ChooseLevel myMain;

	if (myMain.exec() == QDialog::Accepted)
		printf("user selected: '%s'\n", ASCII(myMain.getCurrent()));
	else
		printf("user clicked cancel\n");

	return 0;
}


const char* ASCII(const QString& aQString)
{
		return aQString.toAscii().constData();
}
