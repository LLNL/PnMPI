/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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

#include "core.h"
#include "pnmpi-config.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/private/app_hooks.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/config.h>
#include <pnmpi/private/modules.h>
#include <pnmpi/private/mpi_interface.h>


int pnmpi_mpi_thread_level_provided;


PNMPI_CONSTRUCTOR(110)
void pnmpi_app_startup(int argc, char **argv)
{
  /* If no module provides the PNMPI_AppStartup hook, we can skip the execution
   * of this function. */
  if (!pnmpi_hook_activated("PNMPI_AppStartup") &&
      !pnmpi_hook_activated("PNMPI_AppStartupOptional"))
    return;


  /* Before executing the PNMPI_AppStartup hook, we have to initialize the MPI
   * environment, so it may be used by the hooks.
   *
   * PnMPI will be initialized with threading support, as we don't know about
   * the threading of the instrumented application. By default the highest
   * possible threading level will be used, but it may be limited by the user
   * for performance. */
  int required = pnmpi_max_module_threading_level();
  if (getenv("PNMPI_THREADING_LEVEL") != NULL)
    required = atoi(getenv("PNMPI_THREADING_LEVEL"));

  /* Initialize MPI depending on the MPI interface used by the instrumented
   * application. */
  int err;
  switch (pnmpi_get_mpi_interface(argv[0]))
    {
    case PNMPI_INTERFACE_C:
    case PNMPI_INTERFACE_NONE:
      pnmpi_debug(PNMPI_DEBUG_INIT, "Initialize C MPI interface ...\n");

#ifdef HAVE_MPI_INIT_THREAD_C
      err = PMPI_Init_thread(&argc, &argv, required,
                             &pnmpi_mpi_thread_level_provided);
      if (err == MPI_SUCCESS)
        pnmpi_debug(PNMPI_DEBUG_INIT, "Provided threading level: %d\n",
                    pnmpi_mpi_thread_level_provided);
#else
      err = PMPI_Init(&argc, &argv);
#endif
      break;

#ifdef COMPILE_FOR_FORTRAN
    /* The Fortran MPI interface will be used for staticaly linked applications
     * and applications without MPI, too. This should have no side-effects. */
    case PNMPI_INTERFACE_FORTRAN:
      pnmpi_debug(PNMPI_DEBUG_INIT, "Initialize Fortran MPI interface ...\n");

#ifdef HAVE_MPI_INIT_THREAD_Fortran
      pmpi_init_thread_(&required, &pnmpi_mpi_thread_level_provided, &err);
      if (err == MPI_SUCCESS)
        pnmpi_debug(PNMPI_DEBUG_INIT, "Provided threading level: %d\n",
                    pnmpi_mpi_thread_level_provided);
#else
      pmpi_init_(&err);
#endif
      break;
#endif

    default:
      pnmpi_error("Automatic detection of the used MPI interface failed.\n");
    }

  if (err != MPI_SUCCESS)
    pnmpi_error("MPI initialization failed.\n");
  pnmpi_init_done = 1;


  pnmpi_print_banner();


  pnmpi_call_hook("PNMPI_AppStartup");
  pnmpi_call_hook("PNMPI_AppStartupOptional");
}
