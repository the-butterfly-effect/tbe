/* The Butterfly Effect
 * This file copyright (C) 2011  Klaas van Gend
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

#include "BackgroundSerializer.h"
#include <QDomElement>
#include <QStringList>

#include "tbe_global.h"
#include <cstdio>

// these are all declared in Level.cpp
extern const char* theBackgroundString;
extern const char* theImageString;
extern const char* theGradientString;
extern const char* theNameString;
extern const char* thePositionString;

QString
BackgroundSerializer::createObjectFromDom(const QDomNode& q, Background* aBGPtr)
{
	QString myValue;

	/// simple sanity checks first...
	if (q.nodeName().isEmpty())
		return "";   // no background section - that's allowed
	if (q.nodeName() != theBackgroundString)
		return QString("expected <%1> but got <%2>.").arg(theBackgroundString).arg(q.nodeName());

	if (q.hasChildNodes()==true)
	{
		// to parse:   <image name="texture" />
		// to parse:   <gradientstop pos="0.0"> 1.0;1.0;1.0;1.0 </gradientstop>
		QDomElement i;
		for (i=q.firstChildElement(); !i.isNull(); i=i.nextSiblingElement())
		{
			if (i.nodeName() == theImageString)
			{
				aBGPtr->theImageName = i.attributes().namedItem(theNameString).nodeValue();
			}

			if (i.nodeName() == theGradientString)
			{
				QStringList myColors = i.text().split(";");
				if (myColors.size() != 4)
					return QString("gradientstop does not have expected format.");
				bool isOKPos, isOK0, isOK1, isOK2, isOK3;
				Background::GradientStop myStop(
						i.attributes().namedItem(thePositionString).nodeValue().toFloat(&isOKPos),
						myColors[0].toFloat(&isOK0),
						myColors[1].toFloat(&isOK1),
						myColors[2].toFloat(&isOK2),
						myColors[3].toFloat(&isOK3));
				if (isOKPos==false)
					return "Parsing position value in gradientstop failed\n";
				if (isOK0==false || isOK1==false || isOK2==false || isOK3==false)
					return "Parsing color values in gradientstop failed\n";
				aBGPtr->theBackgroundGradient.push_back(myStop);
			}
		}
	}

	DEBUG4("createObjectFromDom for background successful\n");
	return "";
}


QString BackgroundSerializer::floatToString(float aValue)
{
	// HACK HACK - yes, I'm aware I'm using C-style tricks here
	// to work around the i18n issues that QT provides me with :-(
	char myString[56];
	snprintf(myString,50, "%2.2f", aValue);
	return QString(myString);
}


void
BackgroundSerializer::serialize(QDomElement* aParent, Background* aBackgroundPtr)
{
	QDomElement myBackground = aParent->ownerDocument().createElement(theBackgroundString);

	if (aBackgroundPtr->theImageName.isEmpty() == false)
	{
		QDomElement myImageName = aParent->ownerDocument().createElement(theImageString);
		myImageName.setAttribute(theNameString, aBackgroundPtr->theImageName);
		myBackground.appendChild(myImageName);
	}

	// save the gradients
	if (aBackgroundPtr->theBackgroundGradient.count() > 0)
	{
		foreach(Background::GradientStop myGS, aBackgroundPtr->theBackgroundGradient)
		{
			QString myGradientValue = QString("%1;%2;%3;%4")
									  .arg(floatToString(myGS.theR))
									  .arg(floatToString(myGS.theG))
									  .arg(floatToString(myGS.theB))
									  .arg(floatToString(myGS.theAlpha));

			QDomElement myGradient = aParent->ownerDocument().createElement(theGradientString);
			myGradient.setAttribute(thePositionString, floatToString(myGS.thePosition));
			QDomText myT = aParent->ownerDocument().createTextNode(myGradientValue);
			myGradient.appendChild(myT);
			myBackground.appendChild(myGradient);
		}
	}
	aParent->appendChild(myBackground);
}
