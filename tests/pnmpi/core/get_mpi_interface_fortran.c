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

/* The following test cases will be used to check if PnMPI detects the language
 * of the instrumented application right.
 *
 * The AppStartup and AppShutdown hooks will be used to initialize and finalize
 * MPI, so this test case can be used with non-mpi applications, too. */

#include <pnmpi/hooks.h>


void PNMPI_AppStartup()
{
}

void PNMPI_AppShutdown()
{
}


/* CONFIGS: env nm
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@
 *
 * RUN-env: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN-env:   @MPIEXEC_PREFLAGS@ @PNMPIZE@ @MPIEXEC_POSTFLAGS@
 * RUN-env:   -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@ @TESTBIN_NOMPI@
 * ENVIRONMENT-env: PNMPI_INTERFACE=Fortran
 * PASS-env: MPI interface: Fortran
 *
 * RUN-nm: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN-nm:   @MPIEXEC_PREFLAGS@ @PNMPIZE@ @MPIEXEC_POSTFLAGS@
 * RUN-nm:   -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@
 * RUN-nm:   @TESTBIN_MPI_FORTRAN@
 * PASS-nm: MPI interface: Fortran
 */
