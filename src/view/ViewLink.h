/* The Butterfly Effect
 * This file copyright (C) 2017  Klaas van Gend
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

#ifndef VIEWLINK_H
#define VIEWLINK_H

#include "ViewItem.h"

/// ViewLink is the C++ representation part of the QML visualization of the
/// the Link object.
/// In the old Qt4 TBE, this used to be a QGraphicsLineItem, but alas, that's
/// no longer possible. Hence the current workaround using a QML Rectangle
/// and some heavy calculations.
class ViewLink : public ViewItem
{
    Q_OBJECT

public:
    // no new constructors needed?

    /// Empty Destructor
    virtual ~ViewLink ( );

    /// Update drawing based on the new endpoint coordinates of the underlying
    /// Link object.
    void adjustObjectDrawingFromAO() override;
};

#endif // VIEWLINK_H
