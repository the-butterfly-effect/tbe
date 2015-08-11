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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "tbe_global.h"
#include "tbe_paths.h"
#include "ImageCache.h"
#include "Level.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtGui/QPainter>
#include <QtGui/QPixmapCache>
#include <QtSvg/QSvgRenderer>

//
QSize ImageCache::theDefaultSize(160,160);


bool ImageCache::getPixmap(const QString& anImageBaseName,
                           QPixmap* anOutputPixmapPtr)
{
    return getPixmap(anImageBaseName, theDefaultSize, anOutputPixmapPtr);
}

bool ImageCache::getPixmap(const QString& anImageBaseName,
                           const QSize& aSize,
                           QPixmap* anOutputPixmapPtr)
{
    DEBUG5ENTRY;

    Q_ASSERT(anImageBaseName.isEmpty() == false);
    if (anImageBaseName.isEmpty())
        return false;

	// TODO/FIXME:
	// prefix size to pixmapname for cache


	// only names, no paths, no extensions
	Q_ASSERT(anImageBaseName.contains(".")==false);
	Q_ASSERT(anImageBaseName.contains("/")==false);

//	// is the image present in the cache?
//	if (QPixmapCache::find(anImageBaseName, anOutputPixmapPtr))
//	{
//		DEBUG4("Image '%s' found in image cache!", ASCII(anImageBaseName));
//		// is the image the right size?
//		if (anOutputPixmapPtr->size()!=aSize && aSize!=theDefaultSize)
//		{
//			*anOutputPixmapPtr = anOutputPixmapPtr->scaled(aSize, Qt::IgnoreAspectRatio);
//		}
//		return true;
//	}

	// No, it isn't. Let's try to load the image.
	QString myFullPathName;
	QPixmap myTempPixmap;

	// we have 3 locations to search for:
	QStringList mySearchPath;
	mySearchPath << IMAGES_DIRECTORY;
	mySearchPath << ":";	// this is Qt-speak for compiled-in resources
	mySearchPath << Level::getPathToLevelFile();
	for (int i=0; i<mySearchPath.count()+1; i++)
	{
		// if i equals that, we know we have exhausted our search paths
		if (i==mySearchPath.count())
		{
			myFullPathName = IMAGES_DIRECTORY + "/NotFound.png";
			DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
			break;
		}

		myFullPathName = QString(mySearchPath[i] + "/%1.png").arg(anImageBaseName);
		DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
		if (QFile::exists(myFullPathName))
			break;

		myFullPathName = QString(mySearchPath[i] + "/%1.jpg").arg(anImageBaseName);
		DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
		if (QFile::exists(myFullPathName))
			break;

		myFullPathName = QString(mySearchPath[i] + "/%1.svg").arg(anImageBaseName);
		DEBUG5("attempt to load '%s'", ASCII(myFullPathName));
		if (QFile::exists(myFullPathName))
		{
			// render the SVG into the Pixmap
			// rely on operator= to make copy
			myTempPixmap = QPixmap(aSize);
			myTempPixmap.fill(QColor(255,255,255,0));

			QSvgRenderer myRenderer(myFullPathName);
			QPainter myPainter;
			myPainter.begin(&myTempPixmap);
			myPainter.setRenderHint(QPainter::Antialiasing);
			myRenderer.render(&myPainter);
			myPainter.end();
			break;
		}
	}

	DEBUG4("  going to use image: %s!", ASCII(myFullPathName));
	if (myTempPixmap.isNull())
		myTempPixmap = QPixmap(myFullPathName);
//	QPixmapCache::insert(anImageBaseName, myTempPixmap);
	*anOutputPixmapPtr = myTempPixmap;

	return true;
}



QIcon ImageCache::getQIcon(const QString& anImageName, const QSize& aSize)
{
    QPixmap myPixmap(aSize);
    myPixmap.fill(QColor(255,255,255,0));
    ImageCache::getPixmap(anImageName, aSize, &myPixmap);
    return QIcon(myPixmap);
}
