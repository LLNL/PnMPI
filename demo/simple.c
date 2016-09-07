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
