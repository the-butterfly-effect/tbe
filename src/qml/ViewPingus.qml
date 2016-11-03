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

/// Use a ViewObject for the basics, as we only want to override the animation.
/// I'm not using the QML Sprite type because it doesn't do what I want.
ViewObject {

    /// Contains the row number.
    property int statenr : 0;
    /// Contains the column number.
    property int framenr : 0;

    rotation: 0
    onFramenrChanged: {
        console.log("framenr = ", framenr);
    }

    Item {
        clip: true
        height: 32
        width: 32
        Image {
            height: 384
            source: img("pingus")
            width: 512
            x: framenr*-32
            y: statenr*-32
        }
    }

}
