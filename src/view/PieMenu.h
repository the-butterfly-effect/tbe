/* The Butterfly Effect
 * This file copyright (C) 2011,2012 Klaas van Gend
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

#ifndef PIEMENU_H
#define PIEMENU_H


namespace ActionIcon
{

enum ActionType {
    // no action on position 0 yet
    ACTION_ROTATE = 1,
    ACTION_RESIZE = 2,
    ACTION_MOVE = 3,
    ACTION_DELETE = 4,
    ACTION_EDITPROPERTIES = 5,
    //      ACTION_EDITSPECIAL = 6,
    ACTION_SETPHONE = 7,
    ACTION_INSERT = 32,
};

}

#endif // PIEMENU_H
