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
#include <stdio.h>

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/service.h>


/** \brief Register a module.
 *
 * \details This function registers the calling module with \p name.
 *
 *
 * \param name The name of the module to be registered.
 *
 * \return \ref PNMPI_SUCCESS The module has been registered.
 *
 *
 * \ingroup PNMPI_Service_GetModuleByName
 */
PNMPI_status_t PNMPI_Service_RegisterModule(const char *name)
{
  assert(name);


  /* Copy name into the module name buffer. If the name is longer than the
   * supplied buffer, a warning will be printed. */
  if (snprintf(modules.module[pnmpi_level]->username, PNMPI_MODULE_USERNAMELEN,
               "%s", name) >= PNMPI_MODULE_USERNAMELEN)
    PNMPI_Warning("Module name '%s' of module %d was too long.\n", name,
                  pnmpi_level);

  /* Mark the module as registered. */
  modules.module[pnmpi_level]->registered = 1;


  return PNMPI_SUCCESS;
}
