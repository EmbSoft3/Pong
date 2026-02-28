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
* @file mk_pong_painter.c
* @brief Déclaration de la fonction mk_pong_paintPlayground.
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

static void mk_pong_setSpeed ( T_mkPongApplication* p_pong )
{
   /* Si la vitesse max n'a pas été atteinte */
   if ( p_pong->playground.ball.speed < K_MK_PONG_BALL_MAX_SPEED )
   {
      /* Actualisation de la vitesse */
      p_pong->playground.ball.speed += 0.1f;
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

static void mk_pong_setScore ( T_mkPongPlayer* p_player )
{
   /* Si la vitesse max n'a pas été atteinte */
   if ( p_player->intScore < K_MK_PONG_MAX_SCORE )
   {
      /* Actualisation du score */
      p_player->intScore = ( uint8_t ) ( p_player->intScore + 1 );

      /* Conversion du score en ASCII pour l'affichage */
      mk_utils_itoa ( p_player->intScore, ( T_str8 ) p_player->strScore, 10, 1 );
   }

   /* Sinon */
   else
   {
      /* Fin de partie */
   }

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_pong_setWinner ( T_mkPongApplication* p_pong )
{
   /* Si le joueur 1 a gagné */
   if ( p_pong->playground.j1.intScore == K_MK_PONG_MAX_SCORE )
   {
      /* Le joueur 1 est le meilleur */
      mk_textField_setText ( &p_pong->background.scoreJ1, ( T_str8 ) "WIN" );
   }

   /* Sinon si le joueur 2 a gagné */
   else if ( p_pong->playground.j2.intScore == K_MK_PONG_MAX_SCORE )
   {
      /* Le joueur 2 est le meilleur */
      mk_textField_setText ( &p_pong->background.scoreJ2, ( T_str8 ) "WIN" );
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

static void mk_pong_setDelta ( T_mkPongApplication* p_pong, T_mkPongPlayer* p_player )
{
   /* Déclaration des variables de travail */
   float32_t l_hypothenuse = 0.0f, l_multiplier = -1.0f, l_delta = 0.0f;
   float32_t l_teta = 0.0f, l_alpha = 0.0f;

   /* Si l'origine de la balle est en dehors de la raquette (partie basse) */
   if ( p_pong->playground.ball.position.y < p_player->racket.y )
   {
      /* On considére que le delta est nul */
      l_delta = 0.0f;
   }

   /* Sinon si l'origine de la balle est en dehors de la raquette (partie haute) */
   else if ( p_pong->playground.ball.position.y > ( p_player->racket.y + K_MK_PONG_RACKET_HEIGHT ) )
   {
      /* On considére que le delta est max */
      l_delta = K_MK_PONG_RACKET_HEIGHT;
   }

   /* Sinon */
   else
   {
      /* On calcule le delta */
      l_delta = p_pong->playground.ball.position.y - p_player->racket.y;
   }

   /* Détermination de l'angle de la balle suite au rebond sur la raquette */
   l_teta = ( l_delta * 180.0f ) / K_MK_PONG_RACKET_HEIGHT; /* Axe vertical entre 0 et 180° */
   l_alpha = 90.0f - l_teta;                                /* Axe horizontal entre 0 et 90° */

   /* Si l'angle est supérieur à 90° */
   if ( l_teta > 90.0f )
   {
      /* Réalisation d'un changement de signe */
      l_alpha = -l_alpha;

      /* La trajectoire de la balle est vers le bas de l'écran, la variation sur l'axe 'y' doit être positive. */
      l_multiplier = 1;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Si l'angle alpha est trop faible */
   if ( l_alpha < 22.5f )
   {
      /* On limite les angles trop faible pour ne pas avoir une variation 'dy' trop importante */
      l_alpha = 22.5f;
   }

   /* Sinon si l'angle alpha est trop important */
   else if ( l_alpha > 67.5f )
   {
      /* On limite les angles trop important pour ne pas avoir une variation 'dy' trop importante */
      l_alpha = 67.5f;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Conversion de l'angle en radian */
   l_alpha = ( l_alpha * g_mkPi ) / 180;

   /* Détermination de la valeur de l'hypoténuse du triangle rectangle */
   l_hypothenuse = _math_vabs ( p_pong->playground.ball.dx ) / _math_fcos ( l_alpha );

   /* Détermination de la nouvelle variation selon l'axe 'y' */
   p_pong->playground.ball.dy = l_multiplier * _math_vsqrt ( ( l_hypothenuse * l_hypothenuse ) - ( p_pong->playground.ball.dx * p_pong->playground.ball.dx ) );

   /* Si la position de la balle est dans la plage de jeu */
   if ( ( p_pong->playground.ball.position.x > p_pong->playground.j2.racket.x ) &&
        ( p_pong->playground.ball.position.x < p_pong->playground.j1.racket.x ) )
   {
      /* Inversion du coefficient directeur dx */
      p_pong->playground.ball.dx = -p_pong->playground.ball.dx ;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire, la balle ne peut pas être renvoyée de l'autre coté */
   }

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_pong_setBallPosition ( T_mkPongApplication* p_pong, uint32_t p_frameNumber )
{
   /* Détermination de la valeur de la prochaine position */
   mk_vect2d_setCoord ( &p_pong->playground.ball.position,
                        p_pong->playground.ball.position.x + ( p_pong->playground.ball.dx * p_pong->playground.ball.speed ),
                        p_pong->playground.ball.position.y + ( p_pong->playground.ball.dy * p_pong->playground.ball.speed ) );

   /* Si la bordure haute a été touchée */
   if ( p_pong->playground.ball.position.y <= K_MK_PONG_BORDER_HEIGHT )
   {
      /* Actualisation des coordonnées */
      mk_vect2d_setCoord ( &p_pong->playground.ball.position,
                           p_pong->playground.ball.position.x, K_MK_PONG_BORDER_HEIGHT );

      /* Inversion du coefficient directeur dy */
      p_pong->playground.ball.dy = -p_pong->playground.ball.dy ;
   }

   /* Sinon si la bordure basse a été touchée */
   else if ( ( p_pong->playground.ball.position.y + K_MK_PONG_BALL_WIDTH ) >= ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT ) )
   {
      /* Actualisation des coordonnées */
      mk_vect2d_setCoord ( &p_pong->playground.ball.position,
                           p_pong->playground.ball.position.x, ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - K_MK_PONG_BALL_WIDTH ) );

      /* Inversion du coefficient directeur dy */
      p_pong->playground.ball.dy = -p_pong->playground.ball.dy ;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Si la raquette du joueur 2 a été touchée */
   if ( ( p_pong->playground.ball.position.x <= ( p_pong->playground.j2.racket.x + K_MK_PONG_RACKET_WIDTH ) ) &&
        ( ( p_pong->playground.ball.position.y + K_MK_PONG_BALL_WIDTH ) >= p_pong->playground.j2.racket.y ) &&
        ( p_pong->playground.ball.position.y <= ( p_pong->playground.j2.racket.y + K_MK_PONG_RACKET_HEIGHT ) ) )
   {
      /* Détermination de la nouvelle valeur de la variation 'dy' */
      mk_pong_setDelta ( p_pong, &p_pong->playground.j2 );

      /* Mise à jour de la vitesse de la balle */
      mk_pong_setSpeed ( p_pong );
   }

   /* Sinon si la raquette du joueur 1 a été touchée */
   else if ( ( ( p_pong->playground.ball.position.x + K_MK_PONG_BALL_WIDTH ) >= p_pong->playground.j1.racket.x ) &&
        ( ( p_pong->playground.ball.position.y + K_MK_PONG_BALL_WIDTH ) >= p_pong->playground.j1.racket.y ) &&
        ( p_pong->playground.ball.position.y <= ( p_pong->playground.j1.racket.y + K_MK_PONG_RACKET_HEIGHT ) ) )
   {
      /* Détermination de la nouvelle valeur de la variation 'dy' */
      mk_pong_setDelta ( p_pong, &p_pong->playground.j1 );

      /* Mise à jour de la vitesse de la balle */
      mk_pong_setSpeed ( p_pong );
   }

   /* Dans certaine situation, la balle peut passer derrière la raquette. */
   /* Si le joueur 1 a marqué un point */
   if ( p_pong->playground.ball.position.x <= 0 )
   {
      /* Actualisation du score */
      mk_pong_setScore ( &p_pong->playground.j1 );

      /* Réinitialisation des paramètres */
      mk_pong_reset ( p_pong );
   }

   /* Sinon si le joueur 2 a marqué un point */
   else if ( ( p_pong->playground.ball.position.x + K_MK_PONG_RACKET_WIDTH ) >= ( mk_display_getWidth ( ) - 1 ) )
   {
      /* Actualisation du score */
      mk_pong_setScore ( &p_pong->playground.j2 );

      /* Réinitialisation des paramètres */
      mk_pong_reset ( p_pong );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Suppression du warning */
   ( void ) p_frameNumber;

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_pong_setRacketPosition ( T_mkPongApplication* p_pong, T_mkPongPlayer* p_player, uint32_t p_frameNumber )
{
   /* A chaque frame, on actualise la position verticale de la raquette afin de suivre la balle */
   float32_t l_deltaY = ( p_pong->playground.ball.position.y + ( K_MK_PONG_BALL_WIDTH >> 1 ) ) - ( p_player->racket.y + ( K_MK_PONG_RACKET_HEIGHT >> 1 ) );

   /* Si la position de la balle sur l'axe 'Y' est supérieure au centre de la raquette */
   if ( l_deltaY > K_MK_PONG_RACKET_THRESHOLD )
   {
      /* Si la position de la raquette peut être décalée vers le haut */
      if ( p_player->racket.y < ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - K_MK_PONG_RACKET_HEIGHT - K_MK_PONG_DEAD_ZONE_HEIGHT ) - p_player->delta )
      {
         /* Déplacement de la raquette vers le haut */
         p_player->racket.y += p_player->delta;
      }

      /* Sinon */
      else
      {
         /* Déplacement de la raquette en position min  */
         p_player->racket.y = ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - K_MK_PONG_RACKET_HEIGHT - K_MK_PONG_DEAD_ZONE_HEIGHT );
      }
   }

   /* Sinon si la position de la balle sur l'axe 'Y' est inférieure au centre de la raquette */
   else if ( l_deltaY < ( -K_MK_PONG_RACKET_THRESHOLD ) )
   {
      /* Si la position de la raquette peut être décalée vers le haut */
      if ( p_player->racket.y > ( K_MK_PONG_BORDER_HEIGHT + K_MK_PONG_DEAD_ZONE_HEIGHT + p_player->delta ) )
      {
         /* Déplacement de la raquette vers le bas */
         p_player->racket.y -= p_player->delta;
      }

      /* Sinon */
      else
      {
         /* Déplacement de la raquette en position max  */
         p_player->racket.y = ( float32_t ) ( K_MK_PONG_BORDER_HEIGHT + K_MK_PONG_DEAD_ZONE_HEIGHT );
      }
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Suppression du warning */
   ( void ) p_frameNumber;

   /* Retour */
   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

T_mkCode mk_pong_paintPlayground ( T_mkContainer* p_container, T_mkField* p_field, uint32_t p_frameNumber )
{
   /* Déclaration de la variable de retour */
   T_mkCode l_result = K_MK_OK;

   /* Déclaration des variables de travail */
   uint32_t l_color = 0;

   /* Déclaration des vecteurs de travail */
   T_mkVect2D l_v1, l_v2;

   /* Récupération de l'adresse de l'objet T_mkScreen */
   T_mkScreen* l_playground = ( T_mkScreen* ) p_field->child;

   /* Récupération de l'adresse de l'application */
   T_mkPongApplication* l_pong = &g_mkPongApplication;

   /* Si le champ doit être rafraichi */
   if ( p_field->refresh != K_MK_FIELD_REFRESH_DISABLED )
   {
      /* Conversion de la couleur au format ARGB8888 */
      l_color = mk_color_getARGB32 ( &l_playground->color );

      /* Dessin de l'écran */
      l_result = mk_graphics_fill ( l_playground->field.layer, l_color );

      /* Initialisation des vecteurs */
      mk_vect2d_setCoord ( &l_v1, ( float32_t ) ( ( mk_display_getWidth ( ) - K_MK_PONG_NET_WIDTH ) >> 1 ), ( float32_t ) K_MK_PONG_NET_OFFSET );
      mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( ( mk_display_getWidth ( ) >> 1 ) + K_MK_PONG_NET_WIDTH ), ( float32_t ) ( K_MK_PONG_NET_OFFSET + K_MK_PONG_NET_HEIGHT ) );

      /* Tant que le filet n'a pas été dessiné et tant qu'aucune erreur ne s'est produite */
      while ( ( l_result == K_MK_OK ) && ( l_v2.y < mk_display_getHeight ( ) ) )
      {
         /* Dessin des rectangles successifs */
         l_result = mk_graphics_drawRect ( l_playground->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, K_MK_COLOR_ARGB_WHITE );

         /* Actualisation des coordonnées */
         l_v1.y += ( float32_t ) ( 2 * K_MK_PONG_NET_HEIGHT );
         l_v2.y += ( float32_t ) ( 2 * K_MK_PONG_NET_HEIGHT );
      }

      /* Si aucune erreur ne s'est produite */
      if ( l_result == K_MK_OK )
      {
         /* Initialisation des vecteurs */
         mk_vect2d_setCoord ( &l_v1, 0, 0 );
         mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( mk_display_getWidth ( ) - 1 ), ( float32_t ) K_MK_PONG_BORDER_HEIGHT );

         /* Dessin de la bordure haute */
         l_result = mk_graphics_drawRect ( l_playground->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, K_MK_COLOR_ARGB_WHITE );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }

      /* Si aucune erreur ne s'est produite */
      if ( l_result == K_MK_OK )
      {
         /* Initialisation des vecteurs */
         mk_vect2d_setCoord ( &l_v1, 0, ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - 1 ) );
         mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( mk_display_getWidth ( ) - 1 ), ( float32_t ) ( mk_display_getHeight ( ) - 1 ) );

         /* Dessin de la bordure basse */
         l_result = mk_graphics_drawRect ( l_playground->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, K_MK_COLOR_ARGB_WHITE );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }

      /* Si aucune erreur ne s'est produite */
      if ( l_result == K_MK_OK )
      {
         /* Initialisation des vecteurs */
         mk_vect2d_setCoord ( &l_v1, l_pong->playground.j1.racket.x, l_pong->playground.j1.racket.y );
         mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( l_v1.x + K_MK_PONG_RACKET_WIDTH ), ( float32_t ) ( l_v1.y + K_MK_PONG_RACKET_HEIGHT ) );

         /* Dessin de la raquette gauche */
         l_result = mk_graphics_drawRect ( l_playground->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, K_MK_COLOR_ARGB_WHITE );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }

      /* Si aucune erreur ne s'est produite */
      if ( l_result == K_MK_OK )
      {
         /* Initialisation des vecteurs */
         mk_vect2d_setCoord ( &l_v1, l_pong->playground.j2.racket.x, l_pong->playground.j2.racket.y );
         mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( l_v1.x + K_MK_PONG_RACKET_WIDTH ), ( float32_t ) ( l_v1.y + K_MK_PONG_RACKET_HEIGHT ) );

         /* Dessin de la raquette droite */
         l_result = mk_graphics_drawRect ( l_playground->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, K_MK_COLOR_ARGB_WHITE );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }

      /* Si aucune erreur ne s'est produite */
      if ( l_result == K_MK_OK )
      {
         /* Dessin de la balle */
         mk_vect2d_setCoord ( &l_v1, l_pong->playground.ball.position.x, l_pong->playground.ball.position.y );
         mk_vect2d_setCoord ( &l_v2, ( float32_t ) ( l_v1.x + K_MK_PONG_BALL_WIDTH ), ( float32_t ) ( l_v1.y + K_MK_PONG_BALL_WIDTH ) );

         /* Dessin de la raquette droite */
         l_result = mk_graphics_drawRect ( l_playground->field.layer, K_MK_GRAPHICS_SOLID, l_v1, l_v2, K_MK_COLOR_ARGB_WHITE );
      }

      /* Sinon */
      else
      {
         /* Ne rien faire */
      }

      /* Si aucune erreur ne s'est produite */
      if ( l_result == K_MK_OK )
      {
         /* Si la partie n'est pas terminée */
         if ( ( l_pong->playground.startGame == K_MK_PONG_GAME_START ) &&
              ( l_pong->playground.j1.intScore < K_MK_PONG_MAX_SCORE ) && ( l_pong->playground.j2.intScore < K_MK_PONG_MAX_SCORE ) )
         {
            /* Inhibition des menus */
            mk_textField_setVisibility ( &l_pong->background.buttonStart, K_MK_FIELD_INVISIBLE );
            mk_textField_setVisibility ( &l_pong->background.buttonQuit, K_MK_FIELD_INVISIBLE );

            /* Détermination de la prochaine position de la balle */
            mk_pong_setBallPosition ( l_pong, p_frameNumber );

            /* Actualisation de la position de la raquette du joueur 2. */
            mk_pong_setRacketPosition ( l_pong, &l_pong->playground.j2, p_frameNumber );

            /* Si le mode démo est actif */
            if ( l_pong->playground.mode == K_MK_PONG_MODE_DEMO )
            {
               /* Actualisation de la position de la raquette du joueur 1. */
               mk_pong_setRacketPosition ( l_pong, &l_pong->playground.j1, p_frameNumber );
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
            /* Affichage du gagnant */
            mk_pong_setWinner ( l_pong );

            /* Affichage des menus */
            mk_textField_setVisibility ( &l_pong->background.buttonStart, K_MK_FIELD_VISIBLE );
            mk_textField_setVisibility ( &l_pong->background.buttonQuit, K_MK_FIELD_VISIBLE );
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

   /* Suppression warning */
   ( void ) p_container;

   /* Retour */
   return ( l_result );
}


