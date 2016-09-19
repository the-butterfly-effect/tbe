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

import QtQuick 2.0
import TBEView 1.0

// The ViewItem is an object we own from C++, it contains all info we need.
// We use an embedded Image (and in the future probably also a Text) to
// handle the display of what we want.
ViewItem {
    x: ResizeInfo.pixPerMeter * xInM;
    y: gameView.height - ResizeInfo.pixPerMeter * yInM;
    width: ResizeInfo.pixPerMeter * widthInM;
    height: ResizeInfo.pixPerMeter * heightInM;

    property var imageName;
    Image {
        anchors.fill: parent
        source: img(imageName)
        sourceSize.width: parent.width
        sourceSize.height: parent.height
    }
}
