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

// TODO: I don't like the size of the icon in the button as it is now.
//       It doesn't actually follow the width/height/etc of the label image.
//       kaa-ching, Sept 15, 2016.

Button {
    property var iconsource

    width: ResizeInfo.buttonHeight
    height: ResizeInfo.buttonHeight
    style: ButtonStyle {
        background: Image {
            source: control.pressed ? "qrc:/SimulationButtonInverted.png" : "qrc:/SimulationButtonNormal.png"
        }
        label: Image {
            source: iconsource // should be defined by user
            width: ResizeInfo.buttonIconSize
            height: ResizeInfo.buttonIconSize
            sourceSize.width: width
            sourceSize.height: height
            x: control.pressed ? 0 : -1
            y: control.pressed ? 0 : -1
        }
    }
}
