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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#include "tbe_global.h"
#include "Box2D.h"
#include "ObjectFactory.h"
#include "PostIt.h"
#include "ViewPostIt.h"

static PostItObjectFactory postItHint("PostItHint",
                                      QT_TRANSLATE_NOOP("PostItObjectFactory", "Post-it with hint"),
                                      QT_TRANSLATE_NOOP("PostItObjectFactory",
                                                        "Someone left you a note here.\nYellow post-it notes give you a little hint for solving this level."),

                                      "PostItHint", "PostItHintBackground", QString("color: #000000; background-color: #FFFF00;"));

static PostItObjectFactory postItTutorial("PostItTutorial",
                                          QT_TRANSLATE_NOOP("PostItObjectFactory", "Post-it with tutorial text"),
                                          QT_TRANSLATE_NOOP("PostItObjectFactory",
                                                            "Someone left you a note here.\nGreen post-it notes explain how to play the game."),
                                          "PostItTutorial", "PostItTutorialBackground",
                                          QString("color: #000000; background-color: #8FFF4B;"));

static PostItObjectFactory postItMisc("PostItMisc",
                                      QT_TRANSLATE_NOOP("PostItObjectFactory", "Post-it with miscellaneous content"),
                                      QT_TRANSLATE_NOOP("PostItObjectFactory",
                                                        "Someone left you a note here.\nWhat might be written on it?"),
                                      "PostItMisc", "PostItMiscBackground", QString("color: #000000; background-color: #FFFFFF;"));


PostIt::PostIt(const char *aDisplayName, const char *aTooltip, const QString &anImageName,
               const QString &aBackgroundImageName, const QString &aButtonStyle)
    : theDisplayName(aDisplayName), theImageName(anImageName),
      theBackgroundImageName(aBackgroundImageName), theButtonStyle(aButtonStyle)
{
    // Post-Its are 3x3 inch (i.e. 8x8 cm)
    // but because they are too small, we will triple each side and make them 22x22 cm...
    const double mySideLength = 0.22;

    setTheWidth(mySideLength);
    setTheHeight(mySideLength);

    theTooltip = aTooltip;

    // Note that PostIt doesn't have a physics representation
    // it is only graphics
    theProps.setDefaultPropertiesString(
        Property::ZVALUE_STRING + QString(":1.5/") +
        Property::IMAGE_NAME_STRING + QString(":%1/").arg(theImageName) +
        QString("page1:/page2:/page3:/page4:/page5:/page6:/page7:/page8:/page9:/") +
        "-" + Property::BOUNCINESS_STRING + QString(":/") +
        "-" + Property::FRICTION_STRING + QString(":/") +
        "-" + Property::NOCOLLISION_STRING + QString(":/") +
        "-" + Property::ROTATABLE_STRING + QString(":/") +
        "-" + Property::RESIZABLE_STRING + QString(":/") +
        "-" + Property::MASS_STRING + QString(":/") );

    DEBUG5("PostIt::PostIt done");
}


PostIt::~PostIt( )
{
    ;
}

ViewObjectPtr  PostIt::createViewObject(float aDefaultDepth)
{
    if (theViewObjectPtr != nullptr)
        return theViewObjectPtr;
    QString myImageName;
    if (theProps.property2String(Property::IMAGE_NAME_STRING, &myImageName, true) == false)
        myImageName = getInternalName();

    theViewObjectPtr = ViewObject::factoryMethod<ViewPostIt>(getThisPtr(), myImageName,
                                                             theBackgroundImageName, theButtonStyle);

    setViewObjectZValue(aDefaultDepth); // will set ZValue different if set in property
    return theViewObjectPtr;
}

ViewItem *PostIt::createViewItem(float aDefaultDepth)
{
    return createViewItemInt(aDefaultDepth, "ViewPostIt", "", QString("backgroundImg: \"%1\"").arg(theBackgroundImageName));
}
