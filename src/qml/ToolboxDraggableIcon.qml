/* The Butterfly Effect
 * This file copyright (C) 2016,2017 Klaas van Gend
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

Rectangle {
    id: draggableIcon

    property string source;
    property var    startmousepos;
    property var    itemProxy;
    color: "transparent"

    property var finalwidth;
    property var finalheight;

    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // Adjust width/height for aspect ratio, but use standard sizes if we show the 'empty' icon.
        // TODO: take pre-set rotation into account.
        width:  count > 0 ? ((owidth/oheight > 1.0)? parent.width : parent.height/oheight*owidth) : iconSize;
        height: count > 0 ? ((owidth/oheight > 1.0)? parent.width/owidth*oheight : parent.height) : iconSize;
        opacity: (gameView.isModifyAllowed)?1:0.4
        source: parent.source
        sourceSize.width: finalwidth
        sourceSize.height: finalheight
    }

    MouseArea {
        anchors.fill: parent

        // ensure that non-drag 'click' events arrive in 'toolboxEntry'
        propagateComposedEvents: true
        onClicked: mouse.accepted = false;

        // Drag&Drop events:
        onPressed: {
            // Do not respond if not allowed to insert.
            if ((!gameView.isModifyAllowed) || count <= 0) {
                mouse.accepted = false;
                return;
            }
            // Create the RAII itemProxy that holds the InsertMoveQUndoCommand.
            listView.interactive = false;
            var tlpos = draggableIcon.mapToItem(gameView, 0,0);
            var clickpos = draggableIcon.mapToItem(gameView, mouse.x, mouse.y);
            startmousepos = {x: clickpos.x - tlpos.x, y: clickpos.y - tlpos.y };
            itemProxy = Qt.createQmlObject('import TBEView 1.0; InsertMoveProxy {}', this, "");
            itemProxy.createUndo(getToolboxModelItemPtr(), this,
                                 xwh2m(tlpos.x), y2m(tlpos.y));
        }
        onPositionChanged: {
            if (null === itemProxy)
                return;
            var cmpos = draggableIcon.mapToItem(gameView, mouse.x, mouse.y);
            itemProxy.updateVars(xwh2m(cmpos.x - startmousepos.x),
                                 y2m(cmpos.y - startmousepos.y));
        }
        onReleased: {
            listView.interactive = true;
            // Our object itemProxy is an RAII, upon destroy it will either
            // commit or delete its associated InsertMoveQUndoCommand.
            itemProxy.destroy();
        }
    }
}
