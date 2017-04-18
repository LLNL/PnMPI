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

/* This test case checks the app_shook functionality.
 *
 * Note: As PNMPI_AppStartup does initialize MPI, mpiexec must be used to not
 *       get OpenMPI into a deadlock.
 */

#include <stdio.h>

#include <mpi.h>
#include <pnmpi/hooks.h>


#ifdef WITH_STARTUP
void PNMPI_AppStartup()
{
  int status;
  PMPI_Initialized(&status);

  printf("startup hook: MPI %s\n", status ? "initialized" : "NOT initialized");
}
#endif


#ifdef WITH_SHUTDOWN
void PNMPI_AppShutdown()
{
  int status;
  PMPI_Initialized(&status);

  printf("shutdown hook: MPI %s\n", status ? "initialized" : "NOT initialized");
}
#endif


/* CONFIGS: no_hook startup shutdown combined threading_level_env
 * CONFIGS: combined-static
 *
 * MODTYPE: XMPI
 * COMPILE_INCLUDES: @MPI_C_INCLUDE_PATH@
 * COMPILE_FLAGS: @MPI_C_COMPILE_FLAGS@
 *
 * PNMPICONF: module @MODNAME@
 *
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @PNMPIZE@ @MPIEXEC_POSTFLAGS@
 * RUN:   -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@ -q @TESTBIN@
 *
 * FAIL-no_hook: hook
 *
 * COMPILE_FLAGS-startup: @MPI_C_COMPILE_FLAGS@ -DWITH_STARTUP
 * PASS-startup: startup hook: MPI initialized.
 *
 * COMPILE_FLAGS-shutdown: @MPI_C_COMPILE_FLAGS@ -DWITH_SHUTDOWN
 * PASS-shutdown: shutdown hook: MPI initialized.
 *
 * COMPILE_FLAGS-combined: @MPI_C_COMPILE_FLAGS@ -DWITH_STARTUP -DWITH_SHUTDOWN
 * PASS-combined: startup hook: MPI initialized.*shutdown hook: MPI initialized.
 *
 * COMPILE_FLAGS-combined-static: @MPI_C_COMPILE_FLAGS@
 * COMPILE_FLAGS-combined-static:   -DWITH_STARTUP -DWITH_SHUTDOWN
 * ENVIRONMENT-combined-static: PNMPI_CONF=@PNMPICONF@
 * ENVIRONMENT-combined-static: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT-combined-static: PNMPI_BE_SILENT=1
 * ENVIRONMENT-combined-static: PNMPI_INTERFACE=C
 * RUN-combined-static: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN-combined-static:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C_STATIC@
 * RUN-combined-static:   @MPIEXEC_POSTFLAGS@
 * PASS-combined-static: startup hook:.*shutdown hook:
 *
 * COMPILE_FLAGS-threading_level_env: @MPI_C_COMPILE_FLAGS@ -DWITH_STARTUP
 * ENVIRONMENT-threading_level_env: PNMPI_THREADING_LEVEL=0
 * PASS-threading_level_env: Provided:.*0
 */
