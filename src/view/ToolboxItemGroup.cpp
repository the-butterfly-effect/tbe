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

#include "ToolboxItemGroup.h"

ToolboxItemGroup::ToolboxItemGroup(QObject *parent) : QObject(parent)
{

}


ToolboxItemGroup::ToolboxItemGroup(const QString &aName, int aCount, qreal aWidth,
                                   qreal aHeight, const QString& anIconName,
                                   const QString &aTooltip, QObject *parent)
    : QObject(parent), theName(aName), theCount (aCount), theWidth(aWidth),
      theHeight(aHeight), theIconName(anIconName), theTooltipText(aTooltip)
{
}
