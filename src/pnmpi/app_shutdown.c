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

#include <mpi.h>

#include "attributes.h"
#include "core.h"


#ifdef COMPILE_FOR_FORTRAN
void pmpi_finalize_(int *ierr);
#endif


PNMPI_DESTRUCTOR(110)
void pnmpi_app_shutdown()
{
  /* There are two conditions to execute this function:
   *
   * 1. PnMPI must have been initialized before. If PnMPI (and MPI) have not
   *    been initalized, nothing can be shutdown.
   * 2. app_shutdown must be enabled, otherwise MPI was shutdown before in the
   *    MPI_Finalize wrapper. */
  if (!pnmpi_init_done || !pnmpi_hook_activated("app_shutdown"))
    return;


  pnmpi_call_hook("app_shutdown");


  /* There are some bugs in OpenMPI, sending Fortran PMPI-calls to C MPI calls.
   * But C MPI is wrapped by PnMPI, so a fortran pmpi_finalize_ will never reach
   * a real PMPI_Finalize but runs into an endless loop. To fix this, the
   * following variable should act as a guard, notifing the wrapped MPI_Finalize
   * that it should call PMPI_Finalize instead of skipping it. */
  pnmpi_finalize_done = 1;

#ifdef COMPILE_FOR_FORTRAN
  if (pnmpi_init_was_fortran)
    {
      int res;
      pmpi_finalize_(&res);
    }
  else
#endif
    PMPI_Finalize();
}
