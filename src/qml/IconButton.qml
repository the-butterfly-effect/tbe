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

/// This is a custom Button implementation to satisfy kaa-ching's needs for
/// having his own styled iconbuttons. The button has clicked animations.
/// The button scales width to match the text inside.
/// Use it like a normal Button.
///
/// @note: doesn't support QML Actions (yet)
Image {
    property var iconsource
    property string tooltip

    signal clicked()

    width: ResizeInfo.buttonHeight
    height: ResizeInfo.buttonHeight
    source: mycontrol.pressed ? "qrc:/SimulationButtonInverted.png" : "qrc:/SimulationButtonNormal.png"

    Image {
        id: myicon
        anchors.horizontalCenter: parent.horizontalCenter
        // No need for horizontal offset as it looks good with just vertical offset
        //anchors.horizontalCenterOffset: !mycontrol.pressed ? 0 : -1
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: !mycontrol.pressed ? 0 : 1
        source: iconsource
        opacity: parent.enabled ? 1.0 : 0.3;
        width: ResizeInfo.buttonHeight * 0.45
        height: ResizeInfo.buttonHeight * 0.45
        sourceSize.width: width
        sourceSize.height: height
    }

    MouseArea {
        id: mycontrol
        anchors.fill: parent
        onClicked: if (parent.enabled) parent.clicked()
        hoverEnabled: enabled
        onEntered: mytooltip.show(true);
        onExited: mytooltip.show(false);
    }

    Tooltip {
        id: mytooltip
        text: parent.tooltip
    }
}
