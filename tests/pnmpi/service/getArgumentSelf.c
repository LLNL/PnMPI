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

/* This test case will check, if the module is able to query its own arguments.
 * There will be tests to check that only argument of the own module will be
 * found and that unknown module names will return an error. */

#include <stdio.h>
#include <stdlib.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


#ifndef TEST_ARGUMENT
#define TEST_ARGUMENT "foo"
#endif


void PNMPI_RegistrationPoint()
{
  /* Only the second module in the stack should execute this test. Other handles
   * will be used to store arguments only. */
  int self;
  if (PNMPI_Service_GetModuleSelf(&self) != PNMPI_SUCCESS)
    PNMPI_Error("Can't get module ID.\n");
  if (self != 1)
    return;

  const char *buffer = PNMPI_Service_GetArgumentSelf(TEST_ARGUMENT);
  if (buffer == NULL)
    PNMPI_Warning("GetArgument: not found\n");
  else
    printf("GetArgument: %s=%s\n", TEST_ARGUMENT, buffer);
}


/* CONFIGS: found found_second not_found
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: argument hello world\n
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: argument foo bar\n
 * PNMPICONF: argument bar foo\n
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 *
 * PASS-found: GetArgument: foo=bar
 *
 * COMPILE_FLAGS-found_second: -DTEST_ARGUMENT=\"bar\"
 * PASS-found_second: GetArgument: bar=foo
 *
 * COMPILE_FLAGS-not_found: -DTEST_ARGUMENT=\"hello\"
 * PASS-not_found: GetArgument: not found
 */
