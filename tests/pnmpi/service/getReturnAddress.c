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

/* This test case checks, if the module is able to get its own module ID. The
 * module will be loaded multiple times to check that the module ID is different
 * for the different loaded modules. */

#include <stdio.h>

#include <mpi.h>
#include <pnmpi/const.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/service.h>
#include <pnmpi/xmpi.h>


/** \brief Print the return address of the wrapped call.
 */
static void print_return_address()
{
  void *ptr;
  if (PNMPI_Service_GetReturnAddress(&ptr) != PNMPI_SUCCESS)
    PNMPI_Error("Can't get return address.\n");

  printf("Return address: %p\n", ptr);
}


#ifndef TESTCASE_SEND

int MPI_Init(int *argc, char ***argv)
{
  print_return_address();
  return XMPI_Init(argc, argv);
}

#elif TESTCASE_SEND

int MPI_Send(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
             int tag, MPI_Comm comm)
{
  print_return_address();
  return XMPI_Send(buf, count, datatype, dest, tag, comm);
}

#endif


/* CONFIGS: C_Init C_Send
 * CONFIGS: Fortran_Init Fortran_Send
 *
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@
 *
 *
 * COMPILE_INCLUDES: @MPI_C_INCLUDE_PATH@
 * COMPILE_FLAGS: @MPI_C_COMPILE_FLAGS@
 *
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 *
 *
 * RUN-C_Init: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN-C_Init:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 * COMPILE_FLAGS-C_Send: @MPI_C_COMPILE_FLAGS@ -DTESTCASE_SEND
 * RUN-C_Send: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 2
 * RUN-C_Send:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C_SENDRECV@ @MPIEXEC_POSTFLAGS@
 *
 *
 * RUN-Fortran_Init: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN-Fortran_Init:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 * COMPILE_FLAGS-Fortran_Send: @MPI_C_COMPILE_FLAGS@ -DTESTCASE_SEND
 * RUN-Fortran_Send: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 2
 * RUN-Fortran_Send:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_FORTRAN_SENDRECV@
 * RUN-Fortran_Send:   @MPIEXEC_POSTFLAGS@
 *
 *
 * PASS: Return address: 0x[0-f]+
 * FAIL: Return address: 0x0
 */
