/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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

#include <stdlib.h> // EXIT_* macros

#include "mpi_errors.h"


int main(int argc, char **argv)
{
  /* Initalize and finalize MPI. This should be enough to call all PnMPI setup
   * routines except the wrapper functions. */
  MPI_Init(&argc, &argv);
  MPI_Finalize();

  /* In standard C the following return is not required, but in some situations
   * older versions of mpiexec report the job aborted, so the test case will
   * fail, even if it succeed. Returning EXIT_SUCCESS avoids this false error
   * message. */
  return EXIT_SUCCESS;
}


/* CONFIGS: preload dynamic static
 * DEPENDS-dynamic: testbin-mpi-wrapper pnmpi
 *
 * COMPILE_INCLUDES: @CMAKE_CURRENT_BINARY_DIR@ @MPI_C_INCLUDE_PATH@
 * COMPILE_FLAGS: @MPI_C_COMPILE_FLAGS@
 * LINK_FLAGS: @MPI_C_LINK_FLAGS@
 * LINK: @MPI_C_LIBRARIES@
 *
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @BINARY@ @MPIEXEC_POSTFLAGS@
 * PASS: No modules loaded.
 *
 *
 * ENVIRONMENT-preload: LD_PRELOAD=@LIBPNMPI@
 * ENVIRONMENT-preload: DYLD_INSERT_LIBRARIES=@LIBPNMPI@
 * ENVIRONMENT-preload: DYLD_FORCE_FLAT_NAMESPACE=1
 *
 * LINK-dynamic: pnmpi @MPI_C_LIBRARIES@
 *
 * DEPENDS-static: testbin-mpi-wrapper pnmpi_static
 * LINK-static: pnmpi_static @MPI_C_LIBRARIES@ dl m
 */
