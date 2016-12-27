/* The Butterfly Effect
 * This file copyright (C) 2011, 2016 Klaas van Gend
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
#include "ImageProvider.h"
#include "Level.h"
#include <QCoreApplication>
#include <QFile>
#include <QPainter>
#include <QPixmapCache>
#include <QSvgRenderer>

//
QSize ImageProvider::theDefaultSize(200, 200);

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap ImageProvider::requestPixmap(const QString &aPixmapName,
                                     QSize *aFinalSizePr,
                                     const QSize &aRequestedSize)
{
    QSize myRequestedSize(aRequestedSize);
    DEBUG4("ImageProvider: request for '%s' size %dx%d",
           ASCII(aPixmapName), myRequestedSize.width(), myRequestedSize.height());

    QPixmap myTempPixmap;
    if (aPixmapName.isEmpty())
        return requestPixmap("NotFound", aFinalSizePr, myRequestedSize);

    // only names, no paths, no extensions
    Q_ASSERT(aPixmapName.contains(".") == false);
    Q_ASSERT(aPixmapName.contains("/") == false);

    // No, it isn't. Let's try to load the image.
    QString myFullPathName;

    // we have several locations to search for:
    QStringList mySearchPath;
    mySearchPath << IMAGES_DIRECTORY;
    mySearchPath << ":";    // this is Qt-speak for compiled-in resources
    mySearchPath << Level::getPathToLevelFile();

    for (int i = 0; i < mySearchPath.count() + 1; i++) {
        // if i equals that, we know we have exhausted our search paths
        if (i == mySearchPath.count()) {
            if (aPixmapName!="NotFound")
                return requestPixmap("NotFound", aFinalSizePr, myRequestedSize);
            else
                return myTempPixmap;
            break;
        }

        myFullPathName = QString(mySearchPath[i] + "/%1.svg").arg(aPixmapName);
        DEBUG5("ImageProvider: attempt to load '%s'", ASCII(myFullPathName));
        if (QFile::exists(myFullPathName)) {
            // render the SVG into the Pixmap
            // rely on operator= to make copy
            if (myRequestedSize.isEmpty()) {
                myRequestedSize = theDefaultSize;
            }
            myTempPixmap = QPixmap(myRequestedSize);
            myTempPixmap.fill(QColor(255, 255, 255, 0));

            QSvgRenderer myRenderer(myFullPathName);
            QPainter myPainter;
            myPainter.begin(&myTempPixmap);
            myRenderer.render(&myPainter);
            myPainter.end();
            break;
        }

        myFullPathName = QString(mySearchPath[i] + "/%1.png").arg(aPixmapName);
        DEBUG5("ImageProvider: attempt to load '%s'", ASCII(myFullPathName));
        if (QFile::exists(myFullPathName))
            break;

        myFullPathName = QString(mySearchPath[i] + "/%1.jpg").arg(aPixmapName);
        DEBUG5("ImageProvider: attempt to load '%s'", ASCII(myFullPathName));
        if (QFile::exists(myFullPathName))
            break;
    }

    // if the PNG or JPG was found, load it:
    if (myTempPixmap.isNull()) {
        myTempPixmap = QPixmap(myFullPathName); //.scaled(myRequestedSize);
    }

    if (nullptr != aFinalSizePr) {
        *aFinalSizePr = myRequestedSize;
    }
    return myTempPixmap;
}
