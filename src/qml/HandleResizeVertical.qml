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
    property real sign: 1; // 1 means down, -1 means up

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: (sign>0) ? parent.bottom : parent.top
    source: img("ActionResizeVertically")

    //visible: theDecorated.isVResize === true;

    width: ResizeInfo.handleWidth
    height: ResizeInfo.handleHeight
    sourceSize.width: width
    sourceSize.height: height

    MouseArea {
        anchors.fill: parent
        drag{ target: parent; axis: Drag.XAxis}
        onMouseXChanged: {
            if(drag.active){
                var myMouseY = mouseY
                if (theDecorated.height +sign*mouseY < minSize)
                    myMouseY = -sign*(theDecorated.height-minSize)
                theDecorated.height += sign*myMouseY
                theDecorated.x -= myMouseY * 0.5 * Math.sin(rotationAngle/180*Math.PI)
                theDecorated.y -= sign*myMouseY * (0.5 -sign*0.5*Math.cos(rotationAngle/180*Math.PI))
                theDecorator.height = theDecorated.height
            }
        }
    }

}
