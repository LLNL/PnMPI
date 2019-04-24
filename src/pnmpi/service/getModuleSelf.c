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
#include <pnmpi/service.h>


/** \brief Get handle of own module.
 *
 * \note This function is part of the regular service interface, as it is
 *  required to use this interface. The self service interface is only for
 *  functions to query data for the own module, which is provided by the regular
 *  interface, too.
 *
 *
 * \param handle Where to store the module handle of the calling module.
 *
 * \return \ref PNMPI_SUCCESS Module handle successfully written to \p handle.
 *
 *
 * \ingroup PNMPI_Service_GetModuleByName
 */
PNMPI_status_t PNMPI_Service_GetModuleSelf(PNMPI_modHandle_t *handle)
{
  assert(handle);


  *handle = pnmpi_level;
  return PNMPI_SUCCESS;
}
