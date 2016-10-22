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

import QtQuick 2.0

Rectangle {
    id: paletteItem

    property string source;
    color: "transparent"

    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width:  (owidth/oheight > 1.0)? parent.width : parent.height/owidth*oheight;
        height: (owidth/oheight > 1.0)? parent.width/owidth*oheight : parent.height;
        source: parent.source
        sourceSize.width: width
        sourceSize.height: height
    }

    MouseArea {
        anchors.fill: parent

        // ensure that non-drag 'click' events arrive in 'recipe'
        propagateComposedEvents: true
        onClicked: mouse.accepted = false;

        // we keep drag&drop events here
//        onPressed: if (count > 0) Code.startDrag(mouse);
//        onPositionChanged: Code.continueDrag(mouse);
//        onReleased: Code.endDrag(mouse);
    }
}
