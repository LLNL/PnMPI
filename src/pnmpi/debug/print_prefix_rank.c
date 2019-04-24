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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/print.h>


/** \brief Return the size needed to represent the highest rank.
 *
 * \details This function returns the number of characters required to represent
 *  the highest available rank, so the message output may be aligned.
 *
 *
 * \return The number of characters required.
 * \return 0 MPI is not initialized yet.
 */
static int pnmpi_get_size_width(void)
{
  static int width = -1;

  /* If the width was cached by a call before, return the cached result instead
   * of making a new calculation. */
  if (width != -1)
    return width;


  /* Check if MPI is initialized yet. Otherwise, we can't lookup the size of
   * MPI_COMM_WORLD to calculate its width. */
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
  if (initialized)
    {
      int size;
      if (PMPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        {
          /* pnmpi_error can't be used here, because this would result in an
           * endless loop: pnmpi_error is a wrapper of pnmpi_warning which uses
           * this function, so the PMPI_Comm_size error would occur again and
           * again until the stack is full. */
          fprintf(stderr, "%s:%d: PMPI_Comm_size failed\n", __FUNCTION__,
                  __LINE__);
          exit(EXIT_FAILURE);
        }

      /* As the rank starts counting at zero, we have to decrement one. This has
       * the side effect, that for applications running with a single rank, no
       * rank prefix will be printed. */
      size = size - 1;

      return (width = (size > 0) ? (int)floor(log10(abs(size))) + 1 : 0);
    }
  else
    return 0;
}


/** \brief Print \p format with prefixed rank number.
 *
 * \details This function prints \p format with the arguments \p ap, but
 *  prefixes the lines with the rank of this process.
 *
 *
 * \param stream The stream to print to.
 * \param format Printf-like format string.
 * \param ap Variadic arguments.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_print_prefix_rank(FILE *stream, const char *format, va_list ap)
{
  assert(format);
  assert(ap);


  int width = pnmpi_get_size_width();

  /* If MPI is not initialized yet, we can't print the rank of this process, so
   * the message can be printed as it is to the stream. */
  if (width == 0)
    vfprintf(stream, format, ap);
  else
    {
      int rank = pnmpi_get_rank();

      /* MPI is initialized, so we have to generate a new format string with the
       * current rank was prefix. */
      size_t len = width + strlen(format) + 3;
      char buffer[len];
      if (snprintf(buffer, len, "%*d: %s", width, rank, format) >= len)
        {
          /* pnmpi_error can't be used here, because this would result in an
           * endless loop: pnmpi_error is a wrapper of pnmpi_warning which uses
           * this function, so the PMPI_Comm_size error would occur again and
           * again until the stack is full. */
          fprintf(stderr, "%s:%d: Not enough memory for snprintf.\n",
                  __FUNCTION__, __LINE__);
          exit(EXIT_FAILURE);
        }

      /* Print the format string. */
      vprintf(buffer, ap);
    }

  fflush(stream);
}
