/* The Butterfly Effect 
 * This file copyright (C) 2009  Klaas van Gend
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

#include "tbe_global.h"
#include "MainWindow.h"
#include "Popup.h"

#include "DrawWorld.h"

//////////////////////////////////////////////////////////////////////////////
// constructors & destructors

MainWindow::MainWindow(QWidget *parent)
     : QMainWindow(parent), theSimStateMachine(&ui)
{                                      
        ui.setupUi(this);              
}                           

//////////////////////////////////////////////////////////////////////////////
// public slots - alphabetical

void MainWindow::on_actionAbout_activated()
{
	Popup::Info("<b>The Butterfly Effect</b><br><br>"
				"An open source game that uses realistic physics"
				" simulations to combine lots of simple mechanical elements"
				" to achieve a simple goal in the most complex way possible.<br><br>"
				"(C) 2009 Peter van Ginneken and Klaas van Gend<br>"
				"Licensed under GPL version 2", this);
}

//////////////////////////////////////////////////////////////////////////////
// public accessor methods

void MainWindow::setScene(DrawWorld* aScene)
{
	ui.graphicsView->setScene(aScene);
	
	// calculate x scaling and y scaling
	QSize myViewSize = ui.graphicsView->size();
	float xScale = (myViewSize.width()-10) / aScene->getWidth();
	float yScale = (myViewSize.height()-10) / aScene->getHeight();

	// and use the least for both X and Y
	if (xScale > yScale)
		xScale = yScale;
	// X horizontal positive to right
	// Y vertical   positive up -> that's why we need the negative
	ui.graphicsView->scale(1.0*xScale, -1.0*xScale);
	
	// TODO FIXME HACK HACK HACK
	// hook up animation to start button
//    QObject::connect(ui.pushButton_Start, SIGNAL(clicked()), aScene, SLOT(timeStep()));
	
	
//	ui.graphicsView->fitInView(aScene->theBackGroundRectPtr);
}

