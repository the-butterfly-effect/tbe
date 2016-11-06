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

import QtQuick 2.5
import QtQuick.Controls 1.4
import TBEView 1.0

GameQControls {

    width: 188
    height: 118

    property bool buttonForwardEnabled : true;
    property bool buttonPauseEnabled : false;
    property bool buttonPlayEnabled : true;
    property bool buttonResetEnabled : false;

    function setIcon(number) {
        theIcon.item.accessImage
                = theIconHolder.theImageList[number].source;
    }

    Image {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 188
        height: 50
        source: img("SimulationControlsButtonFrame");

        Row {
            x: 5
            y: 4
            spacing: 3
            width: parent.width
            height: 40

            IconButton {
                id: buttonReset
                iconsource: img("ActionUndo")
                enabled: buttonResetEnabled
                tooltip: qsTr("Reset")
                onClicked: button_reset_clicked();
            }

            Image {
                source: img("Empty")
                width: 7
                height: 7
            }

            IconButton {
                id: buttonPause
                iconsource: img("ActionMenuPause")
                enabled: buttonPauseEnabled
                tooltip: qsTr("Pause")
                onClicked: button_pause_clicked();
            }

            IconButton {
                id: buttonPlay
                iconsource: img("ActionMenuPlay")
                enabled: buttonPlayEnabled
                tooltip: qsTr("Play")
                onClicked: button_play_clicked();
            }
            IconButton {
                id: buttonForward
                iconsource: img("ActionFastForward")
                enabled: buttonForwardEnabled
                tooltip: qsTr("Forward")
                onClicked: button_forward_clicked();
            }
        }

    }

    Image {
        id: theIconHolder
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 68
        height: 68
        source: img("SimulationControlsLabelFrame");

        property list<Item> theImageList :
            [ Image { source: img("StatusFail") },
            Image { source: img("StatusFF") },
            Image { source: img("StatusPlay") },
            Image { source: img("StatusPause") },
            Image { source: img("StatusProblem") },
            Image { source: img("Status4F") },
            Image { source: img("StatusSlow") },
            Image { source: img("StatusStop") },
            Image { source: img("StatusWon") } ]

        Loader {
            id: theIcon
            anchors.top: parent.top
            anchors.right: parent.right
            width: 64
            height: 64
            source: "GameControlsSubImage.qml"
        }

//        Image {
//            anchors.top: parent.top
//            anchors.right: parent.right
//            width: 64
//            height: 64
//            source: img("StatusSlow");
//        }
    }
}
