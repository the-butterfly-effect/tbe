/* The Butterfly Effect
 * This file copyright (C) 2012 Klaas van Gend
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tbe_global.h"
#include "GameResources.h"
#include "ToolboxUpDownControls.h"
#include "ui_ToolboxUpDownControls.h"
#include "ImageCache.h"


ToolboxUpDownControls::ToolboxUpDownControls(ResizingGraphicsView *aParent) :
    QLabel(aParent),
    ui(new Ui::ToolboxUpDownControls)
{
    ui->setupUi(this);

    QPixmap myPixmap;
    ImageCache::getPixmap("ToolboxBackground", size(), &myPixmap);
    this->setPixmap(myPixmap);

    ui->toolButton->setAutoFillBackground(false);
    ui->toolButton->setBackgroundRole(QPalette::NoRole);

    theDownActionPtr = new QAction(ImageCache::getQIcon("ActionToolboxDown", QSize(32,32)), "&Down", this);
    theUpActionPtr   = new QAction(ImageCache::getQIcon("ActionToolboxUp",   QSize(32,32)), "&Up", this);
}


ToolboxUpDownControls::~ToolboxUpDownControls()
{
    delete ui;
}


void ToolboxUpDownControls::parentResize(const QSize& aSize)
{
    // make sure we stay in the top-middle of the view
    move( aSize.width()-size().width(), aSize.height()-size().height());
}

void ToolboxUpDownControls::setDownEnabled()
{
    ui->toolButton->setDefaultAction(theDownActionPtr);
}

void ToolboxUpDownControls::setUpEnabled()
{
    ui->toolButton->setDefaultAction(theUpActionPtr);
}

void ToolboxUpDownControls::setup(GameResources* anGRPtr)
{
    connect(theDownActionPtr, SIGNAL(triggered()), anGRPtr, SLOT(disappearAnimated()));
    connect(theUpActionPtr, SIGNAL(triggered()), anGRPtr, SLOT(appearAnimated()));
    setDownEnabled();
}
