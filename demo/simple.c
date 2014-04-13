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

int main(int argc, char **argv)
{
  int rank, size, i, err;
  int buf[5] = { 0, 0, 0, 0, 0 };
  MPI_Status status;

  err = MPI_Init(&argc, &argv);
  if (err != MPI_SUCCESS)
    {
      printf("ERROR IN INIT: %i\n", err);
      exit(1);
    }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
    {
      for (i = 1; i < size; i++)
        {
          MPI_Recv(buf, 5, MPI_INT, MPI_ANY_SOURCE, 42, MPI_COMM_WORLD,
                   &status);
          MPI_Send(buf, 5, MPI_INT, status.MPI_SOURCE, 42, MPI_COMM_WORLD);
          printf("GOT %i\n", status.MPI_SOURCE);
        }
    }
  else
    {
      MPI_Send(buf, 5, MPI_INT, 0, 42, MPI_COMM_WORLD);
      MPI_Recv(buf, 5, MPI_INT, MPI_ANY_SOURCE, 42, MPI_COMM_WORLD, &status);
    }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
