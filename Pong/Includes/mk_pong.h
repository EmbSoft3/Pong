/**
*
* @copyright Mk - Copyright (C) 2017-2018 RENARD Mathieu.
*
* This file is part of Mk.
*
* Mk is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Mk is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with Mk.  If not, see <https://www.gnu.org/licenses/>.
*
* @file mk_pong.h
* @brief Déclaration des fonctions publiques dédiées à l'application 'Pong'.
* @date 11 août 2023
*
*/

#ifndef MK_PONG_H
#define MK_PONG_H

void mk_pong_main ( T_mkAddr p_param );
void mk_pong_reset ( T_mkPongApplication* p_pong );
void mk_pong_initGames ( T_mkPongApplication* p_pong );

T_mkCode mk_pong_init ( T_mkPongApplication* p_pong );
T_mkCode mk_pong_listenPlayground ( T_mkContainer* p_container, T_mkField* p_field, T_mkCtrlId p_appCtrlId, T_mkAddr p_appCtrl, T_mkCtrlEvent p_evtCtrl, uint32_t p_ctrlId );
T_mkCode mk_pong_listenApplication ( T_mkContainer* p_container, T_mkField* p_field, T_mkCtrlId p_appCtrlId, T_mkAddr p_appCtrl, T_mkCtrlEvent p_evtCtrl, uint32_t p_ctrlId );
T_mkCode mk_pong_paintPlayground ( T_mkContainer* p_container, T_mkField* p_field, uint32_t p_frameNumber );
T_mkCode mk_pong_paintButton ( T_mkContainer* p_container, T_mkField* p_field, uint32_t p_frameNumber );

/**
 *
 */

#endif
