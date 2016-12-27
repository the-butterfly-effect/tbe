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

/// Use a ViewObject for the basics, as we only want to override the hover and
/// click behavior for PostIts (and implement the PostIt viewer itself).
ViewObject {

    /// Will contain the name of the background image.
    property var backgroundImg
    /// Array containing the post it pages text.
    property var pages

    /// Private property containing the current page to view.
    property int pageNumber: 0

    function cleanup() {
        postit.state = "";
        pageNumber = 0;
        nextButton.text = qsTr("Next>")
    }

    Component.onCompleted: cleanup()

    Image {
        id: postit
        width: 0
        height: 0
        // make sure that the larger post-it always fits in the screen
        x: (parent.x<gameView.width-240) ? 0 : parent.width-240
        y: (parent.y<gameView.height-240) ? -15 : parent.height-240+20
        z: 100
        visible: false
        source: img(backgroundImg)
        sourceSize.width: 240
        sourceSize.height: 240
        clip: true

        Text {
            anchors.top: postit.top
            anchors.topMargin: 40
            anchors.leftMargin: 20
            anchors.left: postit.left
            font.bold: true;
            horizontalAlignment: Text.AlignHCenter
            text: pages[pageNumber]
            width: parent.width - 50
            wrapMode: Text.Wrap
        }

        Row {
            anchors.bottom: postit.bottom
            anchors.bottomMargin: 20
            anchors.right: postit.right
            anchors.rightMargin: 20
            TextButton {
                id: nextButton
                onClicked: {
                    if (pageNumber >= pages.length-1) {
                        cleanup();
                    }
                    else {
                        if (pageNumber >= pages.length-2)
                            text = qsTr("Finish");
                        pageNumber++;
                    }
                }
            }
            TextButton {
                text: qsTr("Cancel")
                onClicked: cleanup();
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
        id: highlight
        width: parent.width
        height: parent.height
        color: "transparent"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: highlight.color = "#80FFFFFF"
        onExited:  highlight.color = "transparent"
        onClicked: postit.state = "Opened"
    }
}
