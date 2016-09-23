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
    property var backgroundImg

    Image {
        id: postit
        width: 0
        height: 0
        // make sure that the larger post-it always fits in the screen
        x: (parent.x<gameView.width-width) ? 0 : parent.width-width
        y: (parent.y<gameView.height-height) ? -15 : parent.height-height+15
        z: 100
        visible: false
        source: img(backgroundImg)
        sourceSize.width: 240
        sourceSize.height: 240
        clip: true
        Text {
            anchors.top: postit.top
            anchors.topMargin: 20
            anchors.leftMargin: 20
            anchors.left: postit.left
            width: parent.width - 40
            text: "Hello, world!"
        }

        Row {
            anchors.bottom: postit.bottom
            anchors.bottomMargin: 20
            anchors.horizontalCenter: postit.horizontalCenter
            TextButton {
                text: "Next>"
            }
            TextButton {
                text: "Cancel"
                onClicked: postit.state = ""
            }
        }

        states: State {
            name: "Opened"

            PropertyChanges { target: postit; visible: true; width: 240; height:240 }
        }
        transitions: Transition {
            NumberAnimation { duration: 240; properties: "height" }
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height
        id: highlight
        color: "transparent"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: highlight.color = "#FFFFFFE0"
        onExited:  highlight.color = "transparent"
        onClicked: postit.state = "Opened"
    }
}
