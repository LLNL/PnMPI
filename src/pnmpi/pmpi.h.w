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

/* This wrapper will be used for PMPI functions, as some MPI implementations
 * define not all PMPI- for all available MPI-functions. */

#ifndef PNMPI_PRIVATE_PMPI_H
#define PNMPI_PRIVATE_PMPI_H


#include <mpi.h>
#include <pnmpi/private/config.h>


{{forallfn fn_name}}
{{ret_type}} P{{fn_name}}({{formals}});
{{endforallfn}}


/* If PnMPI will be compiled for the Fortran MPI interface, definitions for the
 * used Fortran PMPI functions are also required. */
#ifdef ENABLE_FORTRAN

void pmpi_init_(int *ierror);

#ifdef HAVE_MPI_INIT_THREAD_Fortran
void pmpi_init_thread_(int *required, int *provided, int *ierror);
#endif

void pmpi_finalize_(int *ierror);

#endif


#endif
