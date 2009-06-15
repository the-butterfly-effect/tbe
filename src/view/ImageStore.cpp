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

#include <QFile>
#include <QPainter>

static ImageStore* theImageStorePtr = NULL;

/*
class SvgIcon : public QIcon
{
	Q_OBJECT
public:

	void setRenderer(QSvgRenderer* aRenderer)
	{ theSvgRenderer = aRenderer; }

protected:


private:
	QSvgRenderer* theSvgRenderer;
};
*/










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



QIcon ImageStore::getMeQIcon(const QString& anImageName, const QSize& aSize)
{
	QSvgRenderer* myRenderer = getRenderer(anImageName);

	// no renderer found? return an empty icon...
	if (myRenderer == NULL)
		return QIcon();

	// cool! we have a renderer. Now let's render!
	QPixmap myPixmap(aSize);
	myPixmap.fill(QColor(255,255,255,0));
	QPainter myPainter;
	myPainter.begin(&myPixmap);
	myPainter.setRenderHint(QPainter::Antialiasing);
	myRenderer->render(&myPainter);
	myPainter.end();
	QIcon myIcon(myPixmap);
	return myIcon;
}


QSvgRenderer* ImageStore::getMeRenderer(const QString& anImageName)
{
	assert(!anImageName.isEmpty());
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
	QStringList mySearchPath = QString(".:images:images/icons:images/objects:images/src").split(":",QString::SkipEmptyParts);
	QStringList::iterator i;
	QSvgRenderer* myPtr = NULL;
	for (i=mySearchPath.begin(); i!=mySearchPath.end(); ++i)
	{
		QString myFullName = (*i) + "/" + anImageName + ".svg";
		DEBUG5("searching for '%s' ... ", myFullName.toAscii().constData());
		if (QFile::exists(myFullName) == false)
		{
			DEBUG5("not found\n");
			continue;
		}
		DEBUG5("found!!!\n");
		myPtr = new QSvgRenderer(myFullName);
		assert(myPtr!=NULL);
		if (myPtr == NULL)
			continue;
		assert(myPtr->isValid());
		theRendererMap[anImageName]=myPtr;
		break;
	}
	DEBUG5("my Renderer is %p\n", myPtr);
	return myPtr;
}
