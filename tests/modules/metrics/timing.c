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

#include <mpi_errors.h>


int main(int argc, char **argv)
{
  /* Note: MPI_Pcontrol is not wrapped by mpi_errors.h, but as the MPI standard
   * does not say something about the value to be returned, it does not have to
   * be checked. */

  MPI_Init(&argc, &argv);
#ifdef WITH_PCONTROL
  MPI_Pcontrol(0);
#endif
  MPI_Finalize();

  /* In standard C the following return is not required, but in some situations
   * older versions of mpiexec report the job aborted, so the test case will
   * fail, even if it succeed. Returning EXIT_SUCCESS avoids this false error
   * message. */
  return EXIT_SUCCESS;
}



/* CONFIGS: basic pcontrol_invalid pcontrol
 *
 * DEPENDS: testbin-mpi-wrapper
 * COMPILE_INCLUDES: @CMAKE_CURRENT_BINARY_DIR@/../../src @MPI_C_INCLUDE_PATH@
 * LINK: @MPI_C_LIBRARIES@
 * LINK_FLAGS: @MPI_C_LINK_FLAGS@
 *
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @BINARY@ @MPIEXEC_POSTFLAGS@
 *
 * COMPILE_FLAGS-basic: @MPI_C_COMPILE_FLAGS@
 * PNMPICONF-basic: module metrics-timing
 * PASS-basic: Timing stats:\n\n Rank 0:.*MPI_Init.*Total:.*MPI_Init
 *
 * COMPILE_FLAGS-pcontrol_invalid: @MPI_C_COMPILE_FLAGS@ -DWITH_PCONTROL
 * PNMPICONF-pcontrol_invalid: module metrics-timing
 * PASS-pcontrol_invalid: advanced mode
 *
 * COMPILE_FLAGS-pcontrol: @MPI_C_COMPILE_FLAGS@ -DWITH_PCONTROL
 * PNMPICONF-pcontrol: module metrics-timing\n
 * PNMPICONF-pcontrol: pcontrol on\n
 * PNMPICONF-pcontrol: module empty\n
 * PNMPICONF-pcontrol: pcontrol on\n
 * PNMPICONF-pcontrol: module metrics-timing\n
 * PNMPICONF-pcontrol: pcontrol on
 * PASS-pcontrol: Timing stats:\n\n Rank 0:.*MPI_Pcontrol.*Total:.*MPI_Pcontrol
 */
