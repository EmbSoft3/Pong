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
* @file mk_pong_listener.c
* @brief Déclaration de la fonction mk_pong_listenPlayground.
* @date 13 août 2023
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

static void mk_pong_handleRacketUP ( T_mkPongApplication* p_pong )
{
   /* Si la position de la raquette peut être décalée vers le haut */
   if ( p_pong->playground.j1.racket.y > ( K_MK_PONG_BORDER_HEIGHT + K_MK_PONG_DEAD_ZONE_HEIGHT + p_pong->playground.j1.delta ) )
   {
      /* Actualisation de la position */
      p_pong->playground.j1.racket.y -= p_pong->playground.j1.delta;
   }

   /* Sinon */
   else
   {
      /* Actualisation de la position */
      p_pong->playground.j1.racket.y = K_MK_PONG_BORDER_HEIGHT + K_MK_PONG_DEAD_ZONE_HEIGHT;
   }

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_pong_handleRacketDOWN ( T_mkPongApplication* p_pong )
{
   /* Si la position de la raquette peut être décalée vers le bas */
   if ( p_pong->playground.j1.racket.y < ( ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - K_MK_PONG_DEAD_ZONE_HEIGHT - K_MK_PONG_RACKET_HEIGHT ) - p_pong->playground.j1.delta ) )
   {
      /* Actualisation de la position */
      p_pong->playground.j1.racket.y += p_pong->playground.j1.delta;
   }

   /* Sinon */
   else
   {
      /* Actualisation de la position */
      p_pong->playground.j1.racket.y = ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - K_MK_PONG_RACKET_HEIGHT - K_MK_PONG_DEAD_ZONE_HEIGHT );
   }

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_pong_handleKeyboardGameInputs ( T_mkPongApplication* p_pong, T_mkCtrlEvent p_evtCtrl, uint32_t p_ctrlId )
{
   /* Si une touche a été appuyée ou est maintenue */
   if ( ( p_evtCtrl == K_MK_EVENT_KEY_DOWN ) || ( p_evtCtrl == K_MK_EVENT_KEY_HOLD ) )
   {
      /* Si la fléche du haut est appuyée */
      if ( p_ctrlId == K_MK_KEYBOARD_UPARROW )
      {
         /* Décalage de la raquette vers le haut */
         mk_pong_handleRacketUP ( p_pong );
      }

      /* Sinon si la flêche du bas est appuyée */
      else if ( p_ctrlId == K_MK_KEYBOARD_DOWNARROW )
      {
         /* Décalage de la raquette vers le bas */
         mk_pong_handleRacketDOWN ( p_pong );
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

   /* Si une touche a été seulement appuyée */
   if ( p_evtCtrl == K_MK_EVENT_KEY_DOWN )
   {
      /* Sinon si la touche échap est appuyée */
      if ( p_ctrlId == K_MK_KEYBOARD_ESCAPE )
      {
         /* On met la partie en pause */
         p_pong->playground.startGame = K_MK_PONG_GAME_STOP;
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

static void mk_pong_handleMenuInputs ( T_mkPongApplication* p_pong, T_mkAddr p_appCtrl, T_mkCtrlEvent p_evtCtrl, uint32_t p_ctrlId )
{
   /* Récupération de l'adresse du clavier */
   T_mkKeyboard* l_keyboard = ( T_mkKeyboard* ) p_appCtrl;

   /* Si un caractère a été saisi */
   if ( p_evtCtrl == K_MK_EVENT_CHAR_DOWN )
   {
      /* Si l'utilisateur veut jouer */
      if ( ( l_keyboard->input->key.ascii == 'r' ) || ( l_keyboard->input->key.ascii == 'R' ) )
      {
         /* On réinitialise la partie */
         mk_pong_initGames ( p_pong );
         p_pong->playground.startGame = K_MK_PONG_GAME_START;

      }

      /* Sinon si l'utilisateur veut quitter l'application */
      else if ( ( l_keyboard->input->key.ascii == 'q' ) || ( l_keyboard->input->key.ascii == 'Q' ) )
      {
         /* Suppression de l'application, libération des ressources mémoires et des containers dédiés à l'application */
         /* Arrêt de la tâche principale si nécessaire */
         ( void ) mk_application_close ( p_pong->application, K_MK_NULL );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }
   }

   /* Sinon si une touche a été appuyée */
   else if ( p_evtCtrl == K_MK_EVENT_KEY_DOWN )
   {
      /* Si la touche échap est appuyée */
      if ( p_ctrlId == K_MK_KEYBOARD_ESCAPE )
      {
         /* On reprend la partie en pause */
         p_pong->playground.startGame = K_MK_PONG_GAME_START;
      }

      /* Sinon si les touches CTRL+A (AZERTY) ont été appuyée */
      else if ( p_ctrlId == ( K_MK_KEYBOARD_LEFTCTRL | 0x14 ) )
      {
         /* On bascule de mode */
         if ( p_pong->playground.mode == K_MK_PONG_MODE_PLAYER )
         {
            /* Activation du mode demo */
            p_pong->playground.mode = K_MK_PONG_MODE_DEMO;
         }

         /* Sinon */
         else
         {
            /* Activation du mode player */
            p_pong->playground.mode = K_MK_PONG_MODE_PLAYER;
         }
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

T_mkCode mk_pong_listenPlayground ( T_mkContainer* p_container, T_mkField* p_field, T_mkCtrlId p_appCtrlId, T_mkAddr p_appCtrl, T_mkCtrlEvent p_evtCtrl, uint32_t p_ctrlId )
{
   /* Récupération de l'adresse de l'application 'Pong' */
   T_mkPongApplication* l_pong = &g_mkPongApplication;

   /* Si un évènement en relation avec le clavier s'est produit */
   if ( p_appCtrlId == K_MK_CONTROL_KEYBOARD )
   {
      /* Si la partie n'est pas terminée */
      if ( ( l_pong->playground.startGame == K_MK_PONG_GAME_START ) &&
           ( l_pong->playground.j1.intScore < K_MK_PONG_MAX_SCORE ) && ( l_pong->playground.j2.intScore < K_MK_PONG_MAX_SCORE ) )
      {
         /* Gestion des touches en relation avec une partie en cours */
         mk_pong_handleKeyboardGameInputs ( l_pong, p_evtCtrl, p_ctrlId );
      }

      /* Sinon */
      else
      {
         /* Gestion des touches en relation avec un début ou une fin de partie */
         mk_pong_handleMenuInputs ( l_pong, p_appCtrl, p_evtCtrl, p_ctrlId );
      }
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Suppression warning */
   ( void ) p_field; ( void ) p_container;

   /* Retour */
   return ( K_MK_OK );
}




