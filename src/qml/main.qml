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

    function img(A) {
        return "image://tbe/"+A;
    }

    property real theScale: 250

    property int theButtonSize: 40
    property int theIconSize: 20

    Rectangle {
        id: gameView

        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        Layout.preferredHeight: width * 0.5
        border.color: "black"

        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#6baaf1";
            }
            GradientStop {
                position: 0.64;
                color: "#ffffff";
            }
            GradientStop {
                position: 0.98;
                color: "#175800";
            }
        }
    }

    ColumnLayout {
        Rectangle {
            color: "white"
            border.color: "darkgrey"
            border.width: 1
            radius: 7

            id: toolbox
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
                tooltip: "Shows the level information again"
                onClicked: {
                    ResizingGraphicsView.slot_showGameResourcesDialog();
                }
            }
            IconButton {
                iconsource: img("ActionMenuEject")
                tooltip: "Choose a different level to play"
                onClicked: {
                    MainWindow.on_action_Open_Level_triggered();
                }
            }
        }
    }
}





