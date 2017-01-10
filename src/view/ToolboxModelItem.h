/* The Butterfly Effect
 * This file copyright (C) 2016,2017 Klaas van Gend
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

#ifndef TOOLBOXMODELITEM_H
#define TOOLBOXMODELITEM_H

#include <QObject>

class ToolboxGroup;
class QQuickItem;

/// This class is the base of the myToolboxModel - as used by the QML Toolbox list view.
/// It links the QML Toolbox items to the ToolboxGroup instances.
class ToolboxModelItem : public QObject
{
    Q_OBJECT
public:
    explicit ToolboxModelItem(QObject *parent = nullptr);
    ToolboxModelItem(const QString &aName, int aCount, qreal aWidth, qreal aHeight,
                     const QString& anIconName, const QString &aTooltip,
                     ToolboxGroup *parent);

    Q_PROPERTY(QString name    MEMBER theName NOTIFY nameChanged)
    Q_PROPERTY(int     count   READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(qreal   owidth  MEMBER theWidth NOTIFY owidthChanged)
    Q_PROPERTY(qreal   oheight MEMBER theHeight NOTIFY oheightChanged)
    Q_PROPERTY(QString iconName MEMBER theIconName NOTIFY iconNameChanged)
    Q_PROPERTY(QString tooltip MEMBER theTooltipText NOTIFY tooltipChanged)

    /// @returns pointer to this object instance.
    Q_INVOKABLE QObject* getToolboxModelItemPtr();

    /// "Returns" the AbstractObject to the toolbox.
    void returnAO2Toolbox(AbstractObjectPtr anAOPtr);

    /// "Retrieves" the AbstractObject from the toolbox.
    AbstractObjectPtr getAOfromToolbox(void);

    QString name();
    int count() { return theCount; }
    qreal owidth();
    qreal oheight();
    QString iconName();
    QString tooltip();

    void setCount(int aNewCount);

signals:
    void nameChanged();
    void countChanged();
    void owidthChanged();
    void oheightChanged();
    void iconNameChanged();
    void tooltipChanged();

public slots:

private:
    QString theName;
    int     theCount;
    qreal   theWidth;
    qreal   theHeight;
    QString theIconName;
    QString theTooltipText;

    ToolboxGroup* theTBGPtr;
};

#endif // TOOLBOXMODELITEM_H
