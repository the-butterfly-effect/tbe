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

#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtCore/QSize>
#include <QtCore/QString>

/** This is a class with only static members - no need to instantiate.
  *
  * It contains functions to work with pixmaps and icons
  * and the caching of these images.
  */
class ImageCache
{
public:
    /// convenience version of the other findPixmap(), where
    /// you cannot set the size and will have to do with the
    /// standard bitmap size (only relevant for SVG images anyway)
    static bool getPixmap(const QString& anImageBaseName,
                          QPixmap* anOutputPixmap);

    ///
    /// @param anImageBaseName
    /// @param aSize
    /// @param anOutputPixmap
    static bool getPixmap(const QString& anImageBaseName,
                          const QSize& aSize,
                          QPixmap* anOutputPixmap);


    /** always accessible (singleton) function to get an Icon from an image
     *   @param   anImageName   file name (no search path, no extension)
     *   @param   aSize         QSize of the icon
     *   @returns a QIcon with the image requested. if no image found,
     *            QIcon will contain either the NotFound image or white field
     */
    static QIcon getQIcon(const QString& anImageName, const QSize& aSize);


private:
    /// if no size specified, this is the size used for the
    /// images that need to be created (i.e. the SVGs)
    static QSize theDefaultSize;
};

#endif // IMAGECACHE_H
