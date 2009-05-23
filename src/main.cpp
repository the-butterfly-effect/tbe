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


#include "MainWindow.h"

#include "ode/ode.h"

#include <QtGui>

#include "World.h"
#include "BaseObject.h"
#include "BowlingBall.h"
#include "BowlingPin.h"


// the verbosity for all logging - by default defined at 5 (most logging)
int theVerbosity = 5;


int main(int argc, char **argv)
{
	// init Qt (graphics toolkit) - www.qtsoftware.com
	QApplication app(argc, argv);
	// init OpenDE (physics library) - opende.sf.net
    dInitODE ();

    MainWindow myMain;
    myMain.show();
    
    World myWorld;
    
    BowlingBall myBall;
    myBall.setTheCenter( Position(0.5, 2, 0) );
    myBall.reset();
    
    BowlingPin myPin;
    myPin.setTheCenter( Position(1, 0.20, 0) );
    myPin.reset();
    
    
    ///////////////////////////////////
    // run simulation - still has to move to World class
    int i;
    for(i=0; i<200;i++)
    {
    	static float mytime=0.0;
    	
		mytime += myWorld.simStep();

		// redraw sphere at new location
	    const dReal *pos1 = dGeomGetPosition (myBall.getTheGeomID());
	    const dReal *pos2 = dGeomGetPosition (myPin.getTheGeomID());
		
		printf("%f \t %f\t%f\t%f \t %f\t%f\t%f\n", mytime, 
				pos1[0], pos1[1], pos1[2],
				pos2[0], pos2[1], pos2[2]);
    }
    
    
    // clean up
    dCloseODE();
    return 0;
    ///////////////////////////////////
//    return app.exec();
}
