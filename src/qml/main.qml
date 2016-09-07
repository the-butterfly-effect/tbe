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
    id: gameView

    function img(A) {
        return "image://tbe/"+A;
    }

    border.color: "black"

    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#6baaf1";
        }
        GradientStop {
            position: 0.64;
            color: "#ffffff";
        }
        GradientStop {
            position: 0.98;
            color: "#175800";
        }
    }

    Image {
        id: ball
        source: img("VolleyBall")
        width: 100
        height: 100
    }

    // TODO/FIXME: temporary, to see it work
    ToolboxEntry {
        anchors.top: ball.bottom
        width: parent.width - 4
        x: 2
    }
}

