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
import QtQuick.Controls 1.4    // for the ToolButton


/// Each entry in the toolbox consists of a rounded rectangle with contents.
/// The Rectangle has two states: default and 'Opened'; depending on the state
/// it shows different contents.
Rectangle {
    id: toolboxEntry

    property real iconSize: 50

    function closeItem() {
        state = '';
    }

    width: parent.width - 2
    height: iconSize + 20;
    color: "whitesmoke"
    border.color: "darkgrey"
    border.width: 2
    radius: 7
    clip: true

    // Register our closeItem() slot with the listView, so it can be
    // called when another item opens.
    Component.onCompleted: {
        listView.allClose.connect(closeItem)
    }

    // Layout:
    // * '' (default):
    //   - left: icon (small, not rotated)
    //   - right:  number x short name
    // * 'Opened':
    //   - topleft: full-size image (rotated/scaled/etc)
    //   - topright: close icon (on top of image if needed)
    //   - below image: number x short name
    //   - below name: tooltip (always visible, but clipped if in default state)

    Row {
        id: topRow
        x: 10; y: 10; width: parent.width-x*2; height: iconSize+y;
        spacing: 10

        // TODO: turn this into something drag&droppable
        // TODO: adjust the size of the icon to 'real life size'
        Image {
            id: itemIcon
            width: iconSize
            height: iconSize
            source: img(iconName)
        }

        Text {
            id: topTitle
            y: -5
            width: parent.width - itemIcon.width - 15
            height: parent.height
            text: "%1x\n%2".arg(count).arg(name)
            font.bold: true
            font.pointSize: 14
            fontSizeMode: Text.Fit
            minimumPointSize: 8
            verticalAlignment: Qt.AlignVCenter
            visible: toolboxEntry.state != 'Opened'
            wrapMode: Text.Wrap
        }
    }

    // the close button, only visible when 'Opened':
    ToolButton {
        anchors {
            horizontalCenter: topRow.right
            horizontalCenterOffset: -5
            verticalCenter: topRow.top
            verticalCenterOffset: 5
        }
        height: 18
        iconSource: "qrc:/Shrink.png"
        onClicked: toolboxEntry.state = '';
        width: 18
        visible: toolboxEntry.state == 'Opened'
        z: 10
    }

    // either the topTitle (closed) or the midTitle (Opened) should be visible
    Text {
        id: midTitle
        anchors { top: topRow.bottom }
        text: "%1x %2".arg(count).arg(name)
        font.bold: true;
        font.pointSize: 14
        wrapMode: Text.Wrap
        width: parent.width-14
        x: 10
        visible: toolboxEntry.state == 'Opened'
    }

    Text {
        id: tooltipText
        text: tooltip
        anchors { top: midTitle.bottom }
        font.bold: false;
        font.pointSize: 10
        wrapMode: Text.Wrap
        width: parent.width-14
        x: 10
        y: 5
    }

    // This mouse region covers the entire toolboxEntry.
    // When clicked it changes mode to 'Opened'.
    // If we are already in 'Opened' state, then nothing happens.
    MouseArea {
        anchors.fill: parent
        onClicked: { listView.allClose(); toolboxEntry.state = 'Opened'; }
    }

    states: State {
        name: "Opened"

        PropertyChanges {
            target: toolboxEntry;
            color: "white"
        }
        PropertyChanges {
            target: toolboxEntry;
            // Ensure we can see the full tooltip+image
            height: tooltipText.height + midTitle.height + 25 + itemIcon.height
        }
    }

    transitions: Transition {
        // Make the state changes smooth
        ParallelAnimation {
            ColorAnimation  { duration: 300; property: "color"; }
            NumberAnimation { duration: 300; property: "height" }
        }
    }

}
