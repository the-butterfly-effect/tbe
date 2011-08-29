/* The Butterfly Effect
 * This file copyright (C) 2009,2011 Klaas van Gend
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

#include <cassert>

#include "tbe_global.h"
#include "ImageStore.h"

#include <QFile>
#include <QPainter>
#include <QApplication>
#include <QSvgRenderer>
#include <QPixmap>

// TODO: remove this
#define TESTCODE


#ifndef TESTCODE
#include "Level.h"

static
#endif
		ImageRendererStore* theImageRendererStorePtr = NULL;

static const char* NOTFOUND="NotFound";


ImageRendererStore::ImageRendererStore()
{
	assert (theImageRendererStorePtr == NULL);
	// nothing to do here...
}


QString ImageRendererStore::getFilePath(const QString& anImageName, const QString& anExtension) const
{
	// let's try to find the file and create the renderer...
	QStringList mySearchPath = (BINARY_DIRECTORY+":"+IMAGES_DIRECTORY)
							   .split(":",QString::SkipEmptyParts);
	// add the local directory of the level file to the search path - at the beginning...
	// FIXME/TODO: test code here!!!
	mySearchPath.push_front(".");
	mySearchPath.push_back("../images");
	QStringList::iterator i;
	for (i=mySearchPath.begin(); i!=mySearchPath.end(); ++i)
	{
		QString myFullName = (*i) + "/" + anImageName + anExtension;
		DEBUG5("searching for '%s' ... ", ASCII(myFullName));
		if (QFile::exists(myFullName) == false)
		{
			DEBUG5("not found\n");
			continue;
		}
		DEBUG5("found.\n");
		return myFullName;
	}
	return QString();
}


ImageRenderer* ImageRendererStore::getImageRenderer(
		const QString& anImageName,
		bool isNullOnNotFound)
{
	// if anImageName is in store, we're done quickly :-)
	RendererMap::iterator mySearchResult = me().theRendererMap.find(anImageName);
	if (mySearchResult != me().theRendererMap.end())
	{
		DEBUG5("Found cached ImageRenderer for '%s'\n", ASCII(anImageName));
		return mySearchResult.value();
	}

	// Not in the map yet?
	// Let's attempt to find the image (svg/png/jpg/jpeg) then...
	QStringList myExtensions(QString(".png;.svg;.jpg;.jpeg;").split(";"));
	QString myFullName;
	foreach(QString i, myExtensions)
	{
		myFullName = me().getFilePath(anImageName, i);
		if (myFullName.isEmpty()==false)
		{
			ImageRenderer* myNewRendererPtr = new ImageRenderer(myFullName);
			me().theRendererMap[anImageName] = myNewRendererPtr;
			return myNewRendererPtr;
		}
	}

	// if isNullOnNotFound is set, we now return NULL
	// otherwise, we'll try to find the NotFound image and return its renderer
	if (isNullOnNotFound)
		return NULL;
	if (anImageName==NOTFOUND)
		return NULL;
	return getImageRenderer(NOTFOUND, true);
}

QSvgRenderer* ImageRendererStore::getSvgRenderer(const QString& anImageName,
										   bool isNullOnNotFound)
{
	ImageRenderer* myRenderer = getImageRenderer(anImageName);
	if (myRenderer->theSvg != NULL)
		return myRenderer->theSvg;
	if (isNullOnNotFound)
		return NULL;
	else
		return getSvgRenderer(NOTFOUND, true);
}

QPixmap* ImageRendererStore::getPixmap(const QString& anImageName)
{
	ImageRenderer* myRenderer = getImageRenderer(anImageName, true);
	if (myRenderer != NULL)
	{
		if (myRenderer->theBitmap)
			return myRenderer->theBitmap;
		// Ow shit, user requests a Pixmap but we only have an SVG...
		// Let's assume some bitmap size and add it to our renderer
		QSize myBitmapSize(384,384);
		QPixmap* myNewPixmapPtr = new QPixmap(
				getQIcon(anImageName, myBitmapSize)
				.pixmap(myBitmapSize, QIcon::Normal, QIcon::On));
		myRenderer->theBitmap = myNewPixmapPtr;
		return myNewPixmapPtr;
	}
	return NULL;
}

QIcon ImageRendererStore::getQIcon(const QString& anImageName, const QSize& aSize)
{
	QPixmap myPixmap(aSize);
	myPixmap.fill(QColor(255,255,255,0));

	ImageRenderer* myRendererPtr = getImageRenderer(anImageName, false);
	if (myRendererPtr != NULL)
	{
		QPainter myPainter;
		myPainter.begin(&myPixmap);
		myPainter.setRenderHint(QPainter::Antialiasing);
		myRendererPtr->render(&myPainter);
		myPainter.end();
	}
	QIcon myIcon(myPixmap);
	return myIcon;
}



ImageRendererStore& ImageRendererStore::me()
{
	if (theImageRendererStorePtr==NULL)
		theImageRendererStorePtr = new ImageRendererStore();
	return *theImageRendererStorePtr;
}



//----------------------------------------------------------------------------

ImageRenderer::ImageRenderer(QString aFullImagePath)
		: theBitmap(NULL), theSvg(NULL)
{
	// validation of the path should have happened already at this point
	assert(QFile::exists(aFullImagePath)!= false);

	if (aFullImagePath.contains(".svg"))
	{
		theSvg = new QSvgRenderer(aFullImagePath);
		assert(theSvg != NULL);
	}
	else
	{
		theBitmap = new QPixmap(aFullImagePath);
		assert(theBitmap != NULL);
	}
}

void ImageRenderer::render (QPainter* aPainter, const QRectF& aBounds )
{
	// it is possible to have both - we prefer the Svg over the Bitmap
	if (theSvg)
		theSvg->render(aPainter, aBounds);
	if (theBitmap)
		aPainter->drawPixmap(aBounds, *theBitmap, theBitmap->rect());
}

void ImageRenderer::render (QPainter* aPainter)
{
	// it is possible to have both - we prefer the Svg over the Bitmap
	if (theSvg)
		theSvg->render(aPainter);
	if (theBitmap)
	{
		QRect myBounds = aPainter->viewport();
		aPainter->drawPixmap(myBounds, *theBitmap, theBitmap->rect());
	}
}

QSize ImageRenderer::defaultSize(void)
{
	if (theSvg)
		return theSvg->defaultSize();
	if (theBitmap)
		return theBitmap->size();
	return QSize(1,1);
}
