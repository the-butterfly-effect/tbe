/* The Butterfly Effect
 * This file copyright (C) 2016,2017  Klaas van Gend
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

import QtQuick 2.0
import TBEView 1.0

/// The ViewLinkAABB is a complicated version of ViewObject.
/// For the visual part of a Link, we need to draw a line between two points.
/// However, that's rather infeasible in QML. Hence this monstrosity.
ViewLink {
    id: viewLink

    property var myColor;
    property var tooltip;
    property list<Item> theImageList;

    x: m2xwh(xInM);
    y: m2y(yInM);
    width: m2xwh(0.05); // m2xwh(widthInM);
    height: m2xwh(1.0); // heightInM);
    rotation: angleInDegrees;

    Rectangle {
        anchors.fill: parent
        color: myColor;
        radius: width/2.;
    }
}
