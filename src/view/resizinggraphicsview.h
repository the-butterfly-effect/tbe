/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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

#ifndef RESIZINGGRAPHICSVIEW_H
#define RESIZINGGRAPHICSVIEW_H

#include <QtGui/QGraphicsView>
#include <QtGui/QResizeEvent>

#include "AbstractObjectPtr.h"
class EditObjectDialog;
class GameResources;
class MainWindow;
class QMenu;
class QMenuBar;
class SimulationControls;
class ViewWorld;
class WinFailDialog;

class ResizingGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ResizingGraphicsView(QWidget *aParentPtr = 0);
    ~ResizingGraphicsView();

    /// only to be called by MainWindow,
    /// clears everything related to the ViewWorld and GameResourcesDialog
    /// and hides the sim controls again.
    void clearViewWorld(void);

    /// used by various components to get to the RSGView
    static ResizingGraphicsView* me(void);

    /// only to be called by ViewWorld, to register itself
    /// in the view
    void setViewWorld(ViewWorld* aScenePtr, const QString& aLevelName);

    /// This member does initialisation beyond creation,
    /// i.e. hooking up various UI elements to actions.
    /// @param aMWPtr
    /// @param aMenuBarPtr
    /// @param aMenuControlsPtr
    void setup(MainWindow* aMWPtr, QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr);

    /// @returns a pointer to the GameResourcesDialog.
    /// @note this member is only used to hand a pointer to Level.
    GameResources* getGameResourcesDialogPtr() const;

    QAction* getFrameRateViewPtr()
    { return theFrameRateViewPtr; }

//protected:
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void slot_levelDeath(void);
    void slot_levelWon(void);

    void slot_actionChooseLevel();
    void slot_actionNextLevel();
    void slot_actionReplay();

public slots:
    void slot_actionSkipLevel();
    void slot_showEditObjectDialog(AbstractObjectPtr anAOPtr);
    void slot_editObjectDialog_destroyed(void);

private:
    GameResources*      theGameResourcesPtr;
    MainWindow*         theMainWindowPtr;
    EditObjectDialog*   theObjectEditorPtr;
    SimulationControls* theSimControlsPtr;
    ViewWorld*          theScenePtr;
    WinFailDialog*      theWinFailDialogPtr;

    QAction*            theFrameRateViewPtr;
};

#endif // RESIZINGGRAPHICSVIEW_H
