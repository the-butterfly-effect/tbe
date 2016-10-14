/* The Butterfly Effect
 * This file copyright (C) 2011,2013,2014,2015 Klaas van Gend
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

#include "ResizeAwareQuickWidget.h"
#include "ViewItem.h"
#include "ViewResizeRotateMoveUndo.h"
#include "ViewWorldItem.h"

#include <QApplication>
#include <QScreen>
#include <QQmlContext>
#include <QQuickItem>


// TODO: make configurable using QSettings
static const qreal theHandleSizeMM = 4.; // handle is resolution independent, 4mm wide and tall
static const int   theHandleMinPix = 18; // never make handle smaller than 18 pixels

static const int   theButtonHeightPix = 40;
static const int   theButtonIconPix   = 20;

ResizeAwareQuickWidget::ResizeAwareQuickWidget(QWidget *parent)
    : QQuickWidget(parent),
      theButtonHeight(theButtonHeightPix),
      theButtonIconSize(theButtonIconPix),
      theWorldWidthInMeters(1.0),
      theWorldHeightInMeters(1.0),
      thePixPerMeter(100),
      theGameViewPtr(nullptr)
{
    // Enable objects in the QML world to see our properties.
    rootContext()->setContextProperty(QStringLiteral("ResizeInfo"), this);

    // TODO/FIXME: Does this belong here???
    qmlRegisterType<ViewItem>("TBEView", 1, 0, "ViewItem");
    qmlRegisterType<ViewResizeRotateMoveUndo>("TBEView", 1, 0, "ViewResizeRotateMoveUndo");
    qmlRegisterType<ViewWorldItem>("TBEView", 1, 0, "ViewWorldItem");

    // Pre-calculate the handle sizes, they normally won't change during play...
    QScreen* myQScreenPtr = QApplication::primaryScreen();
    assert (myQScreenPtr != nullptr);
    theHandleHeight = theHandleSizeMM / 25.4 * myQScreenPtr->physicalDotsPerInchX();
    if (theHandleHeight < theHandleMinPix)
        theHandleHeight = theHandleMinPix;
    theHandleWidth = theHandleSizeMM / 25.4 * myQScreenPtr->physicalDotsPerInchY();
    if (theHandleWidth < theHandleMinPix)
        theHandleWidth = theHandleMinPix;
}

void ResizeAwareQuickWidget::resizeEvent(QResizeEvent *event)
{
    thePixPerMeter = theGameViewPtr->width() / theWorldWidthInMeters;
    emit pixPerMeterChanged();

    if (nullptr != event)
        QQuickWidget::resizeEvent(event);
}

bool ResizeAwareQuickWidget::setupQmlSource(const QUrl &url)
{
    setSource(url);

    // Set the pixels in the corners to the MainWindow's background color.
    // FIXME/TODO: too bad that this doesn't really work when that color
    // happens to be a gradient, like in Linux' KDE5...
    QPalette myPalette = QApplication::palette(this);
    QColor myBackgroundColor = myPalette.color(QPalette::Window);
    setClearColor(myBackgroundColor);

    if (status() == QQuickWidget::Error)
        return false;

    theGameViewPtr = qobject_cast<ViewWorldItem*>(rootObject()->findChild<QQuickItem*>("gameView"));
    assert(nullptr != theGameViewPtr);
    if (nullptr == theGameViewPtr) {
        return false;
    }
    connect(theGameViewPtr, SIGNAL(dimensionsChanged(qreal, qreal)), this, SLOT(updateWorldSize(qreal,qreal)));
    return true;
}

QQuickItem *ResizeAwareQuickWidget::showChooseLevel()
{
    return theGameViewPtr->createDialog("ChooseLevelDialog", "");
}

QQuickItem *ResizeAwareQuickWidget::showLevelInfo()
{
    return theGameViewPtr->createDialog("LevelInfoDialog", "");
}

QQuickItem *ResizeAwareQuickWidget::showWinFail(bool isWin)
{
    return theGameViewPtr->createDialog("WinFailDialog", QString("isWin: %1;").arg(isWin?"true":"false"));
}

void ResizeAwareQuickWidget::setContextProperty(const QString &aName, const QVariant &aValue)
{
    theGameViewPtr->setContextProperty(aName, aValue);
}

void ResizeAwareQuickWidget::updateWorldSize(qreal aWidthInMeter, qreal aHeightInMeter)
{
    DEBUG1ENTRY;
    theWorldWidthInMeters = aWidthInMeter;
    theWorldHeightInMeters= aHeightInMeter;
    emit aspectRatioChanged();
    emit resizeEvent(nullptr);
}
