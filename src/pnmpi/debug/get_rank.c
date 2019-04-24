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

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/print.h>


/** \brief Return the rank of the process.
 *
 *
 * \return The rank of the process.
 * \return -1 MPI is not initialized yet.
 *
 *
 * \private
 */
PNMPI_INTERNAL
int pnmpi_get_rank(void)
{
  /* Use the following variable as cache for the rank of this process. If the
   * rank was cached by a call before, return the cached result instead of
   * making a new lookup. */
  static int rank = -1;
  if (rank != -1)
    return rank;


  /* If MPI was not initialized until now, we can't lookup the rank. */
  int initialized;
  if (PMPI_Initialized(&initialized) != MPI_SUCCESS)
    {
      /* pnmpi_error can't be used here, because this would result in an endless
       * loop: pnmpi_error is a wrapper of pnmpi_warning which uses this
       * function, so the PMPI_Initialized error would occur again and again
       * until the stack is full. */
      fprintf(stderr, "%s:%d: PMPI_Initialized failed\n", __FUNCTION__,
              __LINE__);
      exit(EXIT_FAILURE);
    }
  if (!initialized)
    return -1;

  /* If PnMPI is initialized get the rank of this process. If this fails,
   * print an error message and exit the application. */
  if (PMPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
    {
      /* pnmpi_error can't be used here, because this would result in an
       * endless loop: pnmpi_error is a wrapper of pnmpi_warning which uses
       * this function, so the PMPI_Comm_rank error would occur again and
       * again until the stack is full. */
      fprintf(stderr, "%s:%d: PMPI_Comm_rank failed\n", __FUNCTION__, __LINE__);
      exit(EXIT_FAILURE);
    }

  return rank;
}
