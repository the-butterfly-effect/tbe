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

#include <assert.h>

#include "tbe_global.h"
#include "ImageStore.h"
#include "Level.h"

#include <QFile>
#include <QPainter>

static ImageStore* theImageStorePtr = NULL;

static const char* NOTFOUND="NotFound";


// Constructors/Destructors
//

ImageStore::ImageStore()
{
	// nothing to do here...
}

//
// Methods
//


// Accessor methods
//

ImageStore& ImageStore::me()
{
	if (theImageStorePtr==NULL)
	theImageStorePtr = new ImageStore();
	return *theImageStorePtr;
}

// Other methods
//


QString ImageStore::getFilePath(const QString& anImageName, const QString& anExtension) const
{
	// let's try to find the file and create the renderer...
	QStringList mySearchPath = QString(".:images:images/icons:images/objects:"
									   "images/src:"
									   "images/textures").split(":",QString::SkipEmptyParts);
	// add the local directory of the level file to the search path - at the beginning...
	mySearchPath.push_front(Level::getPathToLevelFile());
	QStringList::iterator i;
	for (i=mySearchPath.begin(); i!=mySearchPath.end(); ++i)
	{
		QString myFullName = (*i) + "/" + anImageName + anExtension;
		DEBUG5("searching for '%s' ... ", myFullName.toAscii().constData());
		if (QFile::exists(myFullName) == false)
		{
			DEBUG5("not found\n");
			continue;
		}
		return myFullName;
	}
	return QString();
}

QPixmap* ImageStore::getMePNGPixmap(QString anImageName)
{
	DEBUG5("getMePNGPixmap(\"%s\")\n", ASCII(anImageName));
	if (anImageName.isEmpty())
		anImageName = NOTFOUND;

	assert(theImageStorePtr != NULL);

	// if anImageName is in store, we're done quickly :-)
	PixmapMap::iterator mySearchResult = thePixmapMap.find(anImageName);
	if (mySearchResult != thePixmapMap.end())
	{
		DEBUG5("found (cached) QPixmap for '%s'\n", anImageName.toAscii().constData());
		return mySearchResult.value();
	}

	// so, it's not in our cache yet...
	// let's try to find the file and create the pixmap...
	QString myFullName = getFilePath(anImageName, ".png");
	QPixmap* myPtr = new QPixmap(myFullName);
	if (myPtr == NULL)
		return NULL;
	if (myPtr->isNull())
		return NULL;
	thePixmapMap[anImageName]=myPtr;
	DEBUG5("my QPixmap* is %p\n", myPtr);
	return myPtr;
}

QIcon ImageStore::getMeQIcon(const QString& anImageName, const QSize& aSize)
{
	DEBUG5("getMeQIcon(\"%s\")\n", ASCII(anImageName));
	QPixmap myPixmap(aSize);
	myPixmap.fill(QColor(255,255,255,0));

	// first try SVG
	// if no SVG found, try PNG
	// if no PNG found, retry the SVG but allow the "NotFound" image
	QSvgRenderer* myRenderer = getMeRenderer(anImageName, false);
	QPixmap* myPNG;
	if (myRenderer == NULL)
	{
		myPNG = getMePNGPixmap(anImageName);
		if (myPNG==NULL)
			myRenderer = getMeRenderer(anImageName);
		else
			myPixmap = *myPNG;
	}

	// no renderer found? return an empty icon...
	if (myRenderer != NULL)
	{
		// cool! we have a renderer. Now let's render!
		QPainter myPainter;
		myPainter.begin(&myPixmap);
		myPainter.setRenderHint(QPainter::Antialiasing);
		myRenderer->render(&myPainter);
		myPainter.end();
	}

	QIcon myIcon(myPixmap);
	return myIcon;
}


QSvgRenderer* ImageStore::getMeRenderer(const QString& anImageName, bool returnNotFound)
{
	if(anImageName.isEmpty())
		return getMeRenderer(NOTFOUND);
	assert(theImageStorePtr != NULL);

	// if anImageName is in store, we're done quickly :-)
	RendererMap::iterator mySearchResult = theRendererMap.find(anImageName);
	if (mySearchResult != theRendererMap.end())
	{
		DEBUG5("found (cached) QSvgRenderer for '%s'\n", anImageName.toAscii().constData());
		return mySearchResult.value();
	}

	// so, it's not in our cache yet...
	// let's try to find the file and create the renderer...
	QString myFullName = getFilePath(anImageName, ".svg");
	QSvgRenderer* myPtr = new QSvgRenderer(myFullName);
	if (myPtr == NULL)
		return NULL;
	if (myPtr->isValid()==false)
	{
		if (anImageName != NOTFOUND && returnNotFound)
			return getMeRenderer(NOTFOUND);
		else
			return NULL;
	}
	theRendererMap[anImageName]=myPtr;
	DEBUG5("my Renderer is %p\n", myPtr);
	return myPtr;
}


