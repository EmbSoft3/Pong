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
* @file mk_pong_reset.c
* @brief Déclaration de la fonction mk_pong_reset.
* @date 13 août 2023
*
*/

#include "mk.h"
#include "mk_pong_constants.h"
#include "mk_pong_types.h"
#include "mk_pong.h"

/**
 * @internal
 * @brief
 * @endinternal
 */

void mk_pong_reset ( T_mkPongApplication* p_pong )
{
   /* Déclaration d'une variable de travail */
   uint32_t l_randomValue = 0;

   /* Initialisation de la position de la balle */
   mk_vect2d_setCoord ( &p_pong->playground.ball.position,
                        ( float32_t ) ( ( mk_display_getWidth ( ) - K_MK_PONG_BALL_WIDTH ) >> 1 ),
                        ( float32_t ) ( ( mk_display_getHeight ( ) - K_MK_PONG_BALL_WIDTH ) >> 1 ) );

   /* Initialisation de la vitesse (n pixel par frame) et de la trajectoire */
   p_pong->playground.ball.speed = K_MK_PONG_BALL_INITIAL_SPEED;
   p_pong->playground.ball.dx = K_MK_PONG_BALL_INITIAL_DX;

   /* Récupération d'un nombre aléatoire [de 0 à 0xFFFFFFFF ] */
   ( void ) mk_utils_rand ( &l_randomValue );

   /* Détermination du joueur possédant le service */
   if ( l_randomValue < 0x80000000 )
   {
      /* Le service revient à J2 */
      p_pong->playground.ball.dx = -p_pong->playground.ball.dx;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   p_pong->playground.ball.dy = ( float32_t ) ( ( ( float32_t ) l_randomValue / 4 ) * ( float32_t ) K_MK_PONG_BALL_MAX_INITIAL_DY / ( float32_t ) 0x40000000  );

   /* Récupération d'un nombre aléatoire [de 0 à 0xFFFFFFFF ] */
   ( void ) mk_utils_rand ( &l_randomValue );

   /* Détermination de la trajectoire sur l'axe y */
   if ( l_randomValue < 0x80000000 )
   {
      /* Le service revient à J2 */
      p_pong->playground.ball.dy = -p_pong->playground.ball.dy;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Retour */
   return;
}
