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

/* This test case checks, if a registered global may be queried by an other
 * module. There will be a valid test and two failure-tests for unknown name and
 * not-matching signature in this test file. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


#define global_name "test"
#define global_sig 'd'

#ifndef TEST_GLOBALNAME
#define TEST_GLOBALNAME global_name
#endif

#ifndef TEST_GLOBALSIG
#define TEST_GLOBALSIG global_sig
#endif

#ifndef TEST_MODULE
#define TEST_MODULE 0
#endif


int global_int = 42;


void registerGlobal()
{
  PNMPI_Global_descriptor_t global;
  global.addr.i = &global_int;
  strncpy(global.name, global_name, PNMPI_SERVICE_NAMELEN);
  global.sig = global_sig;

  int ret = PNMPI_Service_RegisterGlobal(&global);
  if (ret != PNMPI_SUCCESS)
    PNMPI_Error("Error: %d\n", ret);
}


void queryGlobal()
{
  /* Query the global and print a message with the return code. */
  PNMPI_Global_descriptor_t buffer;
  int ret = PNMPI_Service_GetGlobalByName(TEST_MODULE, TEST_GLOBALNAME,
                                          TEST_GLOBALSIG, &buffer);
  switch (ret)
    {
    case PNMPI_SUCCESS:
      printf("getGlobalByName: %d\n", *(buffer.addr.i));
      break;
    case PNMPI_NOMODULE:
    case PNMPI_NOGLOBAL:
    case PNMPI_SIGNATURE:
      PNMPI_Warning("getGlobalByName: %s\n", PNMPI_Service_strerror(ret));
      break;

    default: PNMPI_Error("Unknown error: %d\n", ret); break;
    }
}


void PNMPI_RegistrationPoint()
{
  /* The first loaded module will register the global, others try to access the
   * global. */
  int self;
  if (PNMPI_Service_GetModuleSelf(&self) != PNMPI_SUCCESS)
    PNMPI_Error("Can't get module ID.\n");
  if (self == 0)
    registerGlobal();
  else
    queryGlobal();
}


/* CONFIGS: found no_module no_global_self no_global_other no_signature
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: module @MODNAME@\n
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 *
 * PASS-found: getGlobalByName: 42
 *
 * COMPILE_FLAGS-no_module: -DTEST_MODULE=999
 * PASS: getGlobalByName: Module not found
 *
 * COMPILE_FLAGS-no_global_self: -DTEST_GLOBALNAME=\"foo\"
 * PASS-no_global_self: getGlobalByName: Global not found
 *
 * COMPILE_FLAGS-no_global_other: -DTEST_MODULE=1
 * PASS-no_global_other: getGlobalByName: Global not found
 *
 * COMPILE_FLAGS-no_signature: -DTEST_GLOBALSIG=\'p\'
 * PASS-no_signature: getGlobalByName: Signatures don't match
 */
