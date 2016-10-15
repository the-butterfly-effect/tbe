/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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

#ifndef GAMEFLOW_H
#define GAMEFLOW_H

#include "GameStateMachine.h"
#include "LevelList.h"
#include <QObject>
#include <memory>

class MainWindow;
class QQuickItem;
class RequestDialog;


struct ListRow : public QObject {
    Q_OBJECT

public:
    ListRow(const QString& aNumber, const QString& aTitle, const QString &aFileName);

    Q_PROPERTY(QString number   MEMBER theNumber   NOTIFY numberChanged)
    Q_PROPERTY(QString title    MEMBER theTitle    NOTIFY titleChanged)
    Q_PROPERTY(QString filename MEMBER theFileName NOTIFY filenameChanged)

    QString theNumber;
    QString theTitle;
    QString theFileName;

signals:
    void numberChanged();
    void titleChanged();
    void filenameChanged();
};


class GameFlow : public QObject
{
    Q_OBJECT

public:
    explicit GameFlow(MainWindow *parent, RequestDialog *anRDPtr);

    GameStateMachine* theGameStateMachinePtr;

    /// TODO/FIXME: parked here, called only from MainWindow
    /// @returns The name of the next level to play, or "" if no more levels.
    QString getNextLevelName(void);


signals:

private slots:
    void slot_levelDeath(void);
    void slot_levelWon(void);

    void slot_onLevelIndexSelected(const QVariant& anIndex);


public slots:
    void slot_clearDialog();
    void slot_showLevelInfoDialog();
    void slot_showChooseLevelDialog();

private:
    /// Generate a (translated) list of all levels for the ChooseLevelDialog.
    /// TODO: fix the return type;
    /// TODO: move to LevelList (duh)
    void generateLevelList();
    QList<QObject*> theLevelStringList;

    /// Initialise the WinFailDialog.
    void setupWinFail(bool isAWin);

    QQuickItem* theDialogPtr;
    MainWindow* theMainWindowPtr;
    RequestDialog* theRequestDialogItfPtr;

    std::unique_ptr<LevelList> theLevelList;
    int theFirstSelectableLevel;
};

#endif // GAMEFLOW_H
