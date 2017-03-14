/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/mpi_interface.h>


/** \brief Convert \p value to an \ref pnmpi_mpi_interface value.
 *
 * \details This function converts the value of \p s into a valid MPI interface
 *  language of \ref pnmpi_mpi_interface. The value of \p s will be compared
 *  case insensitive.
 *
 * \note If \p s is not a valid MPI interface language, \ref pnmpi_error will be
 *  called and PnMPI will exit immediately.
 *
 *
 * \param value The string to be compared. Should be either `C` or `Fortran`
 *  (case insensitive).
 *
 * \return \ref PNMPI_INTERFACE_C User defined the C MPI interface to be used.
 * \return \ref PNMPI_INTERFACE_FORTRAN User defined the Fortran MPI interface
 *  to be used.
 */
static pnmpi_mpi_interface convert_env(const char *value)
{
  if (strcasecmp(value, "C") == 0)
    return PNMPI_INTERFACE_C;
  else if (strcasecmp(value, "Fortran") == 0)
    return PNMPI_INTERFACE_FORTRAN;
  else
    pnmpi_error("Unknown MPI interface '%s'.\n", value);
}


/** \brief Check \p cmd for the used MPI interface.
 *
 * \details This function calls `nm` on \p cmd to determine the used MPI
 *  interface. If \p cmd uses the `MPI_Init` symbol, we can reliably tell, this
 *  is an application using the C MPI interface.
 *
 * \note If the application doesn't use dynamic symbols or doesn't use the
 *  `MPI_Init` symbol (i.e. is a non-MPI application), \ref
 *  PNMPI_INTERFACE_FORTRAN will be returned, which is false. You'll have to
 *  decide, if this affects your use-case.
 *
 *
 * \param cmd The instrumented application (= argv[0]).
 *
 * \return \ref PNMPI_INTERFACE_C \p cmd uses the C MPI interface.
 * \return \ref PNMPI_INTERFACE_FORTRAN \p cmd uses the Fortran MPI interface.
 */
static pnmpi_mpi_interface check_nm(const char *cmd)
{
  /* Generate the command line to get the MPI interface language. To get it, nm
   * will be called on the real path to cmd (may be looked up by 'which', if cmd
   * is a programm listed in PATH). If cmd uses 'MPI_Init', this indicates using
   * the MPI C interface. */
  const char *fmt = (access(cmd, F_OK) != -1)
                      ? "nm %s 2>/dev/null | grep MPI_Init >/dev/null"
                      : "nm $(which %s) 2>/dev/null | grep MPI_Init >/dev/null";
  size_t len = strlen(cmd) + strlen(fmt) - 1;
  char command[len];
  if (snprintf(command, len, fmt, cmd) == (len - 1))
    {
      /* Before executing the command, LD_PRELOAD and similar environment
       * variables have to be cleared. Otherwise nm will be called with a
       * preloaded PnMPI, which itself would execute this function, so this
       * would result in a fork bomb. */
      unsetenv("LD_PRELOAD");
      unsetenv("DYLD_INSERT_LIBRARIES");

      return (system(command) == 0) ? PNMPI_INTERFACE_C
                                    : PNMPI_INTERFACE_FORTRAN;
    }

  return PNMPI_INTERFACE_UNKNOWN;
}


/** \brief Get the MPI interface used by the instrumented application.
 *
 * \details This function determines the MPI interface \p cmd is using, so one
 *  can decide which code to execute depending on the used interface (e.g.
 *  starting Fortran MPI in \ref pnmpi_app_startup).
 *
 * \note See \ref check_nm for common problems of the auto-detection.
 *
 *
 * \param cmd The instrumented application (= argv[0]).
 *
 * \return \ref PNMPI_INTERFACE_C \p cmd uses the C MPI interface.
 * \return \ref PNMPI_INTERFACE_FORTRAN \p cmd uses the Fortran MPI interface.
 *
 *
 * \private
 */
PNMPI_INTERNAL
pnmpi_mpi_interface pnmpi_get_mpi_interface(const char *cmd)
{
  /* Check for previous invocations of this function first. If a value has been
   * cached, avoid detecting the interface language a second time and return the
   * value from cache. */
  static pnmpi_mpi_interface mpi_interface = PNMPI_INTERFACE_UNKNOWN;
  if (mpi_interface != PNMPI_INTERFACE_UNKNOWN)
    return mpi_interface;


  /* If the user defined the interface language in the environment, use the
   * defined language instead of detecting one. The interface language will be
   * stored in the PNMPI_INTERFACE environment variable and must be either C or
   * Fortran (case insensitive). */
  const char *env = getenv("PNMPI_INTERFACE");
  if (env != NULL)
    {
      pnmpi_debug(PNMPI_DEBUG_INIT,
                  "Using MPI interface '%s' defined in environment.\n", env);
      return mpi_interface = convert_env(env);
    }

  /* If no interface language was set in the environment, check the application
   * with nm for the used MPI interface. */
  return mpi_interface = check_nm(cmd);
}
