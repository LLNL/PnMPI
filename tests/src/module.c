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

#include <pnmpi-config.h>


/* MPI3 implements const correctness: it will use const for parameters were it
 * is possible. Instead of writing ifdefs arround each affected function, we
 * will define a macro which will be 'const' for MPI3 and empty otherwise, which
 * will be used instead of the const keyword in the function declarations. */
#ifdef HAVE_MPI3_CONST_ARGS
#define MPI3_CONST const
#else
#define MPI3_CONST
#endif


/* The folloing wrappers for MPI_Send and MPI_Recv will print a little debug
 * message to stdout and then execute the original MPI call. This module has no
 * specific usecase but will be used to test the functionality of PnMPI in the
 * defined test cases. */


int MPI_Send(MPI3_CONST void *buf, int count, MPI_Datatype datatype, int dest,
             int tag, MPI_Comm comm)
{
  int rank, res;
  if ((res = PMPI_Comm_rank(comm, &rank)) != MPI_SUCCESS)
    {
      fprintf(stderr, "Wrapper: Error in PMPI_Comm_rank: %d.\n", res);
      exit(EXIT_FAILURE);
    }

  printf("Wrapper: Sending %d elements from rank %d to %d (tag: %d).\n", count,
         rank, dest, tag);


  return PMPI_Send(buf, count, datatype, dest, tag, comm);
}


int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
             MPI_Comm comm, MPI_Status *status)
{
  int res = PMPI_Recv(buf, count, datatype, source, tag, comm, status);


  int rank, res2;
  if ((res2 = PMPI_Comm_rank(comm, &rank)) != MPI_SUCCESS)
    {
      fprintf(stderr, "Wrapper: Error in PMPI_Comm_rank: %d.\n", res2);
      exit(EXIT_FAILURE);
    }

  printf("Wrapper: Rank %d received %d elements from rank %d (tag: %d).\n",
         rank, count, source, tag);


  return res;
}
