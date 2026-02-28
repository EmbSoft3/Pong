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
* @file mk_pong_types.h
* @brief Déclaration des types dédiés à l'application 'Pong'.
* @date 11 août 2023
*
*/

#ifndef MK_PONG_TYPES_H
#define MK_PONG_TYPES_H

/**
 * @struct T_mkPongBackgroundFields
 * @brief Déclaration de la structure T_mkPongBackgroundFields.
 *
 */

typedef struct T_mkPongBackgroundFields T_mkPongBackgroundFields;
struct T_mkPongBackgroundFields
{
   T_mkFont font;
   T_mkFontPage page;
   T_mkLayer layers;
   T_mkScreen playground;
   T_mkTextField scoreJ1;
   T_mkTextField scoreJ2;
   T_mkTextField buttonStart;
   T_mkTextField buttonQuit;
   T_mkTextStyle buttonStyle;
};

/**
 * @struct T_mkPongPlayer
 * @brief Déclaration de la structure T_mkPongPlayer.
 *
 */

typedef struct T_mkPongPlayer T_mkPongPlayer;
struct T_mkPongPlayer
{
   T_mkVect2D racket;
   uint8_t intScore;
   uint8_t strScore [ 3 ];
   float32_t delta;
};

/**
 * @struct T_mkPongBall
 * @brief Déclaration de la structure T_mkPongBall.
 *
 */

typedef struct T_mkPongBall T_mkPongBall;
struct T_mkPongBall
{
   T_mkVect2D position;
   float32_t dx;
   float32_t dy;
   float32_t speed;
};

/**
 * @struct T_mkPongPlayground
 * @brief Déclaration de la structure T_mkPongPlayground.
 *
 */

typedef struct T_mkPongPlayground T_mkPongPlayground;
struct T_mkPongPlayground
{
   T_mkPongPlayer j1;
   T_mkPongPlayer j2;
   T_mkPongBall ball;
   uint32_t startGame;
   uint32_t mode;
};

/**
 * @struct T_mkPongApplication
 * @brief Déclaration de la structure T_mkPongApplication.
 *
 */

typedef struct T_mkPongApplication T_mkPongApplication;
struct T_mkPongApplication
{
   T_mkApplication* application;
   T_mkPongBackgroundFields background;
   T_mkPongPlayground playground;
   T_mkContainer* container;
};

/**
 *
 */

#endif

