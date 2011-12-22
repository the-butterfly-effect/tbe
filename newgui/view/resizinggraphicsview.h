/* The Butterfly Effect
 * This file copyright (C) 2011 Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef RESIZINGGRAPHICSVIEW_H
#define RESIZINGGRAPHICSVIEW_H

#include <QtGui/QGraphicsView>
#include <QtGui/QResizeEvent>

class GameResources;
class QMenu;
class QMenuBar;
class QPropertyAnimation;
class SimulationControls;
class ViewWorld;

class ResizingGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ResizingGraphicsView(QWidget *parent = 0);
    ~ResizingGraphicsView();

    /// only to be called by MainWindow,
    /// clears everything related to the ViewWorld and GameResourcesDialog
    /// and hides the sim controls again.
    void clearViewWorld(void);

    /// only to be called by ViewWorld, to register itself
    /// in the view
    void setViewWorld(ViewWorld* aScenePtr, const QString& aLevelName);

    /// this member does initialisation beyond creation
    /// @param
    /// @param
    void setup(QMenuBar* aMenuBarPtr, QMenu* anMenuControlsPtr);

    void hideSimControls(void);
    void showSimControls(void);

    void clearGameResourcesDialog();
    void createGameResourcesDialog();
    GameResources* getGameResourcesDialogPtr() const;

    void enableGameResourcesDialog();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    virtual void showGRDialog();
    virtual void hideGRDialog();

signals:
//    void hideGameResourcesDialog();

public slots:

private:
    GameResources*      theGameResourcesPtr;
    SimulationControls* theSimControlsPtr;

	QPropertyAnimation* theGRAnimationPtr;
	QAction*            theGRDownActionPtr;
	QAction*            theGRUpActionPtr;
};

#endif // RESIZINGGRAPHICSVIEW_H
