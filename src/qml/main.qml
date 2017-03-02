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

Item {
    id: window

    /// Refers to the currently by the user selected item.
    property var selectedItem : undefined;

    function img(A) {
        return "image://tbe/"+A;
    }

    function xwh2m(aDimInPix) {
        return aDimInPix / ResizeInfo.pixPerMeter;
    }

    function m2xwh(aDimInM) {
        return ResizeInfo.pixPerMeter * aDimInM;
    }

    function y2m(aYInPix) {
         return (gameView.height-aYInPix) / ResizeInfo.pixPerMeter;
    }

    function m2y(aYInM) {
        return gameView.height - ResizeInfo.pixPerMeter * aYInM;
    }

   ViewWorld {
        id: gameView

        clip: true
        objectName: "gameView"
        z: 3

        onIsModifyAllowedChanged: {
            if (!isModifyAllowed) {
                if (selectedItem)
                    selectedItem.destroy();
                selectedItem = undefined;
            }
        }

        // Depending on the aspect ratio, we want to fill up available width or height.
        readonly property real aspectRatio: (window.width-toolbox.width) / window.height
        anchors.right: toolcol.left
        anchors.top: parent.top
        height: (aspectRatio < ResizeInfo.aspectRatio)
                ? (window.width-toolbox.width) / ResizeInfo.aspectRatio : window.height
        width: (aspectRatio > ResizeInfo.aspectRatio)
               ? window.height * ResizeInfo.aspectRatio : (window.width-toolbox.width)

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(selectedItem)
                    selectedItem.destroy()
                selectedItem = null;
            }
        }

        GameControls {
            anchors.right: gameView.right;
            anchors.top: gameView.top;
            z: 400
        }
    }

    ColumnLayout {
        id: toolcol
        z: 2

        width: toolbox.width + 10
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

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
                clip: true
                color: "transparent"
                y: 2
                z: 2
            }
        }

        Row {
            spacing: 18
            width: parent.width
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.minimumHeight: 48
            Layout.maximumHeight: 48

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





