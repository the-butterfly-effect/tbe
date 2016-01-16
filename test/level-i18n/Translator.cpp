/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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

#include "tbe_global.h"
#include "Translator.h"

#include "QLibraryInfo"
#include "QCoreApplication"

Singleton::Translator::Translator()
{
    // nothing to do?
}


Singleton::Translator &Singleton::Translator::me()
{
    // This is enough for a singleton in C++11 and beyond
    // (per C++11 standard, paragraph 6.7 [stmt.dcl] line 4)
    static Translator mySingleton;
    return mySingleton;
}

bool Singleton::Translator::init()
{
    //** Strings in source code or XML are UTF-8, make sure Qt understands
    theTextCodecPtr = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(theTextCodecPtr);

    QString myLocale = QLocale::system().name();
    DEBUG3("Loading translator for locale '%s'", ASCII(myLocale));

    // for strings from TBE
    QString myLocation = "tbe_" + myLocale;
    DEBUG3("Attemp1: load from %s", ASCII(myLocation));
    if (theTbeQtTranslator.load(myLocation))
    {
        DEBUG3("   ... success");
    }
    else
    {
        myLocation = "./tbe_" + myLocale;
        DEBUG3("Attemp2: load from %s", ASCII(myLocation));
        if (theTbeQtTranslator.load(myLocation))
        {
            DEBUG3("   ... success");
        }
        else
        {
            DEBUG2("PROBLEM: no translator for %s loaded", ASCII(myLocale));
        }
    }
    if (theTbeQtTranslator.isEmpty())
        DEBUG1("PROBLEM: translator is empty");
    QCoreApplication::instance()->installTranslator(&theTbeQtTranslator);
    // for strings from Qt itself
    theQtTranslator.load("qt_" + myLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::instance()->installTranslator(&theQtTranslator);


    return true;
}
