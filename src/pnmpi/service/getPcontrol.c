/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

#include <pnmpi/debug_io.h>
#include <pnmpi/service.h>

#include "core.h"


/** \brief Get the Pcontrol value of module \p handle.
 *
 *
 * \param handle The module to be checked.
 * \param [out] flag Where to store the result.
 *
 * \return \ref PNMPI_SUCCESS Successfully stored the result in \p flag.
 *
 *
 * \ingroup pnmpi_service
 */
pnmpi_status PNMPI_Service_GetPcontrol(PNMPI_modHandle_t handle, int *flag)
{
  /* Check, if module is available. If the module is not available, an error
   * message will be printed and the application exits immediately. */
  if (handle >= modules.num)
    pnmpi_error("Unknown module %d.\n", handle);


  *flag = modules.module[handle]->pcontrol;
  return PNMPI_SUCCESS;
}


/** \brief Get the Pcontrol value of the calling module.
 *
 *
 * \return The Pcontrol configuration value of the calling module.
 *
 *
 * \ingroup pnmpi_service_self
 */
int PNMPI_Service_GetPcontrolSelf()
{
  return modules.module[pnmpi_level]->pcontrol;
}
