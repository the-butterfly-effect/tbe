/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#ifndef GAMERESOURCES_H
#define GAMERESOURCES_H

#include <QStateMachine>
#include <QWidget>

namespace Ui {
    class GameResources;
}

class AbstractObject;
class Level;
#include "LocalString.h"
class QMenuBar;
class QGraphicsScene;
#include "resizinggraphicsview.h"
#include "ToolboxGroup.h"
class ViewWorld;


/// This class holds the toolbox, the game info and (in the future)
/// the button for hints. It is displayed when you click on the appropriate
/// action in the menu bar and/or at the start of a level.
///
/// Usually this class is owned by the resizingsgraphicsview of MainWindow,
/// which this class is drawn on top of (like an overlay).
class GameResources : public QWidget
{
    Q_OBJECT

public:
    explicit GameResources(ResizingGraphicsView* aRSGVPtr);
    ~GameResources();

    /// Called by the ResizingGraphicsView so we can accomodate for
    /// the change in scale of the toolboxview.
    /// @param aTransformMatrix the transform matrix used by the parent.
    void parentResize(const QTransform& aTransformMatrix);

    /// set the Level where we're going to get our info from:
    /// i.e. level name, author but also the contents of the Toolbox view
    void setLevelPtr(Level* aLevelPtr);

signals:
    void hideMe();

protected:
    /// ill-named standard Qt member to change the i18n of the dialog
    virtual void changeEvent(QEvent *e);

private slots:
    void on_theOKButton_clicked();

private:
    Ui::GameResources *ui;

    QGraphicsScene* theToolboxPtr;

    /// pointer to Level instance, which contains level names and such
    Level* theLevelPtr;

    /// pointer to the ViewWorld i.e. the main scene of the game
    ViewWorld* theViewWorldPtr;

    /// GraphicsView holding the ViewWorld, we are an overlay to that View
    ResizingGraphicsView* theParentPtr;

	/// used for the animation
	QStateMachine theStateMachine;

};

#endif // GAMERESOURCES_H
