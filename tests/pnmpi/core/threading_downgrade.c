/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

/* This test case checks, if PnMPI limits the threading level, if at least one
 * module does not support the full threading level of MPI. */

#include <mpi.h>


int PnMPI_threading_level = MPI_THREAD_SINGLE;


/* MODTYPE: XMPI
 * COMPILE_INCLUDES: @MPI_C_INCLUDE_PATH@
 * COMPILE_FLAGS: @MPI_C_COMPILE_FLAGS@
 *
 * PNMPICONF: module @MODNAME@
 *
 * RUN: @PNMPIZE@ -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@
 * RUN:   @TESTBIN_MPI_C_THREADED@
 * PASS: provide a maximum of 0
 */
