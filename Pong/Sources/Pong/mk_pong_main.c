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
* @file mk_pong_main.c
* @brief Déclaration de la fonction mk_pong_main.c.
* @date 11 août 2023
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

void mk_pong_main ( T_mkAddr p_param )
{
   /* Initialisation de l'application */
   T_mkCode l_result = mk_pong_init ( ( T_mkPongApplication* ) &g_mkPongApplication );

   /* Si aucune erreur ne s'est produite */
   if ( l_result == K_MK_OK )
   {
      /* Arrêt de la tâche */
      l_result = mk_task_terminate ( K_MK_NULL );
   }

   /* Sinon */
   else
   {
      /* Ne rien faire */
   }

   /* Boucle pour toujours */
   while ( 1 )
   {
      /* ... */
   }

   ( void ) p_param;

   /* Retour */
   return;
}
