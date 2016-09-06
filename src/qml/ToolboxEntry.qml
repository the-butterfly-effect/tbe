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
import QtQuick.Controls 1.4


/// Each entry in the toolbox consists of a rounded rectangle with contents.
/// The Rectangle has two states: default and 'opened', depending on the state
/// it shows different contents.
Rectangle {
    id: toolboxEntry

    width: parent.width - 2
    height: iconSize + 20;
    color: "whitesmoke"
    border.color: "darkgrey"
    border.width: 3
    radius: 7
    clip: true

    property real iconSize: 50

    // layout:
    // * closed:
    //   - left: icon (small, not rotated)
    //   - right:  number x short name
    // * opened:
    //   - topleft: full-size image (rotated/scaled/etc)
    //   - topright: close icon (on top of image if needed)
    //   - below image: number x short name
    //   - below name: tooltip

}
