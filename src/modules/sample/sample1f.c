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

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>


void mpi_send_(void *buf, int *num, MPI_Datatype *dtype, int *node, int *tag,
               MPI_Comm *comm, int *ierr)
{
  printf("WRAPPER 1: Before send\n");
  fflush(stdout);

  *ierr = PMPI_Send(buf, *num, *dtype, *node, *tag, *comm);

  printf("WRAPPER 1: After send\n");
  fflush(stdout);

  return;
}

void mpi_recv_(void *buf, int *num, MPI_Datatype *dtype, int *node, int *tag,
               MPI_Comm *comm, MPI_Status *status, int *ierr)
{
  printf("WRAPPER 1: Before recv\n");
  fflush(stdout);

  *ierr = PMPI_Recv(buf, *num, *dtype, *node, *tag, *comm, status);

  printf("WRAPPER 1: After recv\n");
  fflush(stdout);

  return;
}
