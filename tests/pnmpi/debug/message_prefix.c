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

#ifndef PRINT_DEFAULT
#define PNMPI_MESSAGE_PREFIX "testmodule"
#endif

#ifdef PNMPI_NO_DEBUG
#undef PNMPI_NO_DEBUG
#endif


#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>


void PNMPI_RegistrationPoint()
{
#ifdef PRINT_DEBUG
  PNMPI_Debug(PNMPI_DEBUG_MODULE, "debug\n");
#endif

#ifdef PRINT_WARNING
  PNMPI_Warning("warning\n");
#endif

#ifdef PRINT_ERROR
  PNMPI_Error("error\n");
#endif
}


/* CONFIGS: default debug warning error
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 *
 * COMPILE_FLAGS-default: -DPRINT_DEFAULT -DPRINT_WARNING
 * PASS-default: \[PnMPI\] warning
 *
 * COMPILE_FLAGS-debug: -DPRINT_DEBUG
 * ENVIRONMENT-debug: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT-debug: PNMPI_CONF=@PNMPICONF@
 * ENVIRONMENT-debug: PNMPI_DBGLEVEL=2
 * PASS-debug: \[testmodule\] debug
 *
 * COMPILE_FLAGS-warning: -DPRINT_WARNING
 * PASS-warning: \[testmodule\] warning
 *
 * COMPILE_FLAGS-error: -DPRINT_ERROR
 * PASS-error: \[testmodule\] .*:[0-9]+: error
 */
