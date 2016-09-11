/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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

#include "Toolbox.h"
#include "ToolboxGroup.h"
#include "ToolboxGroupSerializer.h"
#include "ToolboxItemGroup.h"
#include "ToolboxListWidgetItem.h"

#include <QGraphicsView>
#include <QListWidget>
#include <QQmlContext>
#include <QQuickWidget>

static Toolbox *theCurrentToolboxPtr = nullptr;


Toolbox::Toolbox()
    : theToolboxOldStylePtr(nullptr),
      theToolboxQmlStylePtr(nullptr)
{
    assert(theCurrentToolboxPtr == nullptr);
    theCurrentToolboxPtr = this;
}


Toolbox::~Toolbox ( )
{
    DEBUG1ENTRY;
    assert(theCurrentToolboxPtr == this);
    theCurrentToolboxPtr = nullptr;
}


QString Toolbox::addToolboxGroup(const QDomNode& aNode)
{
    // no sanity checks, leave that to the serializer
    QString myExtraError;
    ToolboxGroup *myTbGPtr = ToolboxGroupSerializer::createObjectFromDom(aNode, &myExtraError);
    if (myTbGPtr)
        theToolboxList.insert(myTbGPtr->theGroupName, myTbGPtr);
    return myExtraError;
}


void Toolbox::clear()
{
    theToolboxList.clear();
}


ToolboxGroup *
Toolbox::findToolBoxGroup(AbstractObjectPtr anAOPtr)
{
    foreach (ToolboxGroup *i, theCurrentToolboxPtr->theToolboxList) {
        if (i->theInternalName == anAOPtr->getInternalName())
            return i;
    }
    return nullptr;
}


void Toolbox::repopulateToolbox(ResizingGraphicsView* aGVPtr)
{
    theToolboxOldStylePtr->clear();
    for (auto i : theToolboxList)
        new ToolboxListWidgetItem(aGVPtr, i, theToolboxOldStylePtr);
}


void Toolbox::serialize(QDomDocument& aDocumentRef,
                        QDomElement& aToolboxDomNodeRef)
{
    for (auto myI : theToolboxList) {
        aToolboxDomNodeRef.appendChild(ToolboxGroupSerializer::serialize(aDocumentRef, myI));
    }
}

void Toolbox::setupOld(QListWidget *aToolboxOldStylePtr)
{
    assert (nullptr == theToolboxOldStylePtr);
    theToolboxOldStylePtr = aToolboxOldStylePtr;
    assert (theToolboxOldStylePtr);
}


void Toolbox::setupQml(QQuickWidget *aToolboxQmlStylePtr)
{
    assert (nullptr == theToolboxQmlStylePtr);
    theToolboxQmlStylePtr = aToolboxQmlStylePtr;
    assert (theToolboxQmlStylePtr);

    qmlRegisterType<ToolboxItemGroup>("TBEView", 1, 0, "ToolboxItemGroup");
    QList<QObject*> myTBGList;
    myTBGList.append(new ToolboxItemGroup("Volley Ball", 3, 0.21, 0.21, "VolleyBall", "A volleyball is light and very bouncy."));
    myTBGList.append(new ToolboxItemGroup("Slightly Bigger Left Ramp", 1, 1.0, 0.4, "LeftRamp", "This is a ramp.\nThe left is lower than the right, so things slide to the left."));
    myTBGList.append(new ToolboxItemGroup("Penguin", 2, 0.28, 0.28, "pinguswalkleft", "A penguin walks left or right and turns around when it collides with something heavy. It can push light objects around. It also likes to slide down slopes but can’t take much abuse."));
    myTBGList.append(new ToolboxItemGroup("Balloon", 5, 0.27, 0.36, "Balloon", "A helium balloon. Lighter than air, it moves up.\nIt will pop when it hits sharp objects or gets squashed."));
    QQmlContext *ctxt = theToolboxQmlStylePtr->rootContext();
    ctxt->setContextProperty("myToolboxModel", QVariant::fromValue(myTBGList));
}
