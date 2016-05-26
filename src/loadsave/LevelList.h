/* The Butterfly Effect
 * This file copyright (C) 2009,2015  Klaas van Gend
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

#ifndef LevelList_H
#define LevelList_H

#include <QCoreApplication>
#include <QXmlDefaultHandler>

/// this class contains a list of all levels
class LevelList : protected QXmlDefaultHandler
{
public:
    /// Describes the meta-info for a level in the game
    struct LevelMetaInfo {
        enum Status
        { FRESH, SKIPPED, COMPLETED };

        enum Status theStatus;
        QString     theFileName;
        QString     theTitle;       // translatable
        QString     theDescription; // translatable
    };

    /// Constructor, will open levels description file 'aFileName' in aBaseDir.
    /// @param aBaseDir   Base directory to find aFileName in, also base for all level file name
    /// @param aFileName  Name of the 'levels.xml' file - usually just that.
    LevelList(const QString &aBaseDir, const QString &aFileName);

    /// @returns a COPY of the meta info for level aName (full path!)
    /// @note: if aName doesn't exit, return value will be an empty struct
    LevelMetaInfo getLevelMetaInfo(QString aName);

    /// @returns Full path for the first level in the list.
    /// @note    If there is no level in the list, return value will be empty.
    /// @note    As a side-effect, will reload all Skipped/Completed statuses.
    QString getFirstLevel();

    /// @returns full path for the next level in the list
    /// @note: if aName doesn't exit, return value will be empty
    QString getNextLevel(QString aName);

    /// @returns First non-skipped non-completed level - as a full path.
    /// @note    If there are no such levels left, returns empty.
    /// @note    As a side-effect, will reload all Skipped/Completed statuses.
    QString getNextToPlayLevel();

private:
    // to make sure the below overrides are not accessible
    friend class QXmlSimpleReader;

    /// implemented from QXmlDefaultHandler
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName) override;
    /// implemented from QXmlDefaultHandler
    bool characters(const QString &aStr) override
    {
        currentText += aStr;
        return true;
    }

    /// implemented from QXmlDefaultHandler
    bool fatalError(const QXmlParseException &exception) override;

    /// Update all level statuses for new Skipped or Completed info.
    void updateSkippedCompleted();

private:
    int     theNr;
    QString currentText;
    QString errorStr;

    /// contains the base paths for the levels.xml and the levels
    QString theBaseLevelsDir;

    typedef QList<LevelMetaInfo> MetaInfoList;
    MetaInfoList theMetaList;

    /// Finds aName in theMetaList.
    /// @returns the index to the element containing aName, or -1 if not found
    int findNameInList(const QString &aName);

    /** this class is used after the above LevelList class to quickly retrieve
      * title and description from a level (in the right locale)
      */
    class FastLevelParser : public QXmlDefaultHandler
    {
    public:
        /// empty constructor - Note that the file name is specified on a different call
        FastLevelParser(void) {};

        /// implemented from QXmlDefaultHandler
        virtual bool endElement(const QString &namespaceURI, const QString &localName,
                                const QString &qName) override;

        /// implemented from QXmlDefaultHandler
        virtual bool characters(const QString &aStr) override
        {
            currentText += aStr;
            return true;
        }


        virtual bool startElement(const QString &namespaceURI, const QString &localName,
                                  const QString &qName, const QXmlAttributes &attributes) override;

        virtual QString errorString() const override
        {
            return errorStr;
        }

        QString theTitle; // translatable
        QString theDescription; // translatable

        QXmlAttributes theAttrs;

    private:
        QString currentText;
        QString errorStr;

    }; // end-of FastLevelParser
};

#endif // LevelList_H
