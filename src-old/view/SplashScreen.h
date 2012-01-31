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


#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QGraphicsSvgItem>

/** class SplashScreen
  *
  * This class abstracts the actual drawing of objects
  * 
  */

class SplashScreen : public QGraphicsSvgItem
{

	Q_OBJECT
	
	// Constructors/Destructors
	//  
public:
	/**
	 * Simple Constructor
	 */
	SplashScreen (const QString& anSVGFileName);

	/**
	 * Empty Destructor
	 */
	virtual ~SplashScreen ( );

signals:
	void clicked();
	
protected:

	/** overridden from QGraphicsSvgItem
	 *  if the user clicks on the object, we will emit clicked()
	 *  
	 *  @param event the even to handle
	 */
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
};

#endif // SPLASHSCREEN_H
