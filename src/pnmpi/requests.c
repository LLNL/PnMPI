/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2018 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2018 RWTH Aachen University, Federal Republic of Germany
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

#include "pnmpimod.h"

/*------------------------------------------------------------*/
/* Declarations */

/*..........................................................*/
/* Size of additional storage across all modules */

int pnmpi_req_storage_size = -1;


/*------------------------------------------------------------*/
/* Public Interface */

/*..........................................................*/
/* Request additional memory in the request object
   This must be called from within MPI_Init

   IN:  size = number of bytes requested
               (if 0, just storage of standard parameters
   OUT: >=0: offset of new storage relative to request pointer
         <0: error message
*/

/* No need to be threadsafe here, since there can only be
   one activation of MPI_Init anyway */

int PNMPI_Service_RequestStorage(int size)
{
  int ret;

  if (pnmpi_init_running)
    {
      if (pnmpi_req_storage < 0)
        {
          /* first request, activate it */
          pnmpi_req_storage = 0;
        }

      ret = sizeof(PNMPI_Request_Map) + pnmpi_req_storage;
      pnmpi_req_storage += size;
      return ret;
    }
  else
    {
      /* can only be called from within MPI_Init */
      return PNMPI_ERROR_NOT_INIT;
    }
}


/*------------------------------------------------------------*/
/* Internal Interface */


/*------------------------------------------------------------*/
/* The End. */
