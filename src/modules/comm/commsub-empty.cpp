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

#include "commsub.h"

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/const.h>


void COMM_ALL_INIT(int argc, char **argv)
{
}

void COMM_ALL_PREINIT(int argc, char **argv)
{
}

void COMM_ALL_FINALIZE()
{
}

void SEND_P2P_START(PNMPI_CONST void *buf, int count, MPI_Datatype dt, int node,
                    int tag, MPI_Comm comm, void **ptr, int type)
{
}

void SEND_P2P_ASYNC_MID1(PNMPI_CONST void *buf, int count, MPI_Datatype dt,
                         int node, int tag, MPI_Comm comm, void **ptr, int type)
{
}

void SEND_P2P_END(PNMPI_CONST void *buf, int count, MPI_Datatype dt, int node,
                  int tag, MPI_Comm comm, int err, void **ptr, void **midptr,
                  int type)
{
}

void RECV_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag,
                    MPI_Comm comm, void **ptr, int type)
{
}

void RECV_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node,
                         int tag, MPI_Comm comm, int err, void **ptr, int type)
{
}

void RECV_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag,
                  MPI_Comm comm, int err, void **ptr, void **midptr, int type,
                  MPI_Status *statusarray, int numindex, int index)
{
}

void COMM_P2P_ASYNC_MID2(int count, MPI_Request *requests, int flag,
                         void **midptr)
{
}

void COMM_P2P_ASYNC_COMPLETION(int flag)
{
}

void COMM_COLL_REDUCE(void *buf, int count, MPI_Datatype dt, MPI_Op ops,
                      int size, void **ptr)
{
}

void COMM_COLL_START(MPI_Comm comm, int root, int type, void **ptr)
{
}

void COMM_COLL_END(MPI_Comm comm, int root, int type, void **ptr)
{
}
