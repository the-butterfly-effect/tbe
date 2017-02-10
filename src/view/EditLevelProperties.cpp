/* The Butterfly Effect
 * This file copyright (C) 2009,2010,2017  Klaas van Gend
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

#include "EditLevelProperties.h"
#include "ui_EditLevelProperties.h"

#include "Level.h"
#include "World.h"
#include "ViewWorldItem.h"
#include "GoalEditor.h"    // contains DoubleSpinBoxDelegate
#include "resizinggraphicsview.h"
#include <QStandardItem>
#include <QTableView>
#include <QColorDialog>

EditLevelProperties::EditLevelProperties(Level *aLevelPtr, QWidget *parent) :
    QDialog(parent),
    theLevelPtr(aLevelPtr),
    m_ui(new Ui::EditLevelProperties)
{
    m_ui->setupUi(this);

    // *** setup the Level Properties group box
    m_ui->doubleSpinBoxHeight->setValue(aLevelPtr->theWorldPtr->getTheWorldHeight());
    m_ui->doubleSpinBoxWidth->setValue(aLevelPtr->theWorldPtr->getTheWorldWidth());

    // *** setup the Background group box
    //  basic setup for the tableView
    m_ui->tableView->setItemDelegateForColumn ( 0, new DoubleSpinBoxDelegate(this, 0.0, 1.0));
    m_ui->tableView->setItemDelegateForColumn ( 1, new ColorPickerDelegate(this));
    m_ui->tableView->setItemDelegateForColumn ( 2, new DoubleSpinBoxDelegate(this, 0.0, 1.0));
    theModel = new QStandardItemModel(3, 1);
    Background *myBGPtr = &(aLevelPtr->theWorldPtr->theBackground);
    m_ui->lineEditImageName->setText(myBGPtr->theImageName);
    m_ui->doubleSpinBoxHRepeat->setValue(myBGPtr->theImageHRepeat);
    m_ui->doubleSpinBoxVRepeat->setValue(myBGPtr->theImageVRepeat);
    populateTableAndGradient(myBGPtr);

    connect (theModel, SIGNAL(itemChanged(QStandardItem *)),
             this, SLOT(slot_modelItemChanged(QStandardItem *)));
    connect (this, SIGNAL(accepted()), this, SLOT(slot_accepted()));
}

EditLevelProperties::~EditLevelProperties()
{
    delete m_ui;
}

void EditLevelProperties::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EditLevelProperties::populateTableAndGradient(Background *aBGPtr)
{
    theModel->clear();
    //: translators: keep the semicolons - they separate the column descriptions
    theModel->setHorizontalHeaderLabels(QString(tr("VPos;Color;Transparency")).split(";"));
    m_ui->tableView->horizontalHeader()->setStretchLastSection (true);
    m_ui->tableView->setModel(theModel);
    m_ui->tableView->setColumnWidth(1, 40);

    //  populate tableView and Gradient
    QLinearGradient myBackground(0, m_ui->gradientView->height(), 0, 0 );
    foreach (Background::GradientStop myGS, aBGPtr->theBackgroundGradient) {
        QColor myColor(myGS.theR * 255, myGS.theG * 255, myGS.theB * 255, myGS.theAlpha * 255);
        myBackground.setColorAt(myGS.thePosition, myColor);

        QList<QStandardItem *> myRowList;
        QStandardItem *myPosPtr = new QStandardItem(QString::number(myGS.thePosition));
        myRowList.push_back(myPosPtr);
        QStandardItem *myColorPtr = new QStandardItem("");
        myColorPtr->setBackground(myColor);
        myRowList.push_back(myColorPtr);
        QStandardItem *myAlphaPtr = new QStandardItem(QString::number(myGS.theAlpha));
        myRowList.push_back(myAlphaPtr);
        theModel->appendRow(myRowList);
    }
    QGraphicsScene *myScenePtr = new QGraphicsScene(nullptr);
    myScenePtr->setBackgroundBrush(myBackground);
    m_ui->gradientView->setScene(myScenePtr);
}

void EditLevelProperties::slot_accepted()
{
    // pass the values
    writeChanges();

    // and force the redraw...
    ViewWorldItem::me()->setupBackground();
}

void EditLevelProperties::slot_modelItemChanged(QStandardItem * /*anItem*/)
{
    // this code may be a bit heavy for a single cell change
    // but we're not too careful about performance right now
    writeChanges();

    // and force the redraw...
    ViewWorldItem::me()->setupBackground();
    populateTableAndGradient(&(theLevelPtr->theWorldPtr->theBackground));
}

void EditLevelProperties::writeChanges(void)
{
    // TODO/FIXME: this would be the ideal place to create an UndoObject...

    // *** Level Properties
    theLevelPtr->theWorldPtr->theWorldHeight = m_ui->doubleSpinBoxHeight->value();
    theLevelPtr->theWorldPtr->theWorldWidth = m_ui->doubleSpinBoxWidth->value();

    // *** Background Properties
    Background *myBGPtr = &(theLevelPtr->theWorldPtr->theBackground);
    myBGPtr->theImageName = m_ui->lineEditImageName->text();
    myBGPtr->theImageHRepeat = m_ui->doubleSpinBoxHRepeat->value();
    myBGPtr->theImageVRepeat = m_ui->doubleSpinBoxVRepeat->value();

    myBGPtr->theBackgroundGradient.clear();
    for (int i = 0; i < theModel->rowCount(); i++) {
        float myPos = theModel->item(i, 0)->text().toFloat();
        float myAlpha = theModel->item(i, 2)->text().toFloat();
        QColor myColor = theModel->item(i, 1)->data(Qt::BackgroundRole).value<QColor>();
        myBGPtr->theBackgroundGradient.push_back(
            Background::GradientStop(myPos, myColor.redF(),
                                     myColor.greenF(), myColor.blueF(), myAlpha));
    }
}

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
ColorPickerDelegate::ColorPickerDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *ColorPickerDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &/* option */,
                                           const QModelIndex &/* index */) const
{
    QColorDialog *myDialogPtr = new QColorDialog(parent);
    return myDialogPtr;
}

void ColorPickerDelegate::setEditorData(QWidget *editor,
                                        const QModelIndex &index) const
{
    static_cast<QColorDialog *>(editor)->setCurrentColor(
        index.model()->data(index, Qt::BackgroundRole).value<QColor>() );
}

void ColorPickerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const
{
    QColorDialog *myBoxPtr = static_cast<QColorDialog *>(editor);
    QColor myValue = myBoxPtr->currentColor();
    model->setData(index, myValue, Qt::BackgroundRole);
}

void ColorPickerDelegate::updateEditorGeometry(QWidget * /*anEditor*/,
                                               const QStyleOptionViewItem & /*option*/, const QModelIndex &/* index */) const
{
    // we don't care about the geometry, as we open a separate dialog anyway
}
