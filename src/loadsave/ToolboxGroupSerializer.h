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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA.
 */

#ifndef TOOLBOXGROUPSERIALIZER_H
#define TOOLBOXGROUPSERIALIZER_H

#include "ToolboxGroup.h"

// forward declarations:
class QDomElement;
class QDomNode;
class QString;


/** This Serializer cannot be created.
 *  it has two static members:
 *    - to create a DomNode from a ToolboxGroup instantiation
 *    - to populate a ToolboxGroup from a DomNode.
 */
class ToolboxGroupSerializer
{
public:
    /** create a QDomElement of the ToolboxGroup pointed to
     *  @param aDomDocument QDomDocument to create the serialized data for
     *  @param theToolboxGroupPtr   ToolboxGroup to serialize
     */
    static QDomElement serialize(QDomDocument &aDomDocument, ToolboxGroup* aToolboxGroupPtr);

    /** fill up ToolboxGroup from the information in the provided Dom
     *  @param q  the QDomNode containing the object definition
     *  @param aTbGPtr   OUTPUT (must already exist) filled in struct
     *  @returns  error message if failed / empty string if successful.
     */
    static ToolboxGroup* createObjectFromDom(const QDomNode& q, QString* anOutputErrorMsg);

private:
    /// constructor only called by ToolboxGroup
    ToolboxGroupSerializer(const ToolboxGroup* anObjectPtr);
    /// Neutralize copy constructor and assignment operator
    ToolboxGroupSerializer(ToolboxGroupSerializer&) = delete;
    /// Neutralize copy constructor and assignment operator
    ToolboxGroupSerializer& operator=(const ToolboxGroupSerializer&) = delete;
};

#endif // TOOLBOXGROUPSERIALIZER_H
