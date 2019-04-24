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

#include "core.h"
#include <pnmpi/private/modules.h>
#include <pnmpi/service.h>


/** \brief Get a modules global.
 *
 * \details This function searches for global \p name of module \p handle with
 *  signature \p sig. If such a global was found, a handle will be stored in \p
 *  dest.
 *
 *
 * \param handle The module to be checked.
 * \param name Name of the global to be searched.
 * \param sig Signature of the global to be searched.
 * \param [out] dest Where to store the pointer to the global struct.
 *
 * \return \ref PNMPI_SUCCESS Successfully stored pointer to global in \p dest.
 * \return \ref PNMPI_NOMODULE \p handle is no valid module handle.
 * \return \ref PNMPI_NOGLOBAL No global named \p name found.
 * \return \ref PNMPI_SIGNATURE One or more globals named \p name have been
 *  found, but the signatures did not match.
 *
 *
 * \ingroup PNMPI_Service_GetGlobalByName
 */
PNMPI_status_t PNMPI_Service_GetGlobalByName(const PNMPI_modHandle_t handle,
                                             const char *name, const char sig,
                                             PNMPI_Global_descriptor_t *dest)
{
  assert(name);
  assert(dest);

  /* Check, if module is available and return an error code, if it's not
   * available.
   *
   * NOTE: This check will NOT be disabled for PNMPI_NO_DEBUG, as it's essential
   *       for accessing the module by ID below */
  if (!pnmpi_valid_modhandle(handle))
    return PNMPI_NOMODULE;


  int err = PNMPI_NOGLOBAL;

  /* Iterate over the globals list of the module. Compare the name and signature
   * of each global with the searched values from the parameters. If they match
   * copy a pointer to the global struct into dest and return success. */
  module_globlist_p g;
  for (g = modules.module[handle]->globals; g != NULL; g = g->next)
    {
      if (strcmp(g->desc.name, name) == 0)
        {
          if (g->desc.sig == sig)
            {
              *dest = g->desc;
              return PNMPI_SUCCESS;
            }
          else
            /* A global with matching name was found, but the signatures did not
             * match. If no global with matching signature will be found, this
             * error code indicates the mismatch for the user. */
            err = PNMPI_SIGNATURE;
        }
    }

  return err;
}
