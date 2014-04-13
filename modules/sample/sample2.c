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

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int MPI_Send(void *buf, int num, MPI_Datatype dtype, int node, int tag,
             MPI_Comm comm)
{
  int res;

  printf("WRAPPER 2: Before send\n");
  fflush(stdout);

  res = PMPI_Send(buf, num, dtype, node, tag, comm);

  printf("WRAPPER 2: After send\n");
  fflush(stdout);

  return res;
}
