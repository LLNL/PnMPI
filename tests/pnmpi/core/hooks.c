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

/* This test case is used to check if PnMPI hits all hooks in modules.
 *
 * Note: This test cases check NOT, if the functionality of the specific hooks
 *       is correct, but only if they are called!
 *
 * Note: As PNMPI_AppStartup does initialize MPI, mpiexec must be used to not
 *       get OpenMPI into a deadlock.
 */

#include <stdio.h> // printf

#include <pnmpi/hooks.h> // PnMPI header to check if the hooks signatures match.


#ifndef HOOKNAME
#define HOOKNAME unknown
#endif


void HOOKNAME()
{
  printf("%s hit.\n", __FUNCTION__);
}


/* CONFIGS: RegistrationPoint PNMPI_AppStartup PNMPI_AppShutdown
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@
 *
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @PNMPIZE@ @MPIEXEC_POSTFLAGS@
 * RUN:   -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@ @TESTBIN_MPI_C@
 *
 *
 * COMPILE_FLAGS-RegistrationPoint: -DHOOKNAME=PNMPI_RegistrationPoint
 * PASS-RegistrationPoint: PNMPI_RegistrationPoint hit.
 *
 * COMPILE_FLAGS-PNMPI_AppStartup: -DHOOKNAME=PNMPI_AppStartup
 * PASS-PNMPI_AppStartup: PNMPI_AppStartup hit.
 *
 * COMPILE_FLAGS-PNMPI_AppShutdown: -DHOOKNAME=PNMPI_AppShutdown
 * PASS-PNMPI_AppShutdown: PNMPI_AppShutdown hit.
 */
