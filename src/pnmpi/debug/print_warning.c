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
#include <stdarg.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/private/print.h>


/** \brief Print a warning to stderr.
 *
 * \details This function will print a warning to stderr. The selected debug
 *  node will be ignored: warnings will be printed at all nodes if they occur.
 *
 *
 * \param format Printf-like format string.
 * \param ... Arguments to be evaluated.
 *
 *
 * \hidecallergraph
 * \ingroup pnmpi_debug_io
 * \private
 */
void pnmpi_print_warning(const char *format, ...)
{
  assert(format);


  /* Print the message with rank as prefix. */
  va_list ap;
  va_start(ap, format);
  pnmpi_print_prefix_rank(stderr, format, ap);
  va_end(ap);
}
