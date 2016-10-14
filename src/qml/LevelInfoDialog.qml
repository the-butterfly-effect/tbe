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


/// This will become the QML dialog originally called "GameResources".
Image {
    id: levelInfoDialog

    property string levelName
    property string description
    property string author

    signal resetButton_clicked();
    signal okButton_clicked();

    height: 400
    source: img("GameResources")
    sourceSize.width: width
    sourceSize.height: height
    width: 600
    z: 99999

    Text {
        id: levelNameText;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        font.bold: true
        font.pointSize: 36
        fontSizeMode: Text.Fit
        height: 40
        horizontalAlignment: Text.AlignHCenter
        minimumPointSize: 12
        text: levelName
        width: parent.width - 88
        wrapMode: Text.NoWrap
    }

    Text {
        anchors.top: levelNameText.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 14
        fontSizeMode: Text.Fit
        height: 220
        minimumPointSize: 8
        text: description
        width: parent.width - 60
        wrapMode: Text.WordWrap
    }


    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
        text: qsTr("Level by <b>%1</b>").arg(author)
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 88
        spacing: 5

        TextButton {
            text: qsTr("Reset all")
            onClicked: levelInfoDialog.resetButton_clicked();
        }
        TextButton {
            text: qsTr("Ok")
            onClicked: levelInfoDialog.okButton_clicked();
        }
    }


}
