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

#ifndef VIEWITEM_H
#define VIEWITEM_H

#include <QQuickItem>
#include <Position.h>

#include "AbstractObjectPtr.h"

/// ViewItem is the base of our QML ViewObject class.
/// @note The ViewItem is owned by the ViewWorldItem, so you should pay
///       attention whether a ViewItem is still alive when needed.
class ViewItem : public QQuickItem
{
    Q_OBJECT

public:
    explicit ViewItem(QQuickItem *aParentPtr = Q_NULLPTR);

    Q_PROPERTY(qreal xInM MEMBER theXinM NOTIFY sizeChanged)
    Q_PROPERTY(qreal yInM MEMBER theYinM NOTIFY sizeChanged)
    Q_PROPERTY(qreal widthInM MEMBER theWidthInM NOTIFY sizeChanged)
    Q_PROPERTY(qreal heightInM MEMBER theHeightInM NOTIFY sizeChanged)
    Q_PROPERTY(qreal angleInDegrees MEMBER theAngleInDegrees NOTIFY angleChanged)

    /// Update drawing of the object based on the contents in the provided
    /// AbstractObject.
    /// Updated are: position (incl angle), width, height.
    /// TODO: image frame number
    void adjustObjectDrawingFromAO();

    void setParents(QQuickItem* aParentPtr, AbstractObjectPtr anAOPtr);

signals:
    void sizeChanged();
    void angleChanged();

public slots:

private:
    /// because of the symbiosis between AbstractObject and ViewItem/ViewObject,
    /// we're not storing the shared_ptr, but a weak pointer
    /// (otherwise, no AbstractObject would ever be cleaned away)
    AbstractObjectWeakPtr theAOPtr;

    qreal theXinM;
    qreal theYinM;
    qreal theWidthInM;
    qreal theHeightInM;
    qreal theAngleInDegrees;

    friend class AbstractQUndoCommand;
};

#endif // VIEWITEM_H
