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

static Toolbox *theCurrentToolboxPtr = nullptr;


Toolbox::Toolbox()
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


void Toolbox::serialize(QDomDocument& aDocumentRef,
                        QDomElement& aToolboxDomNodeRef)
{
    for (auto myI : theToolboxList) {
        aToolboxDomNodeRef.appendChild(ToolboxGroupSerializer::serialize(aDocumentRef, myI));
    }
}
