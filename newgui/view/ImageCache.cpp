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

#include "tbe_global.h"
#include "ImageCache.h"
#include <QFile>
#include <QPainter>
#include <QPixmapCache>
#include <QSvgRenderer>

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
    DEBUG1ENTRY;

    Q_ASSERT(anImageBaseName.isEmpty() == false);
    if (anImageBaseName.isEmpty() == true)
        return false;

	// TODO/FIXME:
	// prefix size to pixmapname for cache


	// is the image present in the cache?
	if (QPixmapCache::find(anImageBaseName, anOutputPixmapPtr))
	{
		DEBUG4("Image '%s' found in image cache!", ASCII(anImageBaseName));
		return true;
	}

	// No, it isn't. Let's try to load the image.
	QString myFullPathName;
	// I use a do...while(true) loop here to be able to break from a loop
	// that is run only once.
	do
	{
		// TODO/FIXME: also read from the current level directory
		// i.e. not just ../images

		myFullPathName = QString("../images/%1.png").arg(anImageBaseName);
		DEBUG3("attempt to load '%s'", ASCII(myFullPathName));
		if (QFile::exists(myFullPathName))
			break;

		myFullPathName = QString("../images/%1.jpg").arg(anImageBaseName);
		DEBUG3("attempt to load '%s'", ASCII(myFullPathName));
		if (QFile::exists(myFullPathName))
			break;

		myFullPathName = QString("../images/%1.svg").arg(anImageBaseName);
		DEBUG3("attempt to load '%s'", ASCII(myFullPathName));
		if (QFile::exists(myFullPathName))
		{
			// render the SVG into the Pixmap
			// rely on operator= to make copy
			*anOutputPixmapPtr = QPixmap(aSize);
			anOutputPixmapPtr->fill(QColor(255,255,255,0));

			QSvgRenderer myRenderer(myFullPathName);
			QPainter myPainter;
			myPainter.begin(anOutputPixmapPtr);
			myPainter.setRenderHint(QPainter::Antialiasing);
			myRenderer.render(&myPainter);
			myPainter.end();
			break;
		}

		myFullPathName = "../images/NotFound.png";
		DEBUG4("attempt to load '%s'", ASCII(myFullPathName));
	} while(false);

	DEBUG3("  going to use: %s!", ASCII(myFullPathName));
	if (anOutputPixmapPtr->isNull())
		anOutputPixmapPtr = new QPixmap(myFullPathName);
	QPixmapCache::insert(anImageBaseName, *anOutputPixmapPtr);

	return true;
}



QIcon ImageCache::getQIcon(const QString& anImageName, const QSize& aSize)
{
    QPixmap myPixmap(aSize);
    myPixmap.fill(QColor(255,255,255,0));
    ImageCache::getPixmap(anImageName, aSize, &myPixmap);
    return QIcon(myPixmap);
}
