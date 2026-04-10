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
* @file mk_pong_init.c
* @brief Déclaration de la fonction mk_pong_init.c.
* @date 11 août 2023
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

static T_mkCode mk_pong_setupKeyboardInputs ( T_mkPongApplication* p_pong )
{
   /* Déclaration d'une structure de configuration des attributs */
   T_mkDispatcherAttributes l_attributes = {
         K_MK_POINTER_DEFAULT_RESOLUTION,          /* Résolution des curseurs. */
         K_MK_STICK_DEFAULT_RESOLUTION,            /* Résolution des sticks. */
         K_MK_GRAPHICS_FOREGROUND,                 /* Layer du curseur de la souris. */
         K_MK_FIELD_INVISIBLE,                     /* Visiblité du curseur de la souris. */
         K_MK_NULL,                                /* Curseur par défaut. */
         0, 0,                                     /* Largeur et hauteur du curseur (non utilisé si curseur par défaut). */
         50,                                       /* Durée entre un événement 'Down' et 'Hold'. */
         10,                                       /* Durée entre deux événements 'Hold'. */
   };

   /* Configuration des attributs */
   T_mkCode l_result = mk_application_setAttributes ( p_pong->application, &l_attributes, K_MK_NULL );

   /* Retour */
   return ( l_result );
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static T_mkCode mk_pong_init640x480 ( T_mkPongApplication* p_pong )
{
   /* Déclaration de la variable de retour */
   T_mkCode l_result = K_MK_OK;

   /* Déclaration d'un style */
   T_mkTextStyle l_style = { K_MK_NULL, K_MK_FONT_UTF8, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_LEFT_JUSTIFIED,
                             0, 0, 0, 20, { 0x00, 0x00, 0xFF, 0xFF } };

   /* Copie du style dans une variable persistante. */
   _copy ( &p_pong->background.buttonStyle, &l_style, sizeof ( T_mkTextStyle ) );

   /* Initialisation des données de l'application */
   mk_pong_initGames ( p_pong );

   /* Initialisation des layers */
   mk_layer_init ( &p_pong->background.layers );
   mk_layer_set ( &p_pong->background.layers, K_MK_LAYER_BACKGROUND_VISIBLE );

   /* Initialisation de l'arrière plan */
   mk_screen_init ( &p_pong->background.playground );
   mk_screen_setColor ( &p_pong->background.playground, K_MK_COLOR_ARGB_BLACK );
   mk_screen_setLayer ( &p_pong->background.playground, K_MK_GRAPHICS_BACKGROUND );
   mk_screen_setVisibility ( &p_pong->background.playground, K_MK_FIELD_VISIBLE );
   mk_screen_setActivity ( &p_pong->background.playground, K_MK_FIELD_ACTIF );
   mk_screen_setPainter ( &p_pong->background.playground, mk_pong_paintPlayground );
   mk_screen_setListener ( &p_pong->background.playground, mk_pong_listenPlayground );

   /* Configuration d'une police de caractères personalisée */
   mk_font_init ( &p_pong->background.font );
   mk_font_setHeight ( &p_pong->background.font, K_MK_PONG_FIELD_FONT_HEIGHT );
   mk_font_setNumberOfPixelsBetweenChars ( &p_pong->background.font, K_MK_FONT_DEFAULT_NUMBER_OF_PIXELS_BETWEEN_CHARS );
   mk_font_setNumberOfPixelsBetweenRows ( &p_pong->background.font, K_MK_FONT_DEFAULT_NUMBER_OF_PIXELS_BETWEEN_ROWS );
   mk_font_setTabulationNumberOfPixels ( &p_pong->background.font, K_MK_FONT_DEFAULT_TABULATION_PIXELS_NUMBER );
   mk_font_setPage ( &p_pong->background.page, K_MK_FONT_PAGE_BASIC_LATIN, ( uint32_t ) ( K_MK_FONT_PAGE_LATIN_1_SUPPLEMENT - 1 ), &g_mkFontDejaVuMathTeXGyre36ptBasicLatinPageDescriptor [ 0 ] [ 0 ], g_mkFontDejaVuMathTeXGyre36ptBasicLatinPage );
   mk_font_addPage ( &p_pong->background.font, &p_pong->background.page );

   /* Initialisation de la boite de score 'Joueur 1' */
   mk_textField_init ( &p_pong->background.scoreJ1 );
   mk_textField_setPosition ( &p_pong->background.scoreJ1, K_MK_PONG_FIELD_640_480_SCORE_J1_POSITION_X, K_MK_PONG_FIELD_640_480_SCORE_J1_POSITION_Y );
   mk_textField_setDimension ( &p_pong->background.scoreJ1, K_MK_PONG_FIELD_640_480_SCORE_J1_WIDTH, K_MK_PONG_FIELD_640_480_SCORE_J1_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.scoreJ1, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.scoreJ1, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.scoreJ1, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.scoreJ1, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.scoreJ1, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.scoreJ1, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.scoreJ1, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.scoreJ1, 1, 1, 1, 1 );
   mk_textField_setText ( &p_pong->background.scoreJ1,  p_pong->playground.j1.strScore );
   mk_textField_setTextEncoding ( &p_pong->background.scoreJ1, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.scoreJ1, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.scoreJ1, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.scoreJ1, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.scoreJ1, &p_pong->background.font );

   /* Initialisation de la boite de score 'Joueur 2' */
   mk_textField_init ( &p_pong->background.scoreJ2 );
   mk_textField_setPosition ( &p_pong->background.scoreJ2, K_MK_PONG_FIELD_640_480_SCORE_J2_POSITION_X, K_MK_PONG_FIELD_640_480_SCORE_J2_POSITION_Y );
   mk_textField_setDimension ( &p_pong->background.scoreJ2, K_MK_PONG_FIELD_640_480_SCORE_J2_WIDTH, K_MK_PONG_FIELD_640_480_SCORE_J2_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.scoreJ2, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.scoreJ2, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.scoreJ2, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.scoreJ2, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.scoreJ2, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.scoreJ2, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.scoreJ2, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.scoreJ2, 1, 1, 1, 1 );
   mk_textField_setText ( &p_pong->background.scoreJ2,  p_pong->playground.j2.strScore );
   mk_textField_setTextEncoding ( &p_pong->background.scoreJ2, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.scoreJ2, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.scoreJ2, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.scoreJ2, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.scoreJ2, &p_pong->background.font );

   /* Initialisation du bouton 'Start' */
   mk_textField_init ( &p_pong->background.buttonStart );
   mk_textField_setPosition ( &p_pong->background.buttonStart, K_MK_PONG_FIELD_640_480_MAIN_MENU_POSITION_X, K_MK_PONG_FIELD_640_480_MAIN_MENU_POSITION_Y );
   mk_textField_setDimension ( &p_pong->background.buttonStart, K_MK_PONG_FIELD_640_480_MAIN_MENU_BUTTON_WIDTH, K_MK_PONG_FIELD_640_480_MAIN_MENU_BUTTON_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.buttonStart, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.buttonStart, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.buttonStart, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.buttonStart, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.buttonStart, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.buttonStart, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.buttonStart, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.buttonStart, 5, 1, 1, 40 );
   mk_textField_setText ( &p_pong->background.buttonStart,  ( T_str8 ) "Play" );
   mk_textField_setTextEncoding ( &p_pong->background.buttonStart, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.buttonStart, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.buttonStart, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.buttonStart, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.buttonStart, &p_pong->background.font );
   mk_textField_setPainter ( &p_pong->background.buttonStart, mk_pong_paintButton );

   /* Initialisation du bouton 'Quit' */
   mk_textField_init ( &p_pong->background.buttonQuit );
   mk_textField_setPosition ( &p_pong->background.buttonQuit, K_MK_PONG_FIELD_640_480_MAIN_MENU_POSITION_X,
                              K_MK_PONG_FIELD_640_480_MAIN_MENU_POSITION_Y + K_MK_PONG_FIELD_640_480_MAIN_MENU_BUTTON_HEIGHT + K_MK_PONG_FIELD_640_480_MAIN_MENU_BUTTON_GAP );
   mk_textField_setDimension ( &p_pong->background.buttonQuit, K_MK_PONG_FIELD_640_480_MAIN_MENU_BUTTON_WIDTH, K_MK_PONG_FIELD_640_480_MAIN_MENU_BUTTON_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.buttonQuit, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.buttonQuit, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.buttonQuit, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.buttonQuit, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.buttonQuit, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.buttonQuit, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.buttonQuit, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.buttonQuit, 5, 1, 1, 40 );
   mk_textField_setText ( &p_pong->background.buttonQuit,  ( T_str8 ) "Quit" );
   mk_textField_setTextEncoding ( &p_pong->background.buttonQuit, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.buttonQuit, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.buttonQuit, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.buttonQuit, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.buttonQuit, &p_pong->background.font );
   mk_textField_setPainter ( &p_pong->background.buttonQuit, mk_pong_paintButton );

   /* Retour */
   return ( l_result );
}

/**
 * @internal
 * @brief
 * @endinternal
 */

static T_mkCode mk_pong_init480x272 ( T_mkPongApplication* p_pong )
{
   /* Déclaration de la variable de retour */
   T_mkCode l_result = K_MK_OK;

   /* Déclaration d'un style */
   T_mkTextStyle l_style = { K_MK_NULL, K_MK_FONT_UTF8, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_LEFT_JUSTIFIED,
                             0, 0, 0, 20, { 0x00, 0x00, 0xFF, 0xFF } };

   /* Copie du style dans une variable persistante. */
   _copy ( &p_pong->background.buttonStyle, &l_style, sizeof ( T_mkTextStyle ) );

   /* Initialisation des données de l'application */
   mk_pong_initGames ( p_pong );

   /* Initialisation des layers */
   mk_layer_init ( &p_pong->background.layers );
   mk_layer_set ( &p_pong->background.layers, K_MK_LAYER_BACKGROUND_VISIBLE );

   /* Initialisation de l'arrière plan */
   mk_screen_init ( &p_pong->background.playground );
   mk_screen_setColor ( &p_pong->background.playground, K_MK_COLOR_ARGB_BLACK );
   mk_screen_setLayer ( &p_pong->background.playground, K_MK_GRAPHICS_BACKGROUND );
   mk_screen_setVisibility ( &p_pong->background.playground, K_MK_FIELD_VISIBLE );
   mk_screen_setActivity ( &p_pong->background.playground, K_MK_FIELD_ACTIF );
   mk_screen_setPainter ( &p_pong->background.playground, mk_pong_paintPlayground );
   mk_screen_setListener ( &p_pong->background.playground, mk_pong_listenPlayground );

   /* Configuration d'une police de caractères personalisée */
   mk_font_init ( &p_pong->background.font );
   mk_font_setHeight ( &p_pong->background.font, K_MK_PONG_FIELD_FONT_HEIGHT );
   mk_font_setNumberOfPixelsBetweenChars ( &p_pong->background.font, K_MK_FONT_DEFAULT_NUMBER_OF_PIXELS_BETWEEN_CHARS );
   mk_font_setNumberOfPixelsBetweenRows ( &p_pong->background.font, K_MK_FONT_DEFAULT_NUMBER_OF_PIXELS_BETWEEN_ROWS );
   mk_font_setTabulationNumberOfPixels ( &p_pong->background.font, K_MK_FONT_DEFAULT_TABULATION_PIXELS_NUMBER );
   mk_font_setPage ( &p_pong->background.page, K_MK_FONT_PAGE_BASIC_LATIN, ( uint32_t ) ( K_MK_FONT_PAGE_LATIN_1_SUPPLEMENT - 1 ), &g_mkFontDejaVuMathTeXGyre36ptBasicLatinPageDescriptor [ 0 ] [ 0 ], g_mkFontDejaVuMathTeXGyre36ptBasicLatinPage );
   mk_font_addPage ( &p_pong->background.font, &p_pong->background.page );

   /* Initialisation de la boite de score 'Joueur 1' */
   mk_textField_init ( &p_pong->background.scoreJ1 );
   mk_textField_setPosition ( &p_pong->background.scoreJ1, K_MK_PONG_FIELD_480_272_SCORE_J1_POSITION_X, K_MK_PONG_FIELD_480_272_SCORE_J1_POSITION_Y );
   mk_textField_setDimension ( &p_pong->background.scoreJ1, K_MK_PONG_FIELD_480_272_SCORE_J1_WIDTH, K_MK_PONG_FIELD_480_272_SCORE_J1_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.scoreJ1, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.scoreJ1, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.scoreJ1, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.scoreJ1, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.scoreJ1, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.scoreJ1, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.scoreJ1, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.scoreJ1, 1, 1, 1, 1 );
   mk_textField_setText ( &p_pong->background.scoreJ1,  p_pong->playground.j1.strScore );
   mk_textField_setTextEncoding ( &p_pong->background.scoreJ1, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.scoreJ1, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.scoreJ1, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.scoreJ1, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.scoreJ1, &p_pong->background.font );

   /* Initialisation de la boite de score 'Joueur 2' */
   mk_textField_init ( &p_pong->background.scoreJ2 );
   mk_textField_setPosition ( &p_pong->background.scoreJ2, K_MK_PONG_FIELD_480_272_SCORE_J2_POSITION_X, K_MK_PONG_FIELD_480_272_SCORE_J2_POSITION_Y );
   mk_textField_setDimension ( &p_pong->background.scoreJ2, K_MK_PONG_FIELD_480_272_SCORE_J2_WIDTH, K_MK_PONG_FIELD_480_272_SCORE_J2_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.scoreJ2, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.scoreJ2, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.scoreJ2, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.scoreJ2, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.scoreJ2, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.scoreJ2, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.scoreJ2, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.scoreJ2, 1, 1, 1, 1 );
   mk_textField_setText ( &p_pong->background.scoreJ2,  p_pong->playground.j2.strScore );
   mk_textField_setTextEncoding ( &p_pong->background.scoreJ2, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.scoreJ2, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.scoreJ2, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.scoreJ2, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.scoreJ2, &p_pong->background.font );

   /* Initialisation du bouton 'Start' */
   mk_textField_init ( &p_pong->background.buttonStart );
   mk_textField_setPosition ( &p_pong->background.buttonStart, K_MK_PONG_FIELD_480_272_MAIN_MENU_POSITION_X, K_MK_PONG_FIELD_480_272_MAIN_MENU_POSITION_Y );
   mk_textField_setDimension ( &p_pong->background.buttonStart, K_MK_PONG_FIELD_480_272_MAIN_MENU_BUTTON_WIDTH, K_MK_PONG_FIELD_480_272_MAIN_MENU_BUTTON_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.buttonStart, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.buttonStart, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.buttonStart, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.buttonStart, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.buttonStart, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.buttonStart, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.buttonStart, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.buttonStart, 5, 1, 1, 40 );
   mk_textField_setText ( &p_pong->background.buttonStart,  ( T_str8 ) "Play" );
   mk_textField_setTextEncoding ( &p_pong->background.buttonStart, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.buttonStart, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.buttonStart, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.buttonStart, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.buttonStart, &p_pong->background.font );
   mk_textField_setPainter ( &p_pong->background.buttonStart, mk_pong_paintButton );

   /* Initialisation du bouton 'Quit' */
   mk_textField_init ( &p_pong->background.buttonQuit );
   mk_textField_setPosition ( &p_pong->background.buttonQuit, K_MK_PONG_FIELD_480_272_MAIN_MENU_POSITION_X,
                              K_MK_PONG_FIELD_480_272_MAIN_MENU_POSITION_Y + K_MK_PONG_FIELD_480_272_MAIN_MENU_BUTTON_HEIGHT + K_MK_PONG_FIELD_480_272_MAIN_MENU_BUTTON_GAP );
   mk_textField_setDimension ( &p_pong->background.buttonQuit, K_MK_PONG_FIELD_480_272_MAIN_MENU_BUTTON_WIDTH, K_MK_PONG_FIELD_480_272_MAIN_MENU_BUTTON_HEIGHT );
   mk_textField_setLayer ( &p_pong->background.buttonQuit, K_MK_GRAPHICS_BACKGROUND );
   mk_textField_setBorderVisibility ( &p_pong->background.buttonQuit, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBorderColor ( &p_pong->background.buttonQuit, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setBackgroundVisibility ( &p_pong->background.buttonQuit, K_MK_GRAPHICS_INVISIBLE );
   mk_textField_setBackgroundColor ( &p_pong->background.buttonQuit, K_MK_COLOR_ARGB_BLACK );
   mk_textField_setTextColor ( &p_pong->background.buttonQuit, K_MK_COLOR_ARGB_WHITE );
   mk_textField_setTextAlignment ( &p_pong->background.buttonQuit, K_MK_TEXT_CENTER_JUSTIFIED, K_MK_TEXT_CENTER_JUSTIFIED );
   mk_textField_setMargin ( &p_pong->background.buttonQuit, 5, 1, 1, 40 );
   mk_textField_setText ( &p_pong->background.buttonQuit,  ( T_str8 ) "Quit" );
   mk_textField_setTextEncoding ( &p_pong->background.buttonQuit, K_MK_FONT_UTF8 );
   mk_textField_setVisibility ( &p_pong->background.buttonQuit, K_MK_FIELD_VISIBLE );
   mk_textField_setActivity ( &p_pong->background.buttonQuit, K_MK_FIELD_ACTIF );
   mk_textField_setCache ( &p_pong->background.buttonQuit, ( T_mkAddr ) K_MK_NULL );
   mk_textField_setFont ( &p_pong->background.buttonQuit, &p_pong->background.font );
   mk_textField_setPainter ( &p_pong->background.buttonQuit, mk_pong_paintButton );

   /* Retour */
   return ( l_result );
}

/**
 * @internal
 * @brief
 * @endinternal
 */

T_mkCode mk_pong_init ( T_mkPongApplication* p_pong )
{
   /* Déclaration de la variable de retour */
   T_mkCode l_result = K_MK_OK;
   
   /* Déclaration des variables de travail */
   uint32_t l_width = 0, l_height = 0;

   /* Récupération du couple largeur/hauteur */
   l_width  = mk_display_getWidth ( );
   l_height = mk_display_getHeight ( );

   /* Si l'écran est un écran 640_480 */
   if ( ( l_width == 640 ) && ( l_height == 480 ) )
   {
      /* Initialisation de l'application avec ces dimensions */
      mk_pong_init640x480 ( p_pong );
   }

   /* Sinon si l'écran est un écarn 480x272 */
   else if ( ( l_width == 480 ) && ( l_height == 272 ) )
   {
      /* Initialisation de l'application avec ces dimensions */
      mk_pong_init480x272 ( p_pong );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Enregistrement de l'adresse de l'application */
   p_pong->application = mk_application_getCurrent ( );

   /* Initialisation d'un container */
   l_result = mk_container_create ( &p_pong->container, 0, mk_pong_listenApplication, K_MK_NULL );

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Ajout des champs dans le container */
      l_result  = mk_container_addField ( p_pong->container, &p_pong->background.playground.field, K_MK_NULL );
      l_result |= mk_container_addField ( p_pong->container, &p_pong->background.layers.field, K_MK_NULL );
      l_result |= mk_container_addField ( p_pong->container, &p_pong->background.buttonQuit.field, K_MK_NULL );
      l_result |= mk_container_addField ( p_pong->container, &p_pong->background.buttonStart.field, K_MK_NULL );
      l_result |= mk_container_addField ( p_pong->container, &p_pong->background.scoreJ1.field, K_MK_NULL );
      l_result |= mk_container_addField ( p_pong->container, &p_pong->background.scoreJ2.field, K_MK_NULL );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Ajout du container dans la factory */
      l_result = mk_factory_addContainer ( p_pong->container, K_MK_NULL );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Configuration des caractéristiques du clavier */
      l_result = mk_pong_setupKeyboardInputs ( p_pong );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Configuration du mode de jeu */
   p_pong->playground.mode = K_MK_PONG_MODE_PLAYER;

   /* Retour */
   return ( l_result );
}
