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

#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


void PNMPI_RegistrationPoint()
{
  int handle;
  if (PNMPI_Service_GetModuleSelf(&handle) == PNMPI_SUCCESS)
    printf("My module ID: %d\n", handle);
}


/* MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: module @MODNAME@\n
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 * PASS: 0.*1.*2
 */
