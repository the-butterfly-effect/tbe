/* The Butterfly Effect
 * This file copyright (C) 2012, 2016 Klaas van Gend
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


/// This is the dialog shown when a player wants to adjust a phone number.
/// Upon hitting the OK button, the selected string is passed on in the signal.
/// @param theNumbers   numbers to put in the dialog
Image {
    id: choosePhoneNumberDialog
    signal okButton_clicked(string aNewNumber);
    signal cancelButton_clicked();

    property var theNumbers : [ "Oops,", "forgot to set", "numbers..." ]

    height:138
    source: img("ChoosePhoneNumber")
    sourceSize.width: width
    sourceSize.height: height
    width: 350
    z: 99999

    Item {
        width: parent.width - 66
        height: parent.height - 45
        x: 30
        y: 15

        Text {
            width: parent.width - 10
            height: 32
            id: myTitle
            font.bold: true
            font.pointSize: 36
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            minimumPointSize: 14
            text: qsTr("Select phone to dial")
            wrapMode: Text.NoWrap
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            anchors.top: myTitle.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            spacing: 5

            Text {
                font.bold: true
                text: qsTr("Phone Number:")
                verticalAlignment: Text.AlignVCenter
                height: 30
            }
            ComboBox {
                id: combo
                model: theNumbers
                width: 120
            }
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 20
        spacing: 5

        TextButton {
            text: qsTr("Ok")
            onClicked: choosePhoneNumberDialog.okButton_clicked(combo.currentText);
        }
        TextButton {
            text: qsTr("Cancel")
            onClicked: choosePhoneNumberDialog.cancelButton_clicked();
        }
    }
}
