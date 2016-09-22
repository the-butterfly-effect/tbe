/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012,2016 Klaas van Gend
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

#ifndef VIEWWORLDITEM_H
#define VIEWWORLDITEM_H

#include "AbstractObjectPtr.h"

#include <QQuickItem>

// for std::unique_ptr
#include <memory>

class ViewItem;
class World;

class QQmlComponent;
class QQmlContext;
class QQmlEngine;

/** class ViewWorldItem
  * This class is the C++ representation of ViewWorld in QML.
  * In QML, it is the parent of all ViewObjects.
  */
class ViewWorldItem : public QQuickItem
{
    Q_OBJECT

public:
    explicit ViewWorldItem (QQuickItem *aParentPtr = Q_NULLPTR);

    virtual ~ViewWorldItem();

    static ViewWorldItem* me();

    /// Factory method for creating ViewItems to match the AbstractObjectPtr.
    /// The created ViewItem will automatically be added as a visible child of the ViewWorldItem.
    /// @note: Returns a *pointer* to a ViewItem. Ownership of this pointer
    ///        remains with this ViewWorldItem - we delete the object!
    ViewItem* createViewItem(const AbstractObjectPtr anAOPtr, float aDefaultDepth, const QString& extraOptions);

    void setQmlEnginePtr(QQmlEngine* anEnginePtr, const QUrl& aSource);

    void setWorldPtr(World* aWorldPtr);

    const World *getWorldPtr() const
    {
        return theWorldPtr;
    }

signals:

public slots:
    void setupBackground(void);

private slots:

private:
    World *theWorldPtr;

    /// impl/pimpl containing the ViewItem factory
    class impl;
    impl* pImpl;

private:
};

#endif // VIEWWORLDITEM_H
