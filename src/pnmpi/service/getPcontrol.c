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
#include <pnmpi/debug_io.h>
#include <pnmpi/private/modules.h>
#include <pnmpi/service.h>


/** \brief Get the Pcontrol value of module \p handle.
 *
 *
 * \param handle The module to be checked.
 * \param [out] flag Where to store the result.
 *
 * \return \ref PNMPI_SUCCESS Successfully stored the result in \p flag.
 * \return \ref PNMPI_NOMODULE \p handle is no valid module handle.
 *
 *
 * \ingroup PNMPI_Service_GetPcontrol
 */
PNMPI_status_t PNMPI_Service_GetPcontrol(const PNMPI_modHandle_t handle,
                                         int *flag)
{
  assert(flag);

  /* Check, if module is available and return an error code, if it's not
   * available.
   *
   * NOTE: This check will NOT be disabled for PNMPI_NO_DEBUG, as it's essential
   *       for accessing the module by ID below. */
  if (!pnmpi_valid_modhandle(handle))
    return PNMPI_NOMODULE;


  *flag = modules.module[handle]->pcontrol;
  return PNMPI_SUCCESS;
}


/** \brief Get the Pcontrol value of the calling module.
 *
 *
 * \return The Pcontrol configuration value of the calling module.
 *
 *
 * \ingroup PNMPI_Service_GetPcontrol
 */
int PNMPI_Service_GetPcontrolSelf(void)
{
  return modules.module[pnmpi_level]->pcontrol;
}
