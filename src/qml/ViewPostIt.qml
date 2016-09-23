/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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

// Use a ViewObject for the basics, as we only want to override the hover and
// click behavior for PostIts.
ViewObject {
    Rectangle {
        anchors.fill: parent
        id: highlight
        color: "transparent"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: highlight.color = "#FFFFFFE0"
        onExited:  highlight.color = "transparent"
    }
}
