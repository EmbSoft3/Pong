/**
*
* @copyright Mk - Copyright (C) 2017-2019 RENARD Mathieu.
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
* @file mk_pong_listenApplication.c
* @brief Déclaration de la fonction mk_pong_listenApplication.
* @date 5 nov. 2023
*
*/

#include "mk.h"
#include "mk_pong_constants.h"
#include "mk_pong_types.h"
#include "mk_pong_data.h"
#include "mk_pong.h"

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_home_main_handleApplicationInputs ( T_mkPongApplication* p_pong, T_mkApplication* p_application, T_mkCtrlEvent p_evtCtrl )
{
   /* Si une application a été redémarrée */
   if ( p_evtCtrl == K_MK_EVENT_RESUME )
   {
      /* Si l'application redémarrée est l'application 'Pong' */
      if ( p_application == p_pong->application )
      {
         /* Configuration de la visibilité et de l'activité de l'application */
         mk_container_setVisibility ( p_pong->container, K_MK_FIELD_VISIBLE );
         mk_container_setActivity ( p_pong->container, K_MK_FIELD_ACTIF );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }
   }

   /* Sinon si une application a été suspendue */
   else if ( p_evtCtrl == K_MK_EVENT_SUSPEND )
   {
      /* Si l'application mise en pause est l'application 'Pong' */
      if ( p_application == p_pong->application )
      {
         /* Configuration de la visibilité de l'application */
         mk_container_setVisibility ( p_pong->container, K_MK_FIELD_INVISIBLE );
         mk_container_setActivity ( p_pong->container, K_MK_FIELD_INACTIF );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

T_mkCode mk_pong_listenApplication ( T_mkContainer* p_container, T_mkField* p_field, T_mkCtrlId p_appCtrlId, T_mkAddr p_appCtrl, T_mkCtrlEvent p_evtCtrl, uint32_t p_ctrlId )
{
   /* Récupération de l'adresse de l'application propriétaire du container */
   T_mkApplication* l_application = mk_container_getOwner ( p_container );

   /* Récupération de l'adresse de l'application 'Pong' */
   T_mkPongApplication* l_pong = &g_mkPongApplication;

   /* Si le message est à destination de l'application */
   if ( p_appCtrl == l_application )
   {
      /* Si un évènement système s'est produit */
      if ( p_appCtrlId == K_MK_CONTROL_SYSTEM )
      {
         /* Ne rien faire */
      }

      /* Sinon si un évènement en relation avec les applications s'est produit */
      else if ( p_appCtrlId == K_MK_CONTROL_APPLICATION )
      {
         /* Gestion des entrées relatives aux applications */
         mk_home_main_handleApplicationInputs ( l_pong, p_appCtrl, p_evtCtrl );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Suppression warning */
   ( void ) p_field; ( void ) p_appCtrlId; ( void ) p_ctrlId;

   /* Retour */
   return ( K_MK_OK );
}


