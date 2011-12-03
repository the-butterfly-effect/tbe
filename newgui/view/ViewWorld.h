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

#ifndef VIEWWORLD_H
#define VIEWWORLD_H

#include <QtGui/QGraphicsScene>

class MainWindow;
class World;

/** class ViewWorld
  * This class contains the View* objects and is the QGraphicsScene.
  * TODO/FIXME: validate & list other jobs:
  * //It contains the timers that run the simulation.
  * //It also maintains the undo stack and handles drag-n-drop from the toolbox.
  * //Lastly, it implements the debug callbacks from Box2D to draw outlines.
  */
class ViewWorld : public QGraphicsScene
{
    Q_OBJECT

public:
	explicit ViewWorld (MainWindow* aMainWindowPtr, World* aWorldPtr);

	// Public accessor methods
	//

	qreal getWidth();
	qreal getHeight();

	// QGraphicsScene events
	//

	virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );

signals:

public slots:

private:
	// Private attributes
	//

	MainWindow* theMainWindowPtr;
	World* theWorldPtr;

private:
    void initAttributes ( ) ;

    // keep this one last, it kills copy constructors & assignment operators
    Q_DISABLE_COPY ( ViewWorld );
};

#endif // VIEWWORLD_H
