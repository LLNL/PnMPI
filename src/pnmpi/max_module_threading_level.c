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

#include "core.h"

#include <mpi.h>

#include <pnmpi/private/attributes.h>
#include <pnmpi/private/tls.h>


/* If PnMPI has no thread local storage, it can't be used thread safe and the
 * max. allowed thread level of MPI has to be limited. */
#ifndef PNMPI_COMPILER_NO_TLS
#define PNMPI_MAX_THREAD_LEVEL MPI_THREAD_MULTIPLE;
#else
#define PNMPI_MAX_THREAD_LEVEL MPI_THREAD_SERIALIZED;
#endif


/** \brief Get the maximum supported threading level of all modules.
 *
 * \details This function iterates over all modules and finds the minimum
 *  supported MPI threading level of all modules.
 *
 *
 * \return The maximum supported MPI level of all modules.
 */
PNMPI_INTERNAL
int pnmpi_max_module_threading_level()
{
  int max_level = PNMPI_MAX_THREAD_LEVEL;

  size_t i;
  for (i = 0; i < modules.num; i++)
    {
      /* Stack delimiter may be ignored: they are no real modules, so they can't
       * have any symbols defined. */
      if (modules.module[i]->stack_delimiter)
        continue;

      const int *sym =
        find_symbol(modules.module[i], "PNMPI_SupportedThreadingLevel");
      if (sym != NULL)
        if (*sym < max_level)
          max_level = *sym;
    }

  return max_level;
}
