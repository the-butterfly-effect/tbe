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

#include <QLibraryInfo>
#include <QCoreApplication>

#include <libintl.h>
#include <clocale>
#include <unistd.h>

Singleton::Translator::Translator()
{
    // nothing to do?
}


const char *Singleton::Translator::getText(const char *aStringToTranslate)
{
    return gettext(aStringToTranslate);
}

const QString Singleton::Translator::getText(const QString &aStringToTranslate)
{
    return gettext(ASCII(aStringToTranslate));
}


bool Singleton::Translator::init()
{
    DEBUG1ENTRY;
    //** Strings in source code or XML are UTF-8, make sure Qt understands
    theTextCodecPtr = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(theTextCodecPtr);

    // retrieve the locale and set it
    return setLanguage(QLocale::system().name());
}

bool Singleton::Translator::setLanguage(const QString& aLocale)
{
    DEBUG1ENTRY;
    DEBUG3("Loading translator for locale '%s'", ASCII(aLocale));

    // for strings from TBE
    QString myLocation = "tbe_" + aLocale;
    DEBUG3("Attemp1: load from %s", ASCII(myLocation));
    if (theTbeQtTranslator.load(myLocation))
    {
        DEBUG3("   ... success");
    }
    else
    {
        myLocation = "./tbe_" + aLocale;
        DEBUG3("Attemp2: load from %s", ASCII(myLocation));
        if (theTbeQtTranslator.load(myLocation))
        {
            DEBUG3("   ... success");
        }
        else
        {
            DEBUG2("PROBLEM: no translator for %s loaded", ASCII(aLocale));
        }
    }
    if (theTbeQtTranslator.isEmpty())
        DEBUG1("PROBLEM: translator is empty");
    QCoreApplication::instance()->installTranslator(&theTbeQtTranslator);
    // for strings from Qt itself
    theQtTranslator.load("qt_" + aLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::instance()->installTranslator(&theQtTranslator);

    // for gettext:
    bindtextdomain("tbe_levels", get_current_dir_name());
    textdomain( "tbe_levels");

    // because putenv 'eats' the memory, strdup myBuffer before putenv'ing it
    char myBuffer[255];
    snprintf(myBuffer, 254, "LANGUAGE=%s", ASCII(aLocale));
    putenv(strdup(myBuffer));

    return true;
}


Singleton::Translator &Singleton::Translator::me()
{
    // This is enough for a singleton in C++11 and beyond
    // (per C++11 standard, paragraph 6.7 [stmt.dcl] line 4)
    static Translator mySingleton;
    return mySingleton;
}
