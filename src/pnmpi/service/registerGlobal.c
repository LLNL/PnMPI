/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2019 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2019 RWTH Aachen University, Federal Republic of Germany
 *
 *
 * P^nMPI is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation version 2.1 dated February 1999.
 *
 * P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with P^nMPI; if not, write to the
 *
 *   Free Software Foundation, Inc.
 *   51 Franklin St, Fifth Floor
 *   Boston, MA 02110, USA
 *
 *
 * Written by Martin Schulz, schulzm@llnl.gov.
 *
 * LLNL-CODE-402774
 */

#include <assert.h>
#include <stdlib.h>

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/service.h>


/** \brief Register a global.
 *
 * \details This function registers a new global\p global of the calling module.
 *
 *
 * \param global The global to be registered.
 *
 * \return \ref PNMPI_SUCCESS Global has been registered successfully.
 * \return \ref PNMPI_NOMEM Not enough memory to register global.
 *
 *
 * \ingroup PNMPI_Service_GetGlobalByName
 */
PNMPI_status_t
PNMPI_Service_RegisterGlobal(const PNMPI_Global_descriptor_t *global)
{
  assert(global);


  /* Allocate new memory to store a new global definition in the list of globals
   * of this module. */
  module_globlist_p newglobal =
    (module_globlist_p)malloc(sizeof(module_globlist_t));
  if (newglobal == NULL)
    {
      PNMPI_Warning("Can't allocate memory to register global '%s' for module "
                    "at level %d.\n",
                    global->name, pnmpi_level);
      return PNMPI_NOMEM;
    }

  /* Prepend the global to the modules global list. */
  newglobal->desc = *global;
  newglobal->next = modules.module[pnmpi_level]->globals;
  modules.module[pnmpi_level]->globals = newglobal;
  return PNMPI_SUCCESS;
}
