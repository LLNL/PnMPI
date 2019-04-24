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
#include <stdlib.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/private/print.h>


/** \brief Check if \p level matches the debug level to print.
 *
 * \details This function checks, if \p level matches the debug level defined in
 *  the PNMPI_DBGLEVEL environment variable.
 *
 *
 * \param level The level of the message to print.
 *
 * \return 0 \p level does not match the debug level to print.
 * \return 1 \p level matches the debug level to print.
 */
static int pnmpi_check_dbglevel(const enum pnmpi_debug_level level)
{
  /* If not already looked up, get the contents of the PNMPI_DBGLEVEL
   * environment variable and store them in a static variable, so it may be
   * reused in all following calls to this function. */
  static int dbglevel = -1;
  if (dbglevel == -1)
    {
      const char *tmp = getenv("PNMPI_DBGLEVEL");
      dbglevel = (tmp != NULL) ? atoi(tmp) : 0;
    }

  return (dbglevel & level);
}


/** \brief Check if message should be printed at this rank.
 *
 * \details This function checks, if the current rank matches the rank defined
 *  in PNMPI_DBGNODE environment variable and will return a decision if the
 *  message should be printed at this rank.
 *
 * \note If the user did not define a node where to print the debug message,
 *  messages will be printed at any rank.
 *
 *
 * \return 0 The message should not be printed at this rank.
 * \return 1 The message should be printed  at this rank.
 */
static int pnmpi_check_dbgnode(void)
{
  static int print = -1;

  /* If the decision was made to print the messages on this node or not, return
   * the cached result of this decision. */
  if (print != -1)
    return print;


  /* There was no decision made yet. Lookup the environment variable and the
   * current rank to make a decision if messages should be printed or not at
   * this rank. */
  char *env_var = getenv("PNMPI_DBGNODE");
  if (env_var != NULL)
    {
      int print_node = atoi(env_var);
      if (print_node < 0)
        PNMPI_Error("PNMPI_DBGNODE must be a positive integer.\n");

      /* Check if print_node matches the rank of this process to make a decision
       * about to print the message or not. If MPI can't find a rank, because it
       * is not initialized yet, we can't make this decision and the message
       * will be printed at any rank.
       *
       * As this may result in a lot of debug messages at PnMPI initialization
       * time (depending on the selected debug level), it is a good idea to do
       * the PnMPI debugging before MPI initialization with only one rank. */
      int rank = pnmpi_get_rank();
      return ((rank == -1) || (print = (rank == print_node)));
    }

  /* No debug node was provided in the environment, so debug messages will be
   * printed at every node. */
  else
    return (print = 1);
}


/** \brief Print a debug message for \p level to stdout.
 *
 * \details This function checks if debug messages should be printed for \p
 *  level and at this node. If the requirements are met, the message will be
 *  printed to stdout.
 *
 *
 * \param level The debug level of the message.
 * \param format Printf-like format string.
 * \param ... Arguments to be evaluated.
 *
 *
 * \hidecallergraph
 * \ingroup pnmpi_debug_io
 * \private
 */
void pnmpi_print_debug(const enum pnmpi_debug_level level, const char *format,
                       ...)
{
  assert(format);


  /* Check if the requirements are met to print messages for the requested debug
   * level at this node. If not, we will return without printing anything. */
  if (!pnmpi_check_dbglevel(level) || !pnmpi_check_dbgnode())
    return;

  /* Print the message with rank as prefix. */
  va_list ap;
  va_start(ap, format);
  pnmpi_print_prefix_rank(stdout, format, ap);
  va_end(ap);
}
