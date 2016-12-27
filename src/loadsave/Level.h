/* The Butterfly Effect
 * This file copyright (C) 2009,2010  Klaas van Gend
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

#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDomElement>
#include "tbe_global.h"
#include "AbstractObject.h"
#include "Toolbox.h"

// Forward Declarations:
class Hint;
class MainWindow;
class World;

/**
  * class Level
  *
  * the level reads the objects from a file and creates the world containing
  * the various objects
  *
  * Level inherits QObject to make i18n of error messages easier
  */

class Level :  public QObject
{
    Q_OBJECT

    // Constructors/Destructors
    //
public:

    /// The only constructor
    /// @param aToolboxPtr  pointer to the Toolbox to add the items to
    explicit Level ( Toolbox* aToolboxPtr);

    /**
     * Empty Destructor
     */
    virtual ~Level ( );

public:
    // Public accessor methods
    //

    // Subsequent statuses are stronger and when set, replace a lower one.
    enum LevelStatus {
        FRESH     = 0,  // Level hasn't been opened yet
        PLAYED    = 5,  // Level has been opened, but not skipped or won
        SKIPPED   = 6,  // Level has been played and skipped
        COMPLETED = 9   // Level has been won
    };

    /// @returns the LevelStatus of the current level.
    static LevelStatus getLevelStatus();

    /// @returns the LevelStatus of the named level.
    static LevelStatus getLevelStatus(const QString &aLevelName);

    /// Sets the LevelStatus for the current level.
    /// @note: will not overwrite an existing higer status.
    static void setLevelStatus(LevelStatus aNewLevelStatus);

    /// Sets the LevelStatus for the current level.
    /// @note: will not overwrite an existing higer status.
    static void setLevelStatus(const QString &aLevelName,
                               LevelStatus aNewLevelStatus);

    /// @returns the Level's title (NOT translated).
    QString getName ( ) const
    {
        return theLevelName;
    }

    World *getTheWorldPtr(void) const
    {
        return theWorldPtr;
    }

    /** Open file containing a level definition, parse it,
     *  and fill the World object that was created in the constructor.
     *  Also:
     *
     * @param aFileName file to parse and populate Level with.
     * @return empty string is returned if loading was successful, otherwise
     *          the return will contain the i18n'ed error message
     */
    QString load(const QString &aFileName);

    /** save the Level to a file
     *  the file name must be unique - overwriting is not allowed here
     *
     * @param aFileName file to serialize the Level/World to.
     * @return false if saving failed - error message will be set.
     */
    bool save(const QString &aFileName);

    /// @returns the path to the file that describes the current level
    static QString getPathToLevelFile(void);

    /// @returns a pointer to the Hint at index or nullptr if there are no
    ///          more hints.
    /// @note that the hint does not change owners: still belongs to the Level
    Hint *getHint(int anIndex);

    /// The File name of the Level is used in recording winning the level.
    /// @returns the FULL PATH of the file that describes the current level.
    static QString getLevelFileName(void);

    /// sets the name of the file that describes the current level
    void setLevelFileName(const QString &aName);

    QString theLevelName; // translatable
    QString theLevelAuthor;
    QString theLevelLicense;
    QString theLevelDescription; // translatable
    QString theLevelDate;

protected:
    // TODO FIXME: move these two somewhere else so we no longer need the #include for QDomElement here
    void addTextElement(QDomElement aParent, const QString &anElementName, const QString &aText) const;
    void addAbstractObject(QDomElement aParent, const AbstractObject &anObjectRef) const;
    void addHint(Hint *aHintPtr);

private:
    World *theWorldPtr;

    typedef QList<Hint *>  HintList;
    HintList theHintPtrList;

    Toolbox* theToolboxPtr;

    friend class EditLevelProperties;

private:
    // prevent copy constructor / assignment operator
    Level(const Level &) = delete;
    const Level &operator= (const Level &) = delete;
};

#endif // LEVEL_H
