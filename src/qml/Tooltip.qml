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

/// First shot at implementing a Tooltip, as Qt only starts providing one in 5.7.
/// @param text   Custom provided text
Rectangle {
    id: toolTipBase;
    property string text

    color: "yellow"
    border.color: "black"
    width: 200
    height: toolTipText.height + 10
    visible: false
    radius: 4
    z: 99999    // only makes this one the highest of the siblings of parent, not the top-most object

    anchors.top: parent.bottom
    anchors.left: parent.left

    Text {
        id: toolTipText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        text: parent.text
        width: parent.width - 10
        wrapMode: Text.Wrap
    }

    function onShowTooltip(isToShow) {
        visible = isToShow;
        if (isToShow==true) {
            if (parent.x + width > parent.parent.width) {
                anchors.left = undefined
                anchors.right = parent.right
            }
            if (parent.y + parent.height + height > parent.parent.height) {
                anchors.top = undefined
                anchors.bottom = parent.top
            }
        }
    }

    Component.onCompleted: {
        // only add toolTip if there is one
        if (toolTipBase.text) {
            var extraWatch = Qt.createQmlObject('import QtQuick 2.0; MouseArea {signal showTooltip(bool a); anchors.fill:parent; hoverEnabled: true; onEntered: {showTooltip(true)} onExited: {showTooltip(false)} }',
                                                parent, "mouseItem");
            extraWatch.showTooltip.connect(onShowTooltip);
            extraWatch.showTooltip.connect(onShowTooltip);
        }
    }
}
