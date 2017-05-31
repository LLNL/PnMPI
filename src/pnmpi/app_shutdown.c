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

#include <mpi.h>

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/private/app_hooks.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/modules.h>
#include <pnmpi/private/mpi_interface.h>
#include <pnmpi/private/pmpi.h>
#include <pnmpi/private/pmpi_assert.h>


/** \brief Handler for the PnMPI app shutdown hooks.
 *
 * \details This function will be called to call the app shutdown hooks. After
 *  all hooks have been called, the MPI environment will be destroyed.
 */
PNMPI_INTERNAL
void pnmpi_app_shutdown()
{
  /* There are two conditions to execute this function:
   *
   * 1. PnMPI must have been initialized before. If PnMPI (and MPI) have not
   *    been initalized, nothing can be shutdown.
   * 2. PNMPI_AppShutdown must be enabled, otherwise MPI was shutdown before in
   *    the MPI_Finalize wrapper. */
  if (!pnmpi_init_done || !pnmpi_hook_activated("PNMPI_AppShutdown", 0))
    return;

  /* In some cases modules may have called PMPI_Finalize which was not patched
   * and redirected to PnMPI,so MPI is not active anymore. If the
   * PNMPI_AppShutdown hook is called under these conditions, MPI will crash, so
   * a check is performed to ensure that MPI is still active. */
  int mpi_initialized, mpi_finalized;
  PMPI_Initialized_assert(&mpi_initialized);
  PMPI_Finalized_assert(&mpi_finalized);

  if (!mpi_initialized || mpi_finalized)
    return;


  pnmpi_call_hook("PNMPI_AppShutdown", PNMPI_CALL_HOOK_NEXT_MODULE, 0);


#ifdef COMPILE_FOR_FORTRAN
  if (pnmpi_get_mpi_interface(NULL) == PNMPI_INTERFACE_FORTRAN)
    {
      int res;
      pmpi_finalize_assert_(&res);
    }
  else
#endif
    PMPI_Finalize_assert();
}
