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
* @file mk_pong_initGame.c
* @brief Déclaration de la fonction mk_pong_initGame.c.
* @date 14 août 2023
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

void mk_pong_initGames ( T_mkPongApplication* p_pong )
{
   /* La partie est désactivée par défaut */
   p_pong->playground.startGame = K_MK_PONG_GAME_STOP;

   /* Initialisation des vecteurs positions de chaque raquette */
   mk_vect2d_setCoord ( &p_pong->playground.j1.racket,
                        ( float32_t ) ( mk_display_getWidth ( ) - K_MK_PONG_RACKET_OFFSET - K_MK_PONG_RACKET_WIDTH ),
                        ( float32_t ) ( ( mk_display_getHeight ( ) - K_MK_PONG_RACKET_HEIGHT ) >> 1 ) );
   mk_vect2d_setCoord ( &p_pong->playground.j2.racket,
                        ( float32_t ) K_MK_PONG_RACKET_OFFSET,
                        ( float32_t ) ( ( mk_display_getHeight ( ) - K_MK_PONG_RACKET_HEIGHT ) >> 1 ) );

   /* Initialisation des scores */
   p_pong->playground.j1.intScore = 0;
   p_pong->playground.j1.strScore [ 0 ] = '\0';
   p_pong->playground.j1.delta = K_MK_PONG_RACKET_STEP;

   p_pong->playground.j2.intScore = 0;
   p_pong->playground.j2.strScore [ 0 ] = '\0';

   /* L'IA déplace sa raquette à chaque frame : son delta doit être plus faible */
   /* que celui du joueur humain qui, lui, se déplace par événement clavier. */
   p_pong->playground.j2.delta = K_MK_PONG_AI_RACKET_DELTA;

   /* Réinitialisation de l'état interne de l'IA pour la nouvelle partie */
   p_pong->playground.j1.aiTargetY  = 0.0f;
   p_pong->playground.j1.aiError    = 0.0f;
   p_pong->playground.j1.aiErrorSet = 0;
   p_pong->playground.j2.aiTargetY  = 0.0f;
   p_pong->playground.j2.aiError    = 0.0f;
   p_pong->playground.j2.aiErrorSet = 0;

   /* Initialisation des buffers des boites de texte */
   mk_textField_setText ( &p_pong->background.scoreJ1, p_pong->playground.j1.strScore );
   mk_textField_setText ( &p_pong->background.scoreJ2, p_pong->playground.j2.strScore );

   /* Conversion du score en ASCII pour l'affichage */
   mk_utils_itoa ( p_pong->playground.j1.intScore, ( T_str8 ) p_pong->playground.j1.strScore, 10, 1 );
   mk_utils_itoa ( p_pong->playground.j2.intScore, ( T_str8 ) p_pong->playground.j2.strScore, 10, 1 );

   /* Initialisation de la position, de la trajectoire et de la vitesse de la balle */
   mk_pong_reset ( p_pong );

   /* Retour */
   return;
}
