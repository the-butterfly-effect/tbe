/* The Butterfly Effect
 * This file copyright (C) 2014 Klaas van Gend
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


#ifndef ABSTRACTOBJECTPTR_H
#define ABSTRACTOBJECTPTR_H

/// for making object lifetime code simpler, we're going to use C++11 smartpointers
#include <memory>
class AbstractObject;
typedef std::shared_ptr<AbstractObject> AbstractObjectPtr;

typedef std::weak_ptr<AbstractObject> AbstractObjectWeakPtr;

#endif // ABSTRACTOBJECTPTR_H
