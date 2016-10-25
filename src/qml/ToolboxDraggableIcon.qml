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

Rectangle {
    id: draggableIcon

    property string source;
    property var    newItem : undefined;
    property var    startmousepos;
    property var    undoObject : undefined;
    color: "transparent"

    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        // TODO: take pre-set rotation into account
        width:  (owidth/oheight > 1.0)? parent.width : parent.height/owidth*oheight;
        height: (owidth/oheight > 1.0)? parent.width/owidth*oheight : parent.height;
        source: parent.source
        sourceSize.width: width
        sourceSize.height: height
    }

    MouseArea {
        anchors.fill: parent

        // ensure that non-drag 'click' events arrive in 'toolboxEntry'
        propagateComposedEvents: true
        onClicked: mouse.accepted = false;

        // Drag&Drop events:
        onPressed: {
            listView.interactive = false;
            var tlpos = draggableIcon.mapToItem(gameView, 0,0);
            startmousepos = {x:  mouse.x, y: mouse.y};
            undoObject = createUndo(this,
                                    xwh2m(tlpos.x+startmousepos.x),
                                    y2m(tlpos.y+startmousepos.y));
            newItem = undoObject.getTheDecorated();
            newItem.setupDecorator();
        }
        onPositionChanged: {
            // in this one, we keep everything in pixels :-)
            if (null == newItem)
                return;
            var cmpos = draggableIcon.mapToItem(gameView, mouse.x, mouse.y);
            newItem.x = cmpos.x - startmousepos.x;
            newItem.y = cmpos.y - startmousepos.y;
            newItem.updateVars();
        }
        onReleased: {
            listView.interactive = true;
            newItem.updateVars();
            newItem.restoreBindings();
            undoObject.doneMoving();
            newItem = null;
            undoObject = null;
        }
    }
}
