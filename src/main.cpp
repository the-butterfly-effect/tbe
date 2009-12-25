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
// TODO/FIXME: this should go into preferences or so at some point
bool theIsLevelEditor = false;

// this variable defines whether we are showing Box2D debugging info or not
// TODO/FIXME: this should go into preferences or so at some point
bool theDrawDebug = false;

int main(int argc, char **argv)
{
	// init Qt (graphics toolkit) - www.qtsoftware.com
	QApplication app(argc, argv);

	// read the locale from the environment and set the language...
	// TODO: This is fairly basic - I probably need to "borrow"
	//       the (maybe too flexible) code from umtsmon
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("i18n/tbe_") + locale);
	app.installTranslator(&translator);

	QCoreApplication::setOrganizationName("Sourceforge.net");
	QCoreApplication::setOrganizationDomain("tbe.sf.net");
	QCoreApplication::setApplicationName("The Butterfly Effect");

	{
		QSettings mySettings;
		printf("using settings from: \"%s\"\n", ASCII(mySettings.fileName()));
	}


	// setup main window
	MainWindow myMain;
	myMain.show();

	// run the main display loop    
	int myReturn=app.exec();
    
	return myReturn;
}


const char* ASCII(const QString& aQString)
{
	return aQString.toAscii().constData();
}
