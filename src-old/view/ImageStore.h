/* The Butterfly Effect
 * This file copyright (C) 2009,2011  Klaas van Gend
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

#ifndef IMAGESTORE_H
#define IMAGESTORE_H

#include <QIcon>
#include <QMap>
class QPainter;
class QPixmap;
class QSvgRenderer;

/** ImageRenderer abstracts away the various renderers that QT has
  * for types like SVG and PNG and JPG
  */
class ImageRenderer
{
public:
	// no public constructors or destructors here
	// - this class is only instantiated by ImageRendererStore

	/** Renders the current image in this class using the given painter on the
	 *  specified bounds within the painter. If the bounding rectangle is not
	 *  specified the image is mapped to the whole paint device.
	 *  @param painter
	 *  @param bounds
	 */
	void render (QPainter* painter, const QRectF& bounds );
	void render (QPainter* painter);

	/// @returns the natural size of the image
	QSize defaultSize(void);

private:
	friend class ImageRendererStore;

	/** Constructor - only usable by ImageRendererStore
	 *  @param aFullImagePath
	 */
	ImageRenderer(QString aFullImagePath);

	/// destructor (not virtual - not called ever)
	~ImageRenderer();

	QPixmap* theBitmap;
	QSvgRenderer* theSvg;

	// just to be sure, kill copy constructor and assignment operator
	ImageRenderer(const ImageRenderer&);
	const ImageRenderer& operator=(const ImageRenderer&);
};


/** ImageRendererStore is a singleton class that you can ask for an
 *  ImageRenderer instance based on just a filename (without path or
 *  file name extension).
 *
 *  ImageRendererStore will automatically search through various paths
 *  and try various extensions.
 *
 *  Currently supported are SVG, PNG and JPG images.
 */
class ImageRendererStore
{
public:
	/** always accessible (singleton) function to get a renderer for an image
	 *  where image is any of SVG, PNG or JPG
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @param   isNullOnNotFound  if true, will return NULL when no renderer
	 *			  is found. Otherwise the ImageRenderer for the "NotFound"
	 *			  image will be returned.
	 *   @returns NULL if failed to find image and isNullOnNotFound is true,
	 *			  otherwise always valid ImageRenderer will be returned - or
	 *			  still NULL if even the NotFound.svg image is MIA...
	 */
	static ImageRenderer* getImageRenderer(const QString& anImageName,
										   bool isNullOnNotFound = false);

	/** always accessible (singleton) function to get a renderer for an image
	 *  where image type is SVG only
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @param   isNullOnNotFound  if true, will return NULL when no renderer
	 *			  is found. Otherwise the ImageRenderer for the "NotFound"
	 *			  image will be returned.
	 *   @returns NULL if failed to find image and isNullOnNotFound is true,
	 *			  otherwise always valid ImageRenderer will be returned - or
	 *			  still NULL if even the NotFound.svg image is MIA...
	 */
	static QSvgRenderer* getSvgRenderer(const QString& anImageName,
										   bool isNullOnNotFound = false);

	/** always accessible (singleton) function to get a renderer for an image
	 *  where image type is PNG/JPG only
	 *
	 *  Try to avoid this member as it limits you!!!
	 *
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @returns NULL if failed to find image or a pointer to a valid QPixmap
	 */
	static QPixmap* getPixmap(const QString& anImageName);

	/** always accessible (singleton) function to get an Icon from an image
	 *   @param   anImageName   file name (no search path, no extension)
	 *   @param   aSize         QSize of the icon
	 *   @returns a QIcon with the image requested. if no image found,
	 *            QIcon will contain either the NotFound image or white field
	 */
	static QIcon getQIcon(const QString& anImageName, const QSize& aSize);

	///////////////////////////////////////////////////////////////////////////
	// this is a singleton, don't even *think* of using anything from below
	///////////////////////////////////////////////////////////////////////////
private:
	static ImageRendererStore& me();

	/** this is a singleton
	 *  - don't ever try to run the constructor yourself.
	 *  - just use the getters as specified above using
	 *    ImageRendererStore::getImageRenderer("imagename");
	 */
	ImageRendererStore();

	/// no need to make the destructor virtual - this class SHOULD NOT be derived from
	/// also note that this destructor cannot be called.
	~ImageRendererStore();

	/// concatenate anImageName and anExtension and check if the file exists
	/// along any of the defined paths
	/// @returns the full path if a matching file is found or an empty string
	///          if not
	/// @param anImageName file name (without extension) to search for
	/// @param anExtension extension to add to the file name
	QString getFilePath(const QString& anImageName, const QString& anExtension) const;

	typedef QMap<QString, ImageRenderer*> RendererMap;
	RendererMap theRendererMap;

	// just to be sure, kill copy constructor and assignment operator
	ImageRendererStore(const ImageRendererStore&);
	const ImageRendererStore& operator=(const ImageRendererStore&);
};

#endif // IMAGESTORE_H
