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

#ifndef IMAGESTORE_H
#define IMAGESTORE_H

#include <QImage>
#include <QIcon>
#include <QSize>
#include <QMap>
#include <QSvgRenderer>

/** ImageStore is a singleton class that will load an SVG file
 *  and return the requested imageobject/render type
 *  it caches renderers to SVG images it has loaded
 *
 *  ImageStore also cares for itself to try different paths, only specify
 *  the image file name (like "ActionMove") - we'll add the .svg ourselves
 */
class ImageStore
{
	/////////////////////////////////////////////////////////////////////////////
	// only use the interface in public, like ImageStore::getQIcon(...) !!!
	// don't worry about the magic - this is a "Singleton".
	/////////////////////////////////////////////////////////////////////////////
public:

	/** always accessible (singleton) function to get an Icon from an SVG
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @param   aSize         QSize of the icon
	 *   @returns a QIcon with the image requested. if no image found, QIcon.isValid() will return false
	 */
	static QIcon getQIcon(const QString& anImageName, const QSize& aSize)
		{ return me().getMeQIcon(anImageName, aSize); }

	/** always accessible (singleton) function to get a SVG Renderer
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @returns pointer to the SVGRenderer or NULL if not found
	 */
	static QSvgRenderer* getRenderer(const QString& anImageName)
		{ return me().getMeRenderer(anImageName); }

	/** filters through all necessary directories to find the requested PNG
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @returns QPixmap pointer for the PNG image
	 */
	static QPixmap* getPNGPixmap(const QString& anImageName)
		{ return me().getMePNGPixmap(anImageName); }

	/////////////////////////////////////////////////////////////////////////////
	// don't even *think* of using anything from below
	/////////////////////////////////////////////////////////////////////////////
private:
	static ImageStore& me();

	/** this is a singleton
	 *  - don't ever try to run the constructor yourself.
	 *  - just use the getters as specified above using ImageStore::getWhatIWant("imagename");
	 */
	ImageStore();

	/// no need to make the destructor virtual - this class SHOULD NOT be derived from
	~ImageStore();

	/** this member will load (or return a cached) the QSvgRenderer for image "anImageName.svg"
	 *  it will search through all paths to try to find the image
	 *  @param anImageName - the image name (without preceding paths or the extension)
	 *  @returns NULL if not found or a pointer to a valid, initialised QSvgRenderer
	 */
	QSvgRenderer* getMeRenderer(const QString& anImageName);

	QIcon getMeQIcon(const QString& anImageName, const QSize& aSize);

	QPixmap* getMePNGPixmap(const QString& anImageName);

	QString getFilePath(const QString& anImageName, const QString& anExtension) const;

	typedef QMap<QString, QSvgRenderer*> RendererMap;
	RendererMap theRendererMap;

	typedef QMap<QString, QPixmap*> PixmapMap;
	PixmapMap thePixmapMap;
};

#endif // IMAGESTORE_H
