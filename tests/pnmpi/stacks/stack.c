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

/* This is a simple test case to check the stack switching functionality. It
 * will be called in the default stack and tries to switch to a different stack.
 */

#include <stdio.h>

#include <mpi.h>
#include <pnmpi/newstack.h>
#include <pnmpi/service.h>
#include <pnmpi/xmpi.h>


int MPI_Init(int *argc, char ***argv)
{
  static int flag = 0;

  PNMPI_modHandle_t stack;
  if (flag == 0 &&
      PNMPI_Service_GetStackByName("sample", &stack) == PNMPI_SUCCESS)
    {
      printf("Switching the stack.\n");
      flag = 1;
      XMPI_Init_NewStack(stack, argc, argv);
    }
  else
    XMPI_Init(argc, argv);

  return MPI_SUCCESS;
}


/* MODTYPE: XMPI
 * COMPILE_INCLUDES: @MPI_C_INCLUDE_PATH@ @PROJECT_SOURCE_DIR@/src/pnmpi
 * COMPILE_INCLUDES:   @PROJECT_BINARY_DIR@ @PROJECT_BINARY_DIR@/src/pnmpi
 * COMPILE_FLAGS: @MPI_C_COMPILE_FLAGS@
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: stack sample\n
 * PNMPICONF: module @MODNAME@
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 * PASS: Switching the stack.
 */
