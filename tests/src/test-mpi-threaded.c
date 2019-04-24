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
  /* Initialize and finalize MPI. This should be enough to call all PnMPI setup
   * routines for threading support except the wrapper functions.
   *
   * The highest possible MPI threading level will be requested, so a possible
   * limiting of the threading level by PnMPI may be checked. */
  int provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  MPI_Finalize();

  printf("Required: %d Provided: %d\n", MPI_THREAD_MULTIPLE, provided);

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
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @BINARY@ @MPIEXEC_POSTFLAGS@
 * PASS: Required: [0-9]+ Provided: [0-9]+
 */
