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
      theWorldWidthInMeters(4.5),
      theWorldHeightInMeters(3.1),
      thePixPerMeter(100),
      theGameViewPtr(nullptr)
{
    // Enable objects in the QML world to see our properties.
    rootContext()->setContextProperty(QStringLiteral("ResizeInfo"), this);

    // TODO/FIXME: Does this belong here???
    qmlRegisterType<ViewItem>("TBEView", 1, 0, "ViewItem");

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

    theGameViewPtr = rootObject()->findChild<QQuickItem*>("gameView");
    if (nullptr == theGameViewPtr) {
        return false;
    }
    return true;
}
