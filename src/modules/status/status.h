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

#ifndef _PNMPI_MOD_STATUS
#define _PNMPI_MOD_STATUS


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

#define PNMPI_MODULE_STATUS "status-storage"


/*..........................................................*/
/* Access macros */

#define STATUS_STORAGE(status, offset, type) \
  STATUS_STORAGE_ARRAY(status, offset, 0, type, 1, 0)

#define STATUS_STORAGE_ARRAY(status, offset, total, type, count, num)         \
  (*((type *)(((char *)(status)) + count * sizeof(MPI_Status) + num * total + \
              offset)))

#define PNMPIMOD_STATUS_TAG 0x3e1f9


/*..........................................................*/
/* Request additional memory in the status object
   This must be called from within MPI_Init (after
   calling PMPI_Init)

   IN:  size = number of bytes requested
               (if 0, just storage of standard parameters
   OUT: >=0: offset of new storage relative to request pointer
         <0: error message
*/

typedef int (*PNMPIMOD_Status_RequestStorage_t)(int);
int PNMPIMOD_Status_RequestStorage(int size);


#endif /* _PNMPI_MOD_STATUS */
