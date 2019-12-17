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

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/private/print.h>


/** \brief Abort the application.
 *
 * \details This function aborts the execution of the application by either
 *  calling `MPI_Abort()` in an active MPI environment or the regular `exit()`
 *  function in any other case.
 */
PNMPI_FUNCTION_NORETURN
static void pnmpi_exit()
{
  /* If the MPI runtime is initialized but not finalized yet, MPI_Abort must be
   * used to kill the whole application on all ranks. */
  int flag;
  if ((PMPI_Initialized(&flag) == MPI_SUCCESS) && flag)
    if ((PMPI_Finalized(&flag) == MPI_SUCCESS) && !flag)
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  /* If the conditions listed above are not met, just exit the application the
   * regular way. */
  exit(EXIT_FAILURE);
}


/** \brief Print a warning to stderr and exit the application.
 *
 * \details This function is a wrapper for \ref PNMPI_Warning and exit.
 *
 * \details This function will print a warning to stderr. The selected debug
 *  node will be ignored: warnings will be printed at all nodes if they occur.
 *
 *
 * \param prefix The value of \ref PNMPI_MESSAGE_PREFIX.
 * \param function Function where the error occurred.
 * \param line Line where the error occurred.
 * \param format Printf-like format string.
 * \param ... Arguments to be evaluated.
 *
 *
 * \hidecallergraph
 * \ingroup pnmpi_debug_io
 * \private
 */
void pnmpi_print_error(const char *prefix, const char *function, const int line,
                       const char *format, ...)
{
  assert(function);
  assert(format);


  /* Create a new buffer for a format containing the prefix and function and
   * line parameters. Separated prints after printing the format string can't be
   * used as the buffer will be flushed after calling pnmpi_print_prefix_rank,
   * so function and line may be attached to a different message from an other
   * rank. */
  size_t len = strlen(prefix) + strlen(function) + floor(log10(abs(line))) +
               strlen(format) + 8;
  char buffer[len];
  if (snprintf(buffer, len, "[%s] %s:%d: %s", prefix, function, line, format) >=
      len)
    {
      /* pnmpi_error can't be used here, because this would result in an
       * endless loop. */
      fprintf(stderr, "%s:%d: Not enough memory for snprintf.\n", __FUNCTION__,
              __LINE__);
      pnmpi_exit();
    }

  /* Print the new format string with the variadic arguments passed to the
   * function. */
  va_list ap;
  va_start(ap, format);
  pnmpi_print_prefix_rank(stderr, buffer, ap);
  va_end(ap);

  /* Exit the application with an error code. */
  pnmpi_exit();
}
