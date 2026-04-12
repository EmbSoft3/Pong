/**
*
* @copyright Mk - Copyright (C) 2017-2026 RENARD Mathieu.
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

static void mk_pong_handleRacketBounce ( T_mkPongApplication* p_pong, T_mkPongPlayer* p_player, float32_t p_impactY, float32_t p_dxSign )
{
   float32_t l_delta;    /* Position d'impact relative sur la raquette [0 ; H_racket] */
   float32_t l_teta;     /* Angle vertical [0° ; 180°] */
   float32_t l_alpha;    /* Angle horizontal de sortie [0° ; 90°] (valeur absolue) */
   float32_t l_dySign;   /* Signe de la composante verticale après rebond */
   float32_t l_mag;      /* Norme du vecteur direction avant rebond */

   /* Position d'impact normalisée sur la raquette */
   /* p_impactY est le centre de la balle au moment du contact. */
   /* l_delta est la distance entre le haut de la raquette et ce centre. */
   /* Elle est clampée dans [0 ; RACKET_HEIGHT] pour couvrir les cas limites */
   /* (balle frôlant un bord de la raquette). */
   l_delta = p_impactY - p_player->racket.y;

   /* Si l'impact est en dessous de la raquette */
   if ( l_delta < 0.0f )
   {
      l_delta = 0.0f;
   }

   /* Si l'impact est au dessus de la raquette */
   else if ( l_delta > ( float32_t ) K_MK_PONG_RACKET_HEIGHT )
   {
      l_delta = ( float32_t ) K_MK_PONG_RACKET_HEIGHT;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }
   
   /* Conversion en angles */
   /* teta ∈ [0° ; 180°] : position sur la raquette vue comme un arc. */
   /*   - 0°   → haut de la raquette */
   /*   - 90°  → centre */
   /*   - 180° → bas */
   /* alpha = |90° − teta| : angle horizontal de sortie. */
   /*   - Frappe au centre  → alpha = 0°  (trajectoire plate, sera clampée) */
   /*   - Frappe en haut    → alpha = 90° (trajectoire verticale, sera clampée) */
   /*   - Frappe en bas     → alpha = 90° (idem, trajectoire vers le bas) */
   l_teta  = ( l_delta * 180.0f ) / ( float32_t ) K_MK_PONG_RACKET_HEIGHT;
   l_alpha = 90.0f - l_teta;

   /* Le signe de alpha détermine si la balle monte ou descend */
   l_dySign = ( l_alpha < 0.0f ) ? 1.0f : -1.0f;    /* +1 = vers le bas écran */
   l_alpha  = _math_vabs ( l_alpha );               /* On travaille en valeur absolue */


   /* Clampage de l'angle */
   /* On limite alpha entre ANGLE_MIN et ANGLE_MAX pour éviter : */
   /*   - Les trajectoires trop horizontales (alpha < 22.5°) : la balle ferait des aller-retours interminables sans avancer verticalement. */
   /*   - Les trajectoires trop verticales (alpha > 67.5°) : la balle serait impossible à suivre. */
   
   /* Si clampage MIN */
   if ( l_alpha < K_MK_PONG_ANGLE_MIN_DEG )
   {
      l_alpha = K_MK_PONG_ANGLE_MIN_DEG;
   }
   
   /* Sinon si clampage MAX*/
   else if ( l_alpha > K_MK_PONG_ANGLE_MAX_DEG )
   {
      l_alpha = K_MK_PONG_ANGLE_MAX_DEG;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Conversion en radians */
   l_alpha = ( l_alpha * g_mkPi ) / 180.0f;

   /* Conservation de la norme + recalcul du vecteur */
   l_mag = _math_vsqrt ( ( p_pong->playground.ball.dx * p_pong->playground.ball.dx ) +
        ( p_pong->playground.ball.dy * p_pong->playground.ball.dy )
   );

   /* Nouveau vecteur direction (norme = l_mag, angle = l_alpha) */
   p_pong->playground.ball.dx = p_dxSign  * l_mag * _math_fcos ( l_alpha );
   p_pong->playground.ball.dy = l_dySign  * l_mag * _math_fsin ( l_alpha );

   return;
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static void mk_pong_setBallPosition ( T_mkPongApplication* p_pong, uint32_t p_frameNumber )
{
   /* On considère le SEGMENT parcouru par la balle entre t=0 et t=1 (une frame). */
   /* On vérifie si ce segment franchit le plan x de la raquette. */
   /* Si oui, on calcule le paramètre t ∈ [0,1] exact du croisement, */
   /* on en déduit la position Y précise à cet instant, et on gère le rebond */
   /* à la position d'impact réelle plutôt qu'à la position d'arrivée. */
   
   float32_t l_prevX, l_prevY;    /* Position de la balle en début de frame        */
   float32_t l_nextX, l_nextY;    /* Position de la balle en fin de frame          */
   float32_t l_vx,    l_vy;       /* Déplacement réel cette frame (dx × speed)     */
   float32_t l_t;                 /* Paramètre d'interpolation ∈ [0 ; 1]           */
   float32_t l_impactY;           /* Centre Y de la balle au moment du contact     */
   float32_t l_j1BorderX;         /* Bord gauche de la raquette J1 (droite)        */
   float32_t l_j2BorderX;         /* Bord droit  de la raquette J2 (gauche)        */

   /* Vecteur déplacement réel pour cette frame */
   l_vx = p_pong->playground.ball.dx * p_pong->playground.ball.speed;
   l_vy = p_pong->playground.ball.dy * p_pong->playground.ball.speed;

   l_prevX = p_pong->playground.ball.position.x;
   l_prevY = p_pong->playground.ball.position.y;
   l_nextX = l_prevX + l_vx;
   l_nextY = l_prevY + l_vy;

   /* Si colision avec le bord haut*/
   if ( l_nextY <= ( float32_t ) K_MK_PONG_BORDER_HEIGHT )
   {
      l_nextY = ( float32_t ) K_MK_PONG_BORDER_HEIGHT;
      p_pong->playground.ball.dy = -p_pong->playground.ball.dy;
   }

   /* Sinon si colision avec le bord bas */
   else if ( ( l_nextY + K_MK_PONG_BALL_WIDTH ) >= ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT ) )
   {
      l_nextY = ( float32_t ) ( mk_display_getHeight ( ) - K_MK_PONG_BORDER_HEIGHT - K_MK_PONG_BALL_WIDTH );
      p_pong->playground.ball.dy = -p_pong->playground.ball.dy;
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Raquette du joueur 1 (bord droit de l'écran) */

   /* La balle va vers la droite (l_vx > 0). */
   /* Son bord droit (prevX + BALL_WIDTH) était encore AVANT la raquette. */
   /* Son bord droit (nextX + BALL_WIDTH) a franchi ou atteint la raquette. */
   
   /* Paramètre d'interpolation : */
   /*   t = distance_à_franchir / déplacement_total */
   /*     = (l_j1BorderX − (prevX + BALL_WIDTH)) / l_vx */
   
   /* Position Y au moment du contact : */
   /*   impactY = prevY + t × l_vy   (centre de la balle) */
   
   /* Condition de rebond : impactY dans la hauteur de la raquette. */
   /* Si la balle rate la raquette, elle continue et un point est comptabilisé. */
   l_j1BorderX = p_pong->playground.j1.racket.x;

   /* Condition de collision avec la raquette du joueur 1 */
   if ( ( l_vx > 0.0f ) && ( ( l_prevX + K_MK_PONG_BALL_WIDTH ) < l_j1BorderX ) && ( ( l_nextX + K_MK_PONG_BALL_WIDTH ) >= l_j1BorderX ) )
   {
      /* Instant précis du contact (interpolation linéaire) */
      l_t       = ( l_j1BorderX - ( l_prevX + K_MK_PONG_BALL_WIDTH ) ) / l_vx;
      l_impactY = l_prevY + l_t * l_vy + ( ( float32_t ) K_MK_PONG_BALL_WIDTH / 2.0f );

      /* Condition de rebond */
      if ( ( l_impactY >= p_pong->playground.j1.racket.y ) &&
           ( l_impactY <= ( p_pong->playground.j1.racket.y + ( float32_t ) K_MK_PONG_RACKET_HEIGHT ) ) )
      {
         /* Rebond : la balle repart vers la gauche (dx < 0) */
         mk_pong_handleRacketBounce ( p_pong, &p_pong->playground.j1, l_impactY, -1.0f );
         mk_pong_setSpeed ( p_pong );

         /* Reposition au bord exact pour éviter toute pénétration résiduelle */
         l_nextX = l_j1BorderX - ( float32_t ) K_MK_PONG_BALL_WIDTH;
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

   /* Raquette du joueur 2 (bord gauche de l'écran) */

   /* La balle va vers la gauche (l_vx < 0). */
   /* Son bord gauche (prevX) était encore APRÈS le bord droit de la raquette. */
   /* Son bord gauche (nextX) a franchi ou atteint ce bord droit. */
   l_j2BorderX = p_pong->playground.j2.racket.x + ( float32_t ) K_MK_PONG_RACKET_WIDTH;

   /* Condition de collision avec la raquette du joueur 2 */
   if ( ( l_vx < 0.0f ) && ( l_prevX > l_j2BorderX ) && ( l_nextX <= l_j2BorderX ) )
   {
      l_t = ( l_prevX - l_j2BorderX ) / ( -l_vx );
      l_impactY = l_prevY + l_t * l_vy + ( ( float32_t ) K_MK_PONG_BALL_WIDTH / 2.0f );

      /* Condition de rebond */
      if ( ( l_impactY >= p_pong->playground.j2.racket.y ) &&
           ( l_impactY <= ( p_pong->playground.j2.racket.y + ( float32_t ) K_MK_PONG_RACKET_HEIGHT ) ) )
      {
         /* Rebond : la balle repart vers la droite (dx > 0) */
         mk_pong_handleRacketBounce ( p_pong, &p_pong->playground.j2, l_impactY, 1.0f );
         mk_pong_setSpeed ( p_pong );

         l_nextX = l_j2BorderX;
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

   /* Mise à jour de la position finale */
   mk_vect2d_setCoord ( &p_pong->playground.ball.position, l_nextX, l_nextY );

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


