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

/// Singleton class to govern handing out translated strings and switching
/// of languages.
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

    /// Returns the currently selected language, using the same syntax as
    /// getLanguageList():   "en_US = English"
    QString getCurrentLanguage();

    /// Retrieves a list of possible language settings, including en_US.
    /// Every string in this list looks like: "nl_NL = dutch"
    QStringList getLanguageList();

    /// Use to get the current translation for a level XML string.
    /// @param aStringToTranslate string to translate
    /// @returns translated string according to the currently set locale if possible
    /// @note please refer to the convenience macro TheGetText() below
    const char* getText(const char* aStringToTranslate);

    /// Use to get the current translation for a level XML string.
    /// @param aStringToTranslate string to translate
    /// @returns translated string according to the currently set locale if possible
    /// @note please refer to the convenience macro TheGetText() below
    const QString getText(const QString& aStringToTranslate);

    /// Called at application startup to initialise the locales.
    bool init();

    /// Can be called at any time to switch the language.
    /// (is called automatically by init())
    /// @param aLocale String of the format like "nl_BE",
    ///                 where the lower case indicates the primary language and
    ///                 the uppercase denotes the local variant.
    ///                 (longer string will be cut off)
    /// @returns True when it was possible to load a translator for aLocale.
    bool setLanguage(const QString& aLocale);

private:
    /// Private constructor: we're a singleton after all.
    Translator();

    // No need for virtual destructor, probably no need for destructor at all.
    //~Translator();

    /// No copy constructor.
    Translator(const Translator&) = delete;
    /// No assignment operator.
    const Translator& operator= (const Translator&) = delete;

    /// Attempts to load a <aPath>/tbe_<aLocale>.qm file.
    /// When successful, also sets variable theBaseTbeQtLocation.
    /// @param aPath
    /// @param aLocale
    /// @returns true when successful, false when not.
    bool attemptTbeQtTranslatorLoad(const QString& aPath,
                                    const QString& aLocale);

    /// Set the language for gettext messages to aLocale.
    void setLanguageGettext(const QString &aLocale);
    /// Set the language for the Qt library messages to aLocale.
    bool setLanguageQtIself(const QString& aLocale);

    /// QTextCodec pointer for handling all input (set to UTF-8 by init()).
    QTextCodec* theTextCodecPtr;
    /// QTranslator for all TBE C++ code strings.
    QTranslator theTbeQtTranslator;
    /// QTranslator for the Qt internal strings.
    QTranslator theQtTranslator;

    /// Path to directory that contains find the TBE translations.
    QString theBaseTbeQtLocation;

    /// String of the current language setting
    /// e.g. "nl_NL" for dutch. In some special cases, this string is empty.
    QString theCurrentLanguageString;
};

}; // end-of-namespace Singleton

/// Convenience function macro to call members of TheTranslator singleton
#define TheTranslator Singleton::Translator::me()
/// Convenience function macro to call TheTranslator::getText()
#define TheGetText(A) Singleton::Translator::me().getText(A)

#endif // TRANSLATOR_H
