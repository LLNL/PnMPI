/*
Copyright (c) 2008
Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory.
Written by Martin Schulz, schulzm@llnl.gov.
LLNL-CODE-402774,
All rights reserved.

This file is part of P^nMPI.

Please also read the file "LICENSE" included in this package for
Our Notice and GNU Lesser General Public License.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
(as published by the Free Software Foundation) version 2.1
dated February 1999.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY
OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
terms and conditions of the GNU General Public License for more
details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the

Free Software Foundation, Inc.,
59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA
*/

#include "app_hooks.h"

#include "core.h"


#ifdef COMPILE_FOR_FORTRAN
void pmpi_finalize_(int *ierr);
#endif


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
