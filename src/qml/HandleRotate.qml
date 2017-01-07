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
    id: rotateHandle

    property real hsign: 1; // 1 means right, -1 means left
    property real vsign: 1; // 1 means down, -1 means up

    property real startAngle: 0;
    property var  cpos;

    anchors.horizontalCenter: (hsign>0) ? parent.right : parent.left
    anchors.horizontalCenterOffset: hsign*width/3
    anchors.verticalCenter: (vsign>0) ? parent.bottom : parent.top
    anchors.verticalCenterOffset: vsign*width/3
    source: img("ActionRotate")

    visible: theDecorated.isRotate === true && theDecorator.isBackInToolbox !== true;

    width: ResizeInfo.handleWidth
    height: ResizeInfo.handleHeight
    sourceSize.width: width
    sourceSize.height: height

    MouseArea {
        anchors.fill: parent
        drag{ target: parent; }
        onPositionChanged: {
            var mpos;

            if (theDecorator.theActiveHandle != parent) {
                theDecorator.startNewUndo("HandleRotate", parent);
                mpos = this.mapToItem(gameView, mouseX, mouseY);
                cpos = theDecorated.mapToItem(gameView, theDecorated.width/2, theDecorated.height/2);
                rotateHandle.startAngle = theDecorated.rotation - theDecorator.vector2AngleDegrees(mpos.x -cpos.x, mpos.y - cpos.y);
            }
            else {
                mpos = mapToItem(gameView, mouseX, mouseY);
                var newAngle = theDecorator.vector2AngleDegrees(mpos.x-cpos.x, mpos.y-cpos.y) + rotateHandle.startAngle;
                theDecorated.rotation = Math.floor(newAngle/15. + 0.5)* 15.;
                theDecorated.updateVars();
            }
        }
        onReleased: {
            theDecorated.updateVars();
            theDecorated.restoreBindings();
        }
    }
}
