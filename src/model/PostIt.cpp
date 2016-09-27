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
#include "Translator.h"

static PostItObjectFactory postItHint("PostItHint",
                                      QT_TRANSLATE_NOOP("PostItObjectFactory", "Post-it with hint"),
                                      QT_TRANSLATE_NOOP("PostItObjectFactory",
                                                        "Someone left you a note here.\nYellow post-it notes give you a little hint for solving this level."),

                                      "PostItHint", "PostItHintBackground");

static PostItObjectFactory postItTutorial("PostItTutorial",
                                          QT_TRANSLATE_NOOP("PostItObjectFactory", "Post-it with tutorial text"),
                                          QT_TRANSLATE_NOOP("PostItObjectFactory",
                                                            "Someone left you a note here.\nGreen post-it notes explain how to play the game."),
                                          "PostItTutorial", "PostItTutorialBackground");

static PostItObjectFactory postItMisc("PostItMisc",
                                      QT_TRANSLATE_NOOP("PostItObjectFactory", "Post-it with miscellaneous content"),
                                      QT_TRANSLATE_NOOP("PostItObjectFactory",
                                                        "Someone left you a note here.\nWhat might be written on it?"),
                                      "PostItMisc", "PostItMiscBackground");


PostIt::PostIt(const char *aDisplayName, const char *aTooltip, const QString &anImageName,
               const QString &aBackgroundImageName)
    : theDisplayName(aDisplayName), theImageName(anImageName),
      theBackgroundImageName(aBackgroundImageName)
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

ViewItem *PostIt::createViewItem(float aDefaultDepth)
{
    QString myPageList("pages : [");
    for (int i=1; i<10; i++) {
        QString myPageNr = "page" + QString::number(i);
        QString myPageText =  theProps.getPropertyNoDefault(myPageNr);
        if (myPageText.isEmpty())
            break;
        myPageList += QString("\"%1\",").arg(TheGetText(myPageText));
    }
    myPageList += "];";

    return createViewItemInt(aDefaultDepth, "ViewPostIt", "", QString("backgroundImg: \"%1\"; %2")
                             .arg(theBackgroundImageName).arg(myPageList));
}
