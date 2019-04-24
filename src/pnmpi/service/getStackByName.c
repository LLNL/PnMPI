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


/** \brief Find a stack by name.
 *
 * \details This function searches for a stack named \p name in the list of
 *  loaded modules.
 *
 *
 * \param name Name of the stack to be searched.
 * \param [out] handle Where to store the handle of the first module in the
 *  found stack.
 *
 * \return \ref PNMPI_SUCCESS Stack was found and handle written to \p handle.
 * \return \ref PNMPI_NOSTACK No stack with the required name \p name has been
 *  found.
 *
 *
 * \ingroup PNMPI_Service_GetStackByName
 */
PNMPI_status_t PNMPI_Service_GetStackByName(const char *name,
                                            PNMPI_modHandle_t *handle)
{
  assert(name);
  assert(handle);


  int i;
  for (i = 0; i < modules.num; i++)
    if (modules.module[i]->stack_delimiter)
      if (strcmp(modules.module[i]->name, name) == 0)
        {
          /* Found a stack with equal name. A handle to the first module in this
           * stack will be written into the provided pointer and success
           * returned. */
          *handle = i + 1;
          return PNMPI_SUCCESS;
        }

  /* No stack with the required name has been found. */
  return PNMPI_NOSTACK;
}
