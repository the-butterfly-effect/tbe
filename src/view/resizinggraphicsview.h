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

#include <QGraphicsView>
#include <QResizeEvent>
#include <QStatusBar>
#include <QMouseEvent>

#include "AbstractObjectPtr.h"
class EditObjectDialog;
class GameControls;
class GameFlow;
class GameResources;
class GameStateMachine;
class MainWindow;
class QMenu;
class QMenuBar;
class QQuickItem;
class ViewWorld;

#include <memory>

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
    static ResizingGraphicsView *me(void);

    /// only to be called by ViewWorld, to register itself
    /// in the view
    void setViewWorld(ViewWorld *aScenePtr, const QString &aLevelName);

    /// This member does initialisation beyond creation,
    /// i.e. hooking up various UI elements to actions.
    /// @param aMWPtr
    /// @param aMenuBarPtr
    /// @param aMenuControlsPtr
    void setup(MainWindow *aMWPtr, GameFlow *aGFPtr, GameStateMachine *aGSMPtr, QMenuBar *aMenuBarPtr,
               QMenu *anMenuControlsPtr);

    /// @returns a pointer to the GameResourcesDialog.
    /// @note this member is only used to hand a pointer to Level.
    GameResources *getGameResourcesDialogPtr();

    QAction *getFrameRateViewPtr()
    {
        return theFrameRateViewPtr;
    }

//protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:
    /// retransmitted from WinFailDialog
    void signal_actionChooseLevel();
    /// retransmitted from WinFailDialog
    void signal_actionNextLevel();
    /// retransmitted from WinFailDialog
    void signal_actionReplay();
    /// retransmitted from WinFailDialog
    void signal_actionSkipLevel();
    /// retransmitted from GameResources
    void signal_actionReload();

public slots:
    void slot_showGameResourcesDialog();

private:
    GameResources      *theGameResourcesPtr;
    MainWindow         *theMainWindowPtr;
    EditObjectDialog   *theObjectEditorPtr;
    GameControls       *theGameControlsPtr;
    GameFlow           *theGameFlowPtr;
    GameStateMachine   *theGameStateMachinePtr;
    ViewWorld          *theScenePtr;
    QAction            *theFrameRateViewPtr;
    friend class LevelCreator;
};

#endif // RESIZINGGRAPHICSVIEW_H
