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
import QtQuick.Controls.Styles 1.4  // for styles
import QtQuick.Controls 1.4    // for the Button

/// This is a custom Button implementation to satisfy kaa-ching's needs for
/// having his own styled textbuttons. The button is semi-transparant and has
/// clicked and hover animations. You can adjust the height.
/// The button scales width to match the text inside.
/// Use it like a normal Button.
///
/// @note: doesn't support QML Actions (yet)
BorderImage {
    property var text
    signal clicked()

    width: mytext.width+2*(height-mytext.height)
    height: 40
    border { left: height/2; top: height/2; right: height/2; bottom: height/2 }
    horizontalTileMode: BorderImage.Stretch
    verticalTileMode: BorderImage.Stretch   // let's hope we don't need this, will not look pretty
    source: mycontrol.pressed ? "qrc:/TextButtonInverted.png" :
               mycontrol.containsMouse ? "qrc:/TextButtonHighlighted.png" :
                   "qrc:/TextButtonNormal.png"

    Text {
        id: mytext
        anchors.horizontalCenter: parent.horizontalCenter
        // No need for horizontal offset as it looks good with just vertical offset
        //anchors.horizontalCenterOffset: !mycontrol.pressed ? 0 : -1
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: !mycontrol.pressed ? 0 : 1
        text: parent.text
    }

    MouseArea {
        id: mycontrol
        anchors.fill: parent
        onClicked: parent.clicked()
        hoverEnabled: true
    }
}
