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

#include <mpi.h>

#include "core.h"


void pnmpi_app_startup()
{
  int required = MPI_THREAD_MULTIPLE, provided;

  if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_C)
    {
      int argc;
      char **argv;
      read_cmdline(&argc, &argv);

      pnmpi_init_was_fortran = 0;
      PMPI_Init_thread(&argc, &argv, required, &provided);
    }
  else
    {
      int err;
      pnmpi_init_was_fortran = 1;
      pmpi_init_thread_(&required, &provided, &err);
    }
  pnmpi_init_done = 1;


  // app_startup();
}
