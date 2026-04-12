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
* @file mk_application_data.c
* @brief Déclaration de la fonction mk_application_data.
* @date 2 janv. 2024
*
*/

#include "mk.h"
#include "mk_pong_constants.h"
#include "mk_pong_types.h"
#include "mk_pong.h"

/**
 * @def K_MK_SECTION_STACK
 * @brief Cette constante définit la zone mémoire où la pile de l'application doit être allouée. \n
 *        L'adresse de cette section peut être modifiée dans le fichier d'édition de lien.
 */

#define K_MK_SECTION_STACK __attribute__ ((section(".stack"),used))

/**
 * @def K_MK_SECTION_STACK
 * @brief Cette constante définit la zone mémoire où le descripteur d'application doit être alloué. \n
 *        L'adresse de cette section peut être modifiée dans le fichier d'édition de lien.
 */

#define K_MK_SECTION_DESCRIPTOR __attribute__ ((section(".descriptor"),used))

/**
 * @internal
 * @brief
 * @endinternal
 */

K_MK_SECTION_STACK uint32_t g_mkStack [ K_MK_PONG_STACK_SIZE ];

/**
 * @internal
 * @brief
 * @endinternal
 */

/* Déclaration d'un descripteur d'application */
K_MK_SECTION_DESCRIPTOR T_mkApplicationDynamicHeader g_mkApplicationDynamicHeader =
{
   /* Entête de l'application */
   {
      K_MK_DISPLAY_APPLICATION_DYNAMICHEADER_MAGICNUMBER,      /* Magic Number */
      sizeof ( T_mkApplicationDynamicHeader ),                 /* Taille de l'entête */
      0xFFFFFFFF,                                              /* Reserved */
      1,                                                       /* Version */
      &g_mkApplicationDynamicHeader.descriptor,                /* Adresse du descripteur */
      sizeof ( T_mkApplicationDescriptor ),                    /* Taille du descripteur */
      &g_mkApplicationDynamicHeader.metadata,                  /* Adresse des métadonnées */
      sizeof ( T_mkApplicationMetadata ),                      /* Taille des métadonnées */
      &g_mkApplicationDynamicHeader.args,                      /* Adresse de la structure stockant les arguments saisis en ligne de commande */
      sizeof ( T_mkApplicationCmdLineArguments )               /* Taille de la structure stockant les arguments saisis en ligne de commande */
   },

   /* Descripteur d'application */
   {
      K_MK_APPLICATION_TYPE_DYNAMIC,                           /* Application statique */
      K_MK_TYPE_FLOATING,                                      /* Tâche flottante non privilégiée. */
      K_MK_FALSE,                                              /* Pas d'allocation de page mémoire graphique */
      K_MK_FALSE,                                              /* Pas d'allocation de page mémoire standard */
      g_mkStack,                                               /* Adresse de la stack */
      K_MK_PONG_STACK_SIZE,                                    /* Taille de la stack. */
      mk_pong_main,                                            /* Point d'entrée de l'application */
      K_MK_NULL                                                /* Argument de l'application */
   },

   /* Métadonnées de l'application */
   {
      ( T_str8 ) "Pong",                                       /* Nom de l'application. */
      ( T_str8 ) "Mac Lewis",                                  /* Auteur de l'application. */
      ( T_str8 ) "V1.2",                                       /* Version de l'application. */
      ( T_str8 ) "Just a Pong Game",                           /* Description de l'application. */
      ( T_str8 ) "mk_pong.bmp",                                /* Chemin vers l'icône de l'application. */
      K_MK_NULL                                                /* Adresse d'un bitmap */
   },

   /* Arguments de l'application */
   {
      { 0 }                                                    /* Chaine de caractères contenant les arguments saisis en ligne de commande */
   }
};


