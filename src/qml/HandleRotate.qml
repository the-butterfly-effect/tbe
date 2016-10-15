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

Image {
    property real hsign: 1; // 1 means right, -1 means left
    property real vsign: 1; // 1 means down, -1 means up

    anchors.horizontalCenter: (hsign>0) ? parent.right : parent.left
    anchors.horizontalCenterOffset: hsign*width/3
    anchors.verticalCenter: (vsign>0) ? parent.bottom : parent.top
    anchors.verticalCenterOffset: vsign*width/3
    source: img("ActionRotate")

    //visible: theDecorated.isRotate === true;

    width: ResizeInfo.handleWidth
    height: ResizeInfo.handleHeight
    sourceSize.width: width
    sourceSize.height: height

    MouseArea {
        anchors.fill: parent
        drag{ target: parent; }
        onPositionChanged: {
            if(drag.active){
                var myMouseX = mouseX
                var myMouseY = mouseY
                // STILL TODO
            }
        }
    }

}
