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

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QTranslator>
#include <QTextCodec>
#include <QStringList>
#include <QLocale>

namespace Singleton
{

/// Singleton class to govern handing out translated strings
///
/// In addition to handling the setting up of Qt's i18n mechanism,
/// this class also handles translating of strings from the level XML
/// through the gettext() mechanism. It is also capable of listing
/// present languages and switching translations on-the-fly.
class Translator
{
public:
    /// Singleton: call this member to get a reference to the real instance.
    static Translator& me();

    //------------------------------
    /// Retrieves a list of possible language settings
    //QStringList getLanguageList();

    /// Use to get the current translation for a level XML string.
    /// @param aStringToTranslate string to translate
    /// @returns translated string according to the currently set locale if possible
    const char* getText(const char* aStringToTranslate);

    /// Use to get the current translation for a level XML string.
    /// @param aStringToTranslate string to translate
    /// @returns translated string according to the currently set locale if possible
    const QString getText(const QString& aStringToTranslate);

    /// Called at application startup to initialise the locales.
    bool init();

    /// Can be called at any time to switch the language.
    /// (is called automatically by init())
    /// @param aLocale string of the format like "nl_BE",
    ///                 where the lower case indicates the primary language and
    ///                 the uppercase denotes the local variant.
    bool setLanguage(const QString& aLocale);

private:
    /// private constructor: we're a singleton after all
    Translator();

    // no need for virtual destructor, probably no need for destructor at all
    //~Translator();


    QTextCodec* theTextCodecPtr;
    /// QTranslator for all TBE C++ code strings
    QTranslator theTbeQtTranslator;
    /// QTranslator for the Qt internal strings
    QTranslator theQtTranslator;
};

}; // end-of-namespace Singleton

#define TheTranslator Singleton::Translator::me()
#define TheGetText(A) Singleton::Translator::me().getText(A)

#endif // TRANSLATOR_H
