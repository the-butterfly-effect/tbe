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

ViewResizeRotateMoveUndo {
    id: theDecorator;
    objectName: "theDecorator";
    property real minSize: m2xwh(0.1)

    property real oldWidth;
    property real oldHeight;
    property real oldZ;

    Component.onCompleted: {
        theDecorator.startNewUndo("Move", parent);
    }

    Component.onDestruction: {
        theDecorated.z = oldZ;
    }

    Rectangle {
        anchors.fill: parent;
        border {
            width: 1
            color: isColliding ? "#transparent" : "black"
        }
        color: isColliding ? "#80FF5050" : "#8050FF50"
        visible: true

        MouseArea {
            anchors.fill: parent
            drag{
                target: theDecorated
                smoothed: true
            }
            onPositionChanged: {
                theDecorated.updateVars();
            }
            onPressed: {
                if (theDecorator.theActiveHandle != parent) {
                    theDecorator.startNewUndo("Move", parent);
                }
                theDecorated.updateVars();
            }
            onReleased: {
                theDecorated.updateVars();
                if (isBackInToolbox) {
                    // Delete this object...
                    console.log("Time for a delete!")
                    startNewUndo("ReturnToToolbox", theDecorator);
                    selectedItem.destroy();
                    selectedItem = null;
                    console.log("Deletion done!");
                }
                else {
                    // Move succeeded...
                    theDecorated.restoreBindings();
                }
            }
        }

        HandleResizeHorizontal {
            id: left
            sign: -1
        }

        HandleResizeHorizontal {
            id: right
            sign: 1
        }

        HandleResizeVertical {
            id: up
            sign: -1
        }

        HandleResizeVertical {
            id: down
            sign: 1
        }

        HandleRotate {
            id: topleft
            hsign: -1
            vsign: -1
        }

        HandleRotate {
            id: topright
            hsign:  1
            vsign: -1
        }

        HandleRotate {
            id: bottomright
            hsign: 1
            vsign: 1
        }

        HandleRotate {
            id: bottomleft
            hsign: -1
            vsign:  1
        }

        HandleReturnToToolbox {
            id: returnToToolbox
        }

        HandleChoosePhoneNumber {
            id: setPhoneNumber
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: img("ActionMove")

            visible: theDecorated.isMovable === true && theDecorator.isBackInToolbox !== true;

            width: ResizeInfo.handleWidth
            height: ResizeInfo.handleHeight
            sourceSize.width: width
            sourceSize.height: height
        }

        // TODO: only enable the "delete" handle in Level Creator mode
        // TODO: attach to topleft or bottomright, depending on which is closest to the center of the screen...
//        HandleDelete {
//            id: deletehandle
//            anchors.right: topleft.left;
//            anchors.verticalCenter: topleft.verticalCenter;
//            anchors.rightMargin: ResizeInfo.handleWidth / 5;
//        }

    }
}
