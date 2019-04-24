/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2019 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2019 RWTH Aachen University, Federal Republic of Germany
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

#include <stdio.h>  // fprintf, printf
#include <stdlib.h> // EXIT_* macros

#include "mpi_errors.h"


int main(int argc, char **argv)
{
  /* Initalize MPI. */
  MPI_Init(&argc, &argv);


  /* Get MPI_COMM_WORLD size and current rank and calculate the target for
   * MPI_Send. The size will be checked, too, thus we need at least two ranks
   * for MPI send / recv. */
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size < 2)
    {
      fprintf(stderr, "At least 2 ranks are required for this test.\n");
      MPI_Finalize();
      return EXIT_FAILURE;
    }


  /* All ranks send their rank to rank 0 which then answers the sending rank. */
  int buffer;
  MPI_Status status;

  if (rank == 0)
    {
      int i, buffer;
      for (i = 1; i < size; i++)
        {
          MPI_Recv(&buffer, 1, MPI_INT, i, 42, MPI_COMM_WORLD, &status);
          printf("Got %d from rank %d.\n", buffer, status.MPI_SOURCE);
          MPI_Send(&buffer, 1, MPI_INT, i, 42, MPI_COMM_WORLD);
        }
    }
  else
    {
      MPI_Send(&rank, 1, MPI_INT, 0, 42, MPI_COMM_WORLD);
      MPI_Recv(&buffer, 1, MPI_INT, 0, 42, MPI_COMM_WORLD, &status);
      printf("Got %d from rank %d.\n", buffer, status.MPI_SOURCE);
    }

  /* Finalize MPI. */
  MPI_Finalize();


  /* In standard C the following return is not required, but in some situations
   * older versions of mpiexec report the job aborted, so the test case will
   * fail, even if it succeed. Returning EXIT_SUCCESS avoids this false error
   * message. */
  return EXIT_SUCCESS;
}


/* DEPENDS: testbin-mpi-wrapper
 * COMPILE_INCLUDES: @CMAKE_CURRENT_BINARY_DIR@ @MPI_C_INCLUDE_PATH@
 * COMPILE_FLAGS: @MPI_C_COMPILE_FLAGS@
 * LINK: pnmpi @MPI_C_LIBRARIES@
 * LINK_FLAGS: @MPI_C_LINK_FLAGS@
 *
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 2
 * RUN:     @MPIEXEC_PREFLAGS@ @BINARY@ @MPIEXEC_POSTFLAGS@
 * RUN:   | sort -n
 *
 * PASS: Got 1 from rank 0.\nGot 1 from rank 1.
 */
