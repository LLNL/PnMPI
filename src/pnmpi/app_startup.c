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

#include <stdlib.h>

#include <mpi.h>

#include "app_hooks.h"
#include "attributes.h"
#include "core.h"


int pnmpi_mpi_thread_level_provided;


PNMPI_CONSTRUCTOR(110)
void pnmpi_app_startup(int argc, char **argv)
{
  /* If no module provides the app_startup hook, we can skip this function. */
  if (!pnmpi_hook_activated("app_startup"))
    return;


  /* Before executing the app_startup hook, we have to initialize the MPI
   * environment, so it may be used by the hooks. */

  int required = pnmpi_max_module_threading_level();
  if (getenv("PNMPI_THREADING_LEVEL") != NULL)
    required = atoi(getenv("PNMPI_THREADING_LEVEL"));

  if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_C)
    {
      pnmpi_init_was_fortran = 0;
      PMPI_Init_thread(&argc, &argv, required,
                       &pnmpi_mpi_thread_level_provided);
    }
  else
    {
      int err;
      pnmpi_init_was_fortran = 1;
      pmpi_init_thread_(&required, &pnmpi_mpi_thread_level_provided, &err);
    }
  pnmpi_init_done = 1;


  pnmpi_call_hook("app_startup");
}
