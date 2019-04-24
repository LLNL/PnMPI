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

#ifndef _PNMPI_MOD_TIMELAPSE
#define _PNMPI_MOD_TIMELAPSE


#include <mpi.h>


/*------------------------------------------------------------*/
/* Note: this module must be loaded into the stack before
   any module using it (i.e., must be instantiated between
   any user and the application). Also, any call to its
   services must occur after calling PMPI_Init and services
   are only available to modules requesting it during
   MPI_Init */

/*------------------------------------------------------------*/
/* Additional storage in requests */

#define PNMPI_MODULE_TIMELAPSE "timelapse"


/*..........................................................*/
/* Start at stop timer */

typedef int (*PNMPIMOD_Timelapse_Switch_t)(int);
int PNMPIMOD_Timelapse_Switch(int onoff);


#endif /* _PNMPI_MOD_STATUS */
