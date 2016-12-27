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


/// This is the dialog shown when a player wins or fails a Level.
/// @param isWin: true: win; false: fail
Image {
    id: winFailDialog
    property bool isWin : true;

    signal chooseButton_clicked();
    signal nextButton_clicked();
    signal replayButton_clicked();
    signal skipButton_clicked();

// TODO:
//    connect(ui->chooseButton, SIGNAL(clicked()), aParent, SIGNAL(signal_actionChooseLevel()));
//    connect(ui->nextButton,   SIGNAL(clicked()), aParent, SIGNAL(signal_actionNextLevel()));
//    connect(ui->replayButton, SIGNAL(clicked()), aParent, SIGNAL(signal_actionReplay()));
//    connect(ui->skipButton,   SIGNAL(clicked()), aParent, SIGNAL(signal_actionSkipLevel()));

    height:153
    source: img("WinFail")
    sourceSize.width: width
    sourceSize.height: height
    width: 599
    z: 99999

    Item {
        width: parent.width - 110
        height: parent.height - 20
        x: 30
        y: 10

        Text {
            width: parent.width - 48
            height: 80
            id: myText
            font.bold: true
            font.pointSize: 36
            fontSizeMode: Text.Fit
            minimumPointSize: 12
            text: winFailDialog.isWin ? qsTr("Congratulations!") : qsTr("Fail - retry?")
            wrapMode: Text.NoWrap
            anchors.top: parent.top
            x: 48
        }

        Row {
            anchors.top: myText.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            spacing: 5

            TextButton {
                text: winFailDialog.isWin ? qsTr("Replay") : qsTr("Retry");
                onClicked: winFailDialog.replayButton_clicked();

            }

            TextButton {
                text: qsTr("Skip");
                enabled: !winFailDialog.isWin
                onClicked: winFailDialog.skipButton_clicked();
            }
        }

        Row {
            anchors.top: myText.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 40
            spacing: 5

            TextButton {
                text: qsTr("Choose...")
                onClicked: winFailDialog.chooseButton_clicked();
            }
            TextButton {
                text: qsTr("Next>")
                enabled: winFailDialog.isWin
                onClicked: winFailDialog.nextButton_clicked();
            }
        }

    }
}
