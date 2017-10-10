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

/** \class module_limit_threading
 *
 * \brief Module to limit the threading level of MPI applications on-demand.
 *
 * \details This module limits the required threading level of the MPI
 *  application to a value dynamically set in the environment. Simply set
 *  `PNMPI_THREADING_LEVEL` to an integer with the upper limit of the threading
 *  level to use and the application will not exceed it.
 *
 * \note To get the original threading level in most of the other modules which
 *  may analyze its value, consider to use this module as one of the last in the
 *  module stack.
 */

#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/xmpi.h>


/** \brief MPI_Init_thread wrapper for limiting the \p required threading level.
 *
 * \details This function will check the environment for `PNMPI_THREADING_LEVEL`
 *  and limits the value of \p required to the value set in the environment.
 *
 *
 * \param argc Pointer to argc of `main`.
 * \param argv Pointer to argv of `main`.
 * \param required Required threading level.
 * \param [out] provided The provided threading level returned by the MPI
 *  library.
 *
 * \return Status of `MPI_Init_thread` returned by the MPI library.
 */
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
  /* If the user specified 'PNMPI_THREADING_LEVEL' in the environment and its
   * value is lower than the threading level required by the callee, limit the
   * required threading level to the value set in the environment. */
  const char *env = getenv("PNMPI_THREADING_LEVEL");
  if (env != NULL)
    {
      int value = atoi(env);
      if (value < required)
        {
          required = value;
          PNMPI_Debug(PNMPI_DEBUG_CALL, "Limiting the threading level to %d.\n",
                      required);
        }
    }

  return XMPI_Init_thread(argc, argv, required, provided);
}
