/* The Butterfly Effect
 * This file copyright (C) 2011,2012,2016 Klaas van Gend
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

Image {
    id: chooseLevelDialog

    signal cancelButton_clicked();
    signal goButton_clicked(var number);

    function setActive(number) {
        view.currentRow = number
        view.selection.select(number)
    }

    height: 400
    source: img("GameResources")
    sourceSize.width: width
    sourceSize.height: height
    width: 600
    z: 99999

    Text {
        id: title;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        font.bold: true
        font.pointSize: 36
        fontSizeMode: Text.Fit
        height: 40
        horizontalAlignment: Text.AlignHCenter
        minimumPointSize: 12
        text: qsTr("Choose your next level")
        width: parent.width - 88
        wrapMode: Text.NoWrap
    }

    Text {
        id: explain
        anchors.top: title.bottom
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 20
        fontSizeMode: Text.Fit
        text: qsTr("Please select the level you want to play.<br>Double-click for the post-it notes for help.")
        width: parent.width - 40
        wrapMode: Text.WordWrap
    }

    TableView {
        id: view
        anchors.bottom: cancelButton.top
        anchors.bottomMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: explain.bottom
        anchors.topMargin: 10
        backgroundVisible: true
        model: theLevelList
        selectionMode: SelectionMode.SingleSelection

        TableViewColumn { resizable: false; movable: false; role: "number"; title: qsTr("#"); width: 100}
        TableViewColumn { resizable: false; movable: false; role: "title";   title: qsTr("Level Title"); width: view.width - 130}
        TableViewColumn { resizable: false; movable: false; role: "filename"; title: ""; width: 1}

        onDoubleClicked: chooseLevelDialog.goButton_clicked(row)
    }

    TextButton {
        id: cancelButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        text: qsTr("Cancel")
        onClicked: chooseLevelDialog.cancelButton_clicked();
    }
    TextButton {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 88
        text: qsTr("Go!")
        onClicked: chooseLevelDialog.goButton_clicked(view.currentRow);
    }
}
