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
import QtQuick.Controls 1.4


/// Each entry in the toolbox consists of a rounded rectangle with contents.
/// The Rectangle has two states: default and 'opened', depending on the state
/// it shows different contents.
Rectangle {
    id: toolboxEntry

    property real iconSize: 50

    width: parent.width - 2
    height: iconSize + 200;
    color: "whitesmoke"
    border.color: "darkgrey"
    border.width: 3
    radius: 7
    clip: true


    // layout:
    // * closed:
    //   - left: icon (small, not rotated)
    //   - right:  number x short name
    // * opened:
    //   - topleft: full-size image (rotated/scaled/etc)
    //   - topright: close icon (on top of image if needed)
    //   - below image: number x short name
    //   - below name: tooltip

    Row {
        id: topRow
        x: 10; y: 10; width: parent.width-x*2; height: iconSize+y;
        spacing: 10

        // TODO: turn this into something drag&droppable
        Image {
            id: itemIcon
            width: iconSize;
            height: iconSize;
            source: img("VolleyBall")
        }

        Text {
            id: topTitle
            y: -5
            width: parent.width - itemIcon.width - 15
            height: parent.height
            text: "3x Volley Ball"
            font.bold: true
            font.pointSize: 14
            fontSizeMode: Text.Fit
            minimumPointSize: 8
            verticalAlignment: Qt.AlignVCenter
            // visible:
            wrapMode: Text.Wrap
        }
    }

    // the close button is only visible when 'opened':
    ToolButton {
        anchors {
            horizontalCenter: topRow.right
            horizontalCenterOffset: -5
            verticalCenter: topRow.top
            verticalCenterOffset: 5
        }
        height: 18
        iconSource: "qrc:/Shrink.png"
        width: 18
        // visible:
    }

    // either the topTitle (closed) or the midTitle (opened) should be visible
    Text {
        id: midTitle
        anchors { top: topRow.bottom }
        text: topTitle.text
        font.bold: true;
        font.pointSize: 14
        wrapMode: Text.Wrap
        width: parent.width-14
        x: 10
        // visible:
    }

    Text {
        id: tooltipText
        text: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam ipsum diam, accumsan vitae diam vitae, iaculis fringilla tellus. Sed eu elit eget ex ornare auctor. Suspendisse id dui id velit laoreet dapibus eu ut nisi."
        anchors { top: midTitle.bottom }
        font.bold: false;
        font.pointSize: 10
        wrapMode: Text.Wrap
        width: parent.width-14
        x: 10
        y: 5
        // visible:
    }

}
