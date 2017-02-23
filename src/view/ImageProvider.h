/* The Butterfly Effect
 * This file copyright (C) 2011,2016,2017 Klaas van Gend
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

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QIcon>
#include <QSize>
#include <QQuickImageProvider>

/// This class provides pixmaps to the QML Engine, you don't need to call it directly from QML.
class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();

    /// aPixmapName      the file name of the requested pixmap
    /// @aFinalSizePtr   OUTPUT pointer to a size struct with the final size of the pixmap
    /// @aRequestedSize  (if provided) the requested size of the pixmap
    QPixmap requestPixmap(const QString &aPixmapName,
                          QSize *aFinalSizePr,
                          const QSize &aRequestedSize) override;

    /// Simple static, public member: adds "image://tbe/" in front of image name.
    /// Used to ensure we don't have "image://tbe/" everywhere in the code.
    /// @returns anImageName, prepended with "image://tbe/"
    static QString setPath(QString& anImageName)
    { return "image://tbe/"+anImageName; }

    /** always accessible (singleton) function to get an Icon from an image
     *   @param   anImageName   file name (no search path, no extension)
     *   @param   aSize         QSize of the icon
     *   @returns a QIcon with the image requested. if no image found,
     *            QIcon will contain either the NotFound image or white field
     */
    static QIcon getQIcon(const QString &anImageName, const QSize &aSize);

private:
    /// if no size specified, this is the size used for the
    /// images that need to be created (i.e. the SVGs)
    static QSize theDefaultSize;
};

#define IMG(A) ImageProvider::setPath(A)

#endif // IMAGEPROVIDER_H
