/* The Butterfly Effect
 * This file copyright (C) 2016  Klaas van Gend
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

import QtQuick 2.5

/// Use a ViewObject for the basics, as we only want to override the animation
/// using the QML Sprite type.
ViewObject {

    /// Contains the row number.
    property int statenr : 0;
    /// Contains the column number.
    property int framenr : 0;

    onFramenrChanged: {
        console.log("framenr = ", framenr);
    }

    // The code is here, but animation doesn't work as expected...
    SpriteSequence {
        anchors.fill: parent
        id: penguin
        Sprite {
            source: img("pingus")
            frameX: framenr*32;
            frameY: statenr*32;
            frameWidth: 32
            frameHeight: 32
        }
    }
}
