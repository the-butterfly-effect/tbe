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
import QtQuick.Layouts 1.2     // for RowLayout and ColumnLayout

RowLayout {
    id: window

    /// Refers to the currently by the user selected item.
    property var selectedItem : undefined;

    function img(A) {
        return "image://tbe/"+A;
    }

    ViewWorld {
        id: gameView
        objectName: "gameView"

        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        Layout.preferredHeight: width / ResizeInfo.aspectRatio

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(selectedItem)
                    selectedItem.destroy()
            }
        }
    }

    ColumnLayout {
        Rectangle {
            id: toolbox

            color: "white"
            border.color: "darkgrey"
            border.width: 1
            radius: 7

            width: 200;
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight

            Toolbox {
                width: parent.width
                height: parent.height - 4
                color: "transparent"
                y: 2
            }
        }

        Row {
            spacing: 18
            width: parent.width
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.minimumHeight: 48

            IconButton {
                iconsource: img("IconInfo")
                tooltip: qsTr("Shows the level information again")
                onClicked: {
                    GameFlow.slot_showLevelInfoDialog();
                }
            }
            IconButton {
                iconsource: img("ActionMenuEject")
                tooltip: qsTr("Choose a different level to play")
                onClicked: {
                    GameFlow.slot_showChooseLevelDialog();
                }
            }
        }
    }
}





