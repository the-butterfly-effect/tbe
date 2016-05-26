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

#ifndef ChooseLevel_H
#define ChooseLevel_H

#include "animateddialog.h"
#include "LevelList.h"

#include "tbe_paths.h"

#include <QDialog>
#include <QTreeWidget>
#include <QXmlDefaultHandler>

namespace Ui {
class ChooseLevel;
}

class ChooseLevel : public AnimatedDialog
{
    Q_OBJECT
public:
    /// @param aParentPtr
    /// @param isNoShow
    /// @param aDirectory (only used for testing)
    /// @param aFileName  (only used for testing)
    ChooseLevel(ResizingGraphicsView *aParentPtr = 0, bool isNoShow = false,
                const QString &aDirectory = LEVELS_DIRECTORY, const QString &aFileName = "levels.xml");
    virtual ~ChooseLevel();

    /// TODO: move to (singleton version of) LevelList
    /// @returns The name of the next level to play, or "" if no more levels.
    static QString getNextLevelName(void);

signals:
    void loadLevel(const QString &);

protected slots:
    void on_theTreeWidget_itemDoubleClicked()
    {
        on_pushButton_go_clicked();
    }

    void on_pushButton_go_clicked();

    void on_pushButton_cancel_clicked();

protected:
    virtual void changeEvent(QEvent *e) override;

    /// @returns the currently selected level, which should be the first
    /// not-conquered level. If there is none, returns an empty string.
    QString getCurrent(void);

    /// (Re-)fills the treewidget with current info from theLLPtr
    void fillTreeWidget();

private:
    Ui::ChooseLevel *m_ui;

    static std::shared_ptr<LevelList> theLLPtr;
};

#endif // ChooseLevel_H
