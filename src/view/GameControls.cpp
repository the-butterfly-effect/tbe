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

#include "GameControls.h"
#include "tbe_global.h"

///---------------------------------------------------------------------------
///------------------------ GameState ----------------------------------------
///---------------------------------------------------------------------------

void GameState::onEntry ( QEvent * event )
{
    DEBUG4("GameControls-GameState %s onEntry!", ASCII(theName));
    QState::onEntry(event);
    isActive = true;
}

void GameState::onExit ( QEvent * event )
{
    QState::onExit(event);
    isActive = false;
}



///---------------------------------------------------------------------------
///------------------------ GameControls -------------------------------------
///---------------------------------------------------------------------------

GameControls::GameControls(QObject *parent) :
    QObject(parent)
{
}


void GameControls::slot_Forward_triggered()
{

}


void GameControls::slot_Pause_triggered()
{

}


void GameControls::slot_Play_triggered()
{

}


void GameControls::slot_RealFast_triggered()
{

}


void GameControls::slot_Slow_triggered()
{

}
