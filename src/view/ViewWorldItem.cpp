/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2011,2012 Klaas van Gend
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

#include "AbstractObject.h"
#include "Position.h"
#include "ViewItem.h"
#include "ViewWorldItem.h"
#include "World.h"

#include "tbe_global.h"

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>

/// singleton-like pointer: there's only one.
static ViewWorldItem* theVWIPtr = nullptr;



static void dumpErrors(const QString& aSite, const QQmlComponent& aComponent)
{
    printf("Dumping errors for %s\n", aSite.toLatin1().constData());
    if (aComponent.isError()) {
        QList<QQmlError> myList = aComponent.errors();
        for (auto& i : myList) {
            printf("  error: '%s'.\n", i.toString().toLatin1().constData());
        }
        exit(1);
    }
    printf("  no errors found.\n");
}



/// Impl of ViewWorldItem that creates ViewItems,
/// only exists as pImpl of ViewWorldItem.
class ViewWorldItem::impl {
public:
    impl(QQuickItem* aViewWorldItemPtr, QQmlEngine* aQmlEnginePtr, const QUrl& aSource)
        :
        theParentPtr(aViewWorldItemPtr),
        theQmlComponent(aQmlEnginePtr),
        theQmlContext(theQmlEnginePtr->contextForObject(aViewWorldItemPtr)),
        theQmlEnginePtr(aQmlEnginePtr),
        theSource(aSource)
    {
        // nothing to do here?
        dumpErrors("theQmlComponent in impl constructor", theQmlComponent);
    }

    ViewItem *createViewItem(const QString &aVOType, const AbstractObjectPtr anAOPtr,
                             float aDefaultDepth, const QString &extraOptions);

private:
    QQuickItem* theParentPtr;
    QQmlComponent theQmlComponent;
    QQmlContext theQmlContext;
    QQmlEngine* theQmlEnginePtr;
    QUrl theSource;
};


ViewItem *ViewWorldItem::impl::createViewItem(
        const QString &aVOType,
        const AbstractObjectPtr anAOPtr,
        float aDefaultDepth,
        const QString &extraOptions)
{
    Position myPos = anAOPtr->getOrigCenter();
    qreal myW = anAOPtr->getTheWidth();
    qreal myH = anAOPtr->getTheHeight();

    // TODO: angle
    QString myObjectDescription = QString(
            "%1 { xInM:%2; yInM:%3; z:%4; widthInM:%5; heightInM:%6; %7 }")
            .arg(aVOType)
            .arg(myPos.x - 0.5*myW)
            .arg(myPos.y + 0.5*myH)
            .arg(aDefaultDepth)
            .arg(myW)
            .arg(myH)
            .arg(extraOptions);

    // TODO: let's create all plain QML ViewObjects for now
    theQmlComponent.setData( myObjectDescription.toLatin1(), theSource);
    dumpErrors("theQmlComponent after setData", theQmlComponent);
    ViewItem* myItemPtr = qobject_cast<ViewItem*>(theQmlComponent.create(&theQmlContext));
    dumpErrors("theQmlComponent after create()", theQmlComponent);
    assert(nullptr != myItemPtr);
    myItemPtr->setParentItem(theParentPtr);
    myItemPtr->setParent(theParentPtr);
    return myItemPtr;
}


// ---------------------------------------------------------------------------

ViewWorldItem::ViewWorldItem(QQuickItem *aParentPtr)
    : QQuickItem(aParentPtr),
      theWorldPtr(nullptr),
      pImpl(nullptr)
{
    assert(nullptr == theVWIPtr);
    theVWIPtr = this;
}

ViewWorldItem::~ViewWorldItem()
{
    DEBUG3ENTRY;
    assert(this == theVWIPtr);
    theVWIPtr = nullptr;
    if (pImpl)
        delete pImpl;
}

ViewWorldItem *ViewWorldItem::me()
{
    assert(nullptr != theVWIPtr);
    return theVWIPtr;
}


ViewItem *ViewWorldItem::createViewItem(const QString &aVOType, const AbstractObjectPtr anAOPtr, float aDefaultDepth, const QString &extraOptions)
{
    assert(nullptr != pImpl);
    return pImpl->createViewItem(aVOType, anAOPtr, aDefaultDepth, extraOptions);
}


void ViewWorldItem::setQmlEnginePtr(QQmlEngine *anEnginePtr, const QUrl &aSource)
{
    assert(nullptr != anEnginePtr);
    assert(nullptr == pImpl);
    pImpl = new impl(this, anEnginePtr, aSource);
    assert(nullptr != pImpl);
}


void ViewWorldItem::setWorldPtr(World *aWorldPtr)
{
    // TODO/FIXME: for now, we don't accept loading a new World into the existing object
    assert (nullptr == theWorldPtr);
    assert (nullptr != aWorldPtr);
    theWorldPtr = aWorldPtr;
    setupBackground();
}


void ViewWorldItem::setupBackground(void)
{
    DEBUG1ENTRY;
    assert(nullptr != theWorldPtr);
    if (theWorldPtr->theBackground.theBackgroundGradient.count() == 0 &&
            theWorldPtr->theBackground.theImageName.isEmpty()) {
        // the default if nothing else specified: a blue gradient background
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(0, 0.8, 0.8, 1.0, 1.0));
        theWorldPtr->theBackground.theBackgroundGradient.push_back(
            Background::GradientStop(1, 0.5, 0.5, 0.9, 1.0));
    }
    QVariantMap map;
    for (auto i : theWorldPtr->theBackground.theBackgroundGradient) {
        map.insert(QString("%1").arg(i.thePosition), QColor(i.theR*255, i.theG*255, i.theB*255, i.theAlpha*255));
    }
    QMetaObject::invokeMethod(this, "insertGradient",
                              Q_ARG(QVariant, QVariant::fromValue(map)));
}
