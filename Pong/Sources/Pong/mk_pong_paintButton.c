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
* @file mk_pong_buttonPainter.c
* @brief Déclaration de la fonction mk_pong_paintButton.
* @date 14 août 2023
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

T_mkCode mk_pong_paintButton ( T_mkContainer* p_container, T_mkField* p_field, uint32_t p_frameNumber )
{
   /* Déclaration de la variable de retour */
   T_mkCode l_result = K_MK_OK;

   /* Déclaration des vecteurs de travail */
   T_mkVect2D l_v1, l_v2;

   /* Récupération de l'adresse de l'objet T_mkTextField */
   T_mkTextField* l_pongTextButton = ( T_mkTextField* ) p_field->child;

   /* Récupération de l'adresse de l'application 'Pong' */
   T_mkPongApplication* l_pong = &g_mkPongApplication;

   /* Initialisation des vecteurs */
   mk_vect2d_setCoord ( &l_v1, p_field->position.x, p_field->position.y );
   mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( p_field->position.x + ( float32_t ) p_field->dimension.width - 1.0f ), ( float32_t ) ( p_field->position.y + ( float32_t ) p_field->dimension.height - 1.0f ) );

   /* Dessin du cadre du menu */
   l_result = mk_graphics_drawRoundRect ( l_pongTextButton->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, 10, K_MK_COLOR_ARGB_WHITE );

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Actualisation des vecteurs */
      mk_vect2d_setCoord ( &l_v1, l_v1.x + K_MK_PONG_FIELD_MAIN_MENU_BORDER_WIDTH, l_v1.y + K_MK_PONG_FIELD_MAIN_MENU_BORDER_WIDTH );
      mk_vect2d_setCoord ( &l_v2, l_v2.x - K_MK_PONG_FIELD_MAIN_MENU_BORDER_WIDTH, l_v2.y - K_MK_PONG_FIELD_MAIN_MENU_BORDER_WIDTH );

      /* Dessin de la bordure */
      l_result = mk_graphics_drawRoundRect ( l_pongTextButton->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, 10, K_MK_COLOR_ARGB_BLACK );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Dessin du texte */
      l_result = mk_graphics_drawString ( p_field->layer, l_v1, l_v2, l_pongTextButton->strAddr, &l_pongTextButton->style, K_MK_NULL );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Si le boutton en cours de dessin est le bouton 'Play' */
      if ( l_pongTextButton == &l_pong->background.buttonStart )
      {
         /* Dessin de la commande d'activation */
         l_result = mk_graphics_drawString ( p_field->layer, l_v1, l_v2, ( T_str8 ) "(R)", &l_pong->background.buttonStyle, K_MK_NULL );
      }

      /* Sinon si le boutton en cours de dessin est le bouton 'Play' */
      else if ( l_pongTextButton == &l_pong->background.buttonQuit )
      {
         /* Dessin de la commande d'activation */
         l_result = mk_graphics_drawString ( p_field->layer, l_v1, l_v2, ( T_str8 ) "(Q)", &l_pong->background.buttonStyle, K_MK_NULL );
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
   ( void ) p_frameNumber; ( void ) p_container;

   /* Retour */
   return ( l_result );
}
