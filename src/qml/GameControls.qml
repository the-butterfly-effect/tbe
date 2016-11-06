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

Item {
    width: 188
    height: 118

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
                iconsource: img("ActionUndo")
                enabled: false
                tooltip: qsTr("Reset")
//                onClicked: {
//                    GameFlow.slot_showLevelInfoDialog();
//                }
            }

            Image {
                source: img("Empty")
                width: 7
                height: 7
            }

            IconButton {
                iconsource: img("ActionMenuPause")
                enabled: false
                tooltip: qsTr("Pause")
//                onClicked: {
//                    GameFlow.slot_showChooseLevelDialog();
//                }
            }

            IconButton {
                iconsource: img("ActionMenuPlay")
                tooltip: qsTr("Play")
//                onClicked: {
//                    GameFlow.slot_showLevelInfoDialog();
//                }
            }
            IconButton {
                iconsource: img("ActionFastForward")
                tooltip: qsTr("Forward")
//                onClicked: {
//                    GameFlow.slot_showChooseLevelDialog();
//                }
            }
        }

    }

    Image {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 68
        height: 68
        source: img("SimulationControlsLabelFrame");

        Image {
            anchors.top: parent.top
            anchors.right: parent.right
            width: 64
            height: 64
            source: img("StatusSlow");
        }
    }
}
