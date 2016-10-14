/* The Butterfly Effect
 * This file copyright (C) 2016 Klaas van Gend
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
#ifndef REQUESTDIALOG_H
#define REQUESTDIALOG_H

// forward decls
class QQuickItem;

/// Interface to request various dialogs to be shown.
/// Intended to be only used by the GameFlow class!
class RequestDialog
{

public:
    /// Show the LevelInfo dialog.
    /// @note Although you get a pointer to the Item, ownership remains with
    ///       the QML engine.
    virtual QQuickItem* showChooseLevel() = 0;

    /// Show the LevelInfo dialog.
    /// @note Although you get a pointer to the Item, ownership remains with
    ///       the QML engine.
    virtual QQuickItem* showLevelInfo() = 0;

    /// Show the WinFail dialog.
    /// @note Although you get a pointer to the Item, ownership remains with
    ///       the QML engine.
    virtual QQuickItem* showWinFail(bool isAWin) = 0;

    /// Set a context property.
    /// E.g. used to hand a list of levels to ChooseLevelDialog.
    virtual void setContextProperty(const QString &aName, const QVariant &aValue) = 0;
};

#endif // REQUESTDIALOG_H
