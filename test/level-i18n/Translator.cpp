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
#include <QDir>
#include <QCoreApplication>

#include <libintl.h>
#include <clocale>
#include <unistd.h>

Singleton::Translator::Translator()
{
    // nothing to do?
}


QStringList Singleton::Translator::getLanguageList()
{
    QDir myDir = QDir(theBaseTbeQtLocation);
    QStringList myRawList = myDir.entryList(QStringList("tbe_*.qm"), QDir::Readable|QDir::Files);

    // for each of the entries found, strip off the filename stuff
    QStringList myFinalList;
    for(auto& i : myRawList)
    {
        char my5char[6];
        int c=0;
        for(; c<5; c++)
        {
            if ('.' == i.at(c+4))
                break;
            my5char[c]=i.at(c+4).toLatin1();
        }
        my5char[c]='\0';
        QLocale myLocale (my5char);
        QString myLocaleName = QString("%1 = %2").arg(myLocale.name()).arg(QLocale::languageToString(myLocale.language()));
        myFinalList.append(myLocaleName);
    }
    return myFinalList;
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
    QString myCurrentLocaleName = QLocale::system().name();
    DEBUG1("going to set locale '%s'", ASCII(myCurrentLocaleName));
    return setLanguage(myCurrentLocaleName);
}

bool Singleton::Translator::setLanguage(QString aLocale)
{
    DEBUG1ENTRY;
    // **** Only use the part up to the first space
    aLocale = aLocale.section(' ', 0);
    DEBUG3("Loading translator for locale '%s'", ASCII(aLocale));

    // *** For strings from TBE
    theBaseTbeQtLocation = "";
    QString myLocation = "tbe_" + aLocale;
    DEBUG3("Attemp1: load from %s", ASCII(myLocation));
    if (theTbeQtTranslator.load(myLocation))
    {
        DEBUG3("   ... success");
    }
    else
    {
        theBaseTbeQtLocation = "./";
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
    {
        DEBUG1("PROBLEM: translator for TBE is empty");
    }
    else
        QCoreApplication::instance()->installTranslator(&theTbeQtTranslator);

    // *** For strings from Qt itself
    printf("qt locales: %s\n", ASCII(QLibraryInfo::location(QLibraryInfo::TranslationsPath)));
    theQtTranslator.load("qt_" + aLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if (theQtTranslator.isEmpty())
    {
        DEBUG1("PROBLEM: translator for Qt itself is empty");
    }
    else
        QCoreApplication::instance()->installTranslator(&theQtTranslator);

    // *** For strings from our XML Levels (gettext):
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
