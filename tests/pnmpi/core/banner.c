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

/* The following test cases will be used to check the PnMPI banner for
 * correctness. The banner includes some basic information about the loaded
 * configuration. */

#include <pnmpi/hooks.h>


void PNMPI_RegistrationPoint()
{
}


/* CONFIGS: mpi_interface no_module one_module stack
 *
 * MODTYPE: XMPI
 *
 * ENVIRONMENT: PNMPI_LIB_PATH=@CMAKE_CURRENT_BINARY_DIR@
 * ENVIRONMENT: PNMPI_CONF=@PNMPICONF@
 * RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
 * RUN:   @MPIEXEC_PREFLAGS@ @TESTBIN_MPI_C@ @MPIEXEC_POSTFLAGS@
 *
 *
 * ENVIRONMENT-mpi_interface:
 * PASS-mpi_interface: MPI interface: .*
 *
 * ENVIRONMENT-no_module:
 * PASS-no_module: No modules loaded
 *
 * PNMPICONF-one_module: module @MODNAME@
 * PASS-one_module: Loaded modules:.*Stack default:.*@MODNAME@ \(Pcontrol: 1\)
 *
 * PNMPICONF-stack: module @MODNAME@\n
 * PNMPICONF-stack: stack sample\n
 * PNMPICONF-stack: module @MODNAME@\n
 * PASS-stack: Loaded modules:.*Stack default:.*Stack sample
 */
