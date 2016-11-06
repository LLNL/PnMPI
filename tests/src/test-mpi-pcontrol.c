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

/* This test file will simply pass the own rank in a circle to the next rank and
 * can be used to check the common MPI functions Init, Finalize, Send and Recv
 * in combination with PnMPI. */

#include <stdio.h>  // fprintf, printf
#include <stdlib.h> // EXIT_* macros

#include <mpi.h>


/** \brief Print the error code that \p function returned and exit.
 *
 *
 * \param function Function name.
 */
#define print_err(function)                                    \
  {                                                            \
    fprintf(stderr, function " returned an error: %i\n", err); \
    return EXIT_FAILURE;                                       \
  }


int main(int argc, char **argv)
{
  int err;


  /* Initalize MPI. */
  if ((err = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    print_err("MPI_Init");


  /* Call Pcontrol for testing Pcontrol functionality. */
  MPI_Pcontrol(0);


  /* Finalize MPI. */
  if ((err = MPI_Finalize()) != MPI_SUCCESS)
    print_err("MPI_Finalize");


  /* In standard C the following return is not required, but in some situations
   * older versions of mpiexec report the job aborted, so the test case will
   * fail, even if it succeed. Returning EXIT_SUCCESS avoids this false error
   * message. */
  return EXIT_SUCCESS;
}
