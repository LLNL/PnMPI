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

/* This test case checks, if stacks can be found by name. There will be tests
 * for a valid and an invalid name in this test file. */

#include <stdio.h>
#include <stdlib.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


#ifndef TEST_STACKNAME
#define TEST_STACKNAME "sample"
#endif


void PNMPI_RegistrationPoint()
{
  /* Search for the module and print the return code or the matching module. */
  PNMPI_modHandle_t stack;
  int ret = PNMPI_Service_GetStackByName(TEST_STACKNAME, &stack);
  switch (ret)
    {
    case PNMPI_SUCCESS: printf("GetStackByName: %d\n", stack); break;
    case PNMPI_NOSTACK:
      PNMPI_Warning("GetStackByName: %s\n", PNMPI_Service_strerror(ret));
      break;

    default: PNMPI_Error("Unknown error: %d\n", ret); break;
    }
}


/* CONFIGS: found not_found
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: stack sample\n
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 *
 * PASS-found: GetStackByName: [0-9]+
 *
 * COMPILE_FLAGS-not_found: -DTEST_STACKNAME=\"foo\"
 * PASS-not_found: GetStackByName: Stack not found
 */
