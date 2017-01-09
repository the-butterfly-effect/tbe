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
    ViewItem(QQuickItem *aParentPtr = Q_NULLPTR);
    ~ViewItem();

    Q_PROPERTY(qreal xInM MEMBER theXinM NOTIFY sizeChanged)
    Q_PROPERTY(qreal yInM MEMBER theYinM NOTIFY sizeChanged)
    Q_PROPERTY(qreal widthInM MEMBER theWidthInM NOTIFY sizeChanged)
    Q_PROPERTY(qreal heightInM MEMBER theHeightInM NOTIFY sizeChanged)
    Q_PROPERTY(qreal angleInDegrees MEMBER theAngleInDegrees NOTIFY angleChanged)

    /// For animated objects (except Pingus!): adjust the frameNumber to
    /// display the different images.
    Q_PROPERTY(int   frameNumber MEMBER theFrameNumber WRITE setNewImageIndex NOTIFY frameNumberChanged)

    Q_PROPERTY(bool isAnything READ isAnything NOTIFY isAnythingChanged)
    Q_PROPERTY(bool isHResize  READ isHResize  NOTIFY isAnythingChanged)
    Q_PROPERTY(bool isMovable  READ isMovable  NOTIFY isAnythingChanged)
    Q_PROPERTY(bool isPhone    READ isPhone    NOTIFY isAnythingChanged)
    Q_PROPERTY(bool isRotate   READ isRotate   NOTIFY isAnythingChanged)
    Q_PROPERTY(bool isVResize  READ isVResize  NOTIFY isAnythingChanged)

    /// Update drawing of the object based on the contents in the provided
    /// AbstractObject.
    /// Updated are: position (incl angle), width, height.
    void adjustObjectDrawingFromAO();

    QString firstImageName();
    QString imageName()
    { return theImageName; }

    bool isAnything();      /// @returns true if anything (movable/hresize/phone/rotate/vresize) is true
    bool isMovable();       /// @returns true if player can move object around
    bool isHResize();       /// @returns true if player can resize object horizontally
    bool isPhone();         /// @returns true if player can set a phone number on the object (DetonatorBox)
    bool isRotate();        /// @returns true if player can rotate object
    bool isVResize();       /// @returns true if player can resize object vertically

    /// Called by objects to adjust their image index.
    /// (this is used for animations based on state changes)
    void setNewImageIndex(unsigned int anIndex);

    void setParents(QQuickItem* aParentPtr, AbstractObjectPtr anAOPtr);

signals:
    void sizeChanged();
    void angleChanged();
    void frameNumberChanged();

    void isAnythingChanged();

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

    int   theFrameNumber;

    QString theImageName;

    friend class AbstractQUndoCommand;
};

#endif // VIEWITEM_H
