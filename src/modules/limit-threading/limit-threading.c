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

/** \class module_limit_threading
 *
 * \brief Module to limit the threading level of MPI applications or force a
 *  specific one on-demand.
 *
 * \details This module limits the required threading level of the MPI
 *  application to a value dynamically set in the environment or PnMPI's
 *  configuration file. Simply set `PNMPI_THREADING_LEVEL` in the environment or
 *  the `thread-level` argument of this module in the configuration to one of
 *  `single`, `funneled`, `serialized` or `multiple` to set the upper limit of
 *  the MPI threading level to be uses and the application will not exceed it.
 *
 *  A second way to use this module is to set a specific MPI level to be
 *  enforced by setting `PNMPI_FORCE_THREADING_LEVEL` in the environment or
 *  `force-thread-level` as argument in the configuration. Then, MPI will be
 *  initialized with this specific MPI threading level, even if the application
 *  requested a lower level of parallelism or even didn't call
 *  \ref MPI_Init_thread but just \ref MPI_Init.
 *
 * \note To get the original threading level in most of the other modules which
 *  may analyze its value, consider to use this module as one of the last in the
 *  module stack.
 */

#include <assert.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/service.h>
#include <pnmpi/xmpi.h>


/** \brief Enum to define the MPI threading level defined by the user.
 *
 * \details This enum will be used as wrapper around MPI's threading level to
 *  define an extra value, if the user didn't define a threading level, that is
 *  not conflicting with the values of the MPI header.
 */
enum threading_level
{
  /* NOTE: Instead of the following items of this enum, the original ones
   *       defined in the MPI header will be used in the code below. However,
   *       these are required to get a unique value for THREAD_LEVEL_NONE, that
   *       isn't in conflict with the regular MPI threading levels. */
  THREAD_LEVEL_SINGLE = MPI_THREAD_SINGLE,
  THREAD_LEVEL_FUNNELED = MPI_THREAD_FUNNELED,
  THREAD_LEVEL_SERIALIZED = MPI_THREAD_SERIALIZED,
  THREAD_LEVEL_MULTIPLE = MPI_THREAD_MULTIPLE,

  THREAD_LEVEL_NONE
  /**< No thread level has been defined in the environment or via an argument in
   *   PnMPI's configuration. */
};


/** \brief Get the MPI threading level from the environment or PnMPI config.
 *
 * \details This function will check the environment and PnMPI's configuration
 *  for the MPI threading level should be used as limit or to be enforced.
 *
 * \note If the MPI level is defined in both environment and PnMPI's
 *  configuration, the value set in the environment will be preferred.
 *
 *
 * \param env_name The environment variable to be checked.
 * \param arg_name The configuration argument to be checked.
 *
 * \return The MPI threading level or \ref THREAD_LEVEL_NONE, if no MPI
 *  threading level was set.
 */
static enum threading_level get_threading_level(const char *env_name,
                                                const char *arg_name)
{
  assert(env_name);
  assert(arg_name);


  /* The user may set an MPI threading level either in the environment or
   * PnMPI's configuration file. The former will override the latter. */
  const char *env = getenv(env_name);
  if (env == NULL)
    env = PNMPI_Service_GetArgumentSelf(arg_name);

  /* If the user has defined a MPI threading level, convert it to an integer
   * (defined in the threading_level enum) and return it. */
  if (env != NULL)
    {
      if (strcmp("single", env) == 0)
        return MPI_THREAD_SINGLE;
      else if (strcmp("funneled", env) == 0)
        return MPI_THREAD_FUNNELED;
      else if (strcmp("serialized", env) == 0)
        return MPI_THREAD_SERIALIZED;
      else if (strcmp("multiple", env) == 0)
        return MPI_THREAD_MULTIPLE;

      /* If the MPI threading level is not a valid one of the MPI standard, an
       * error message will be printed and the application exits. */
      else
        PNMPI_Error("MPI threading level must be one of single, funneled, "
                    "serialized or multiple.\n");
    }

  return THREAD_LEVEL_NONE;
}


/** \brief Get the MPI threading level to which the process should be limited.
 *
 * \return The MPI threading level or \ref THREAD_LEVEL_NONE, if there is no
 *  maximum MPI threading level.
 */
static enum threading_level get_threading_level_limit()
{
  return get_threading_level("PNMPI_THREADING_LEVEL", "thread-level");
}


/** /brief Get the MPI threading level to be enforced.
 *
 * \return The MPI threading level to be enforced or \ref THREAD_LEVEL_NONE, if
 *  no MPI threading level was set.
 */
static enum threading_level get_threading_level_force()
{
  return get_threading_level("PNMPI_FORCE_THREADING_LEVEL",
                             "force-thread-level");
}


/** \brief Initialize MPI and check if \p required matches \p provided.
 *
 * \details This tiny wrapper initializes MPI with the \p required MPI threading
 *  level and prints a warning, if the MPI environment didn't provide this level
 *  of parallelism. This function should be used, if the \p required MPI
 *  threading level should be enforced.
 *
 *
 * \param argc Pointer to argc of `main`.
 * \param argv Pointer to argv of `main`.
 * \param required Required threading level.
 * \param [out] provided The provided threading level returned by the MPI
 *  library.
 *
 * \return The return value of \ref XMPI_Init_thread will be passed through.
 */
static int XMPI_Init_thread_check(int *argc, char ***argv, int required,
                                  int *provided)
{
  /* NOTE: As NULL may be a valid value for argc and argv for the MPI library,
   *       these will not be asserted. However, as the value of provided will be
   *       checked, it must not be NULL. */
  assert(provided);


  int ret = XMPI_Init_thread(argc, argv, required, provided);
  if (*provided != required)
    PNMPI_Warning("MPI threading level %d should be enforced, but MPI "
                  "provides only %d.\n",
                  required, *provided);
  return ret;
}


/** \brief MPI_Init_thread wrapper for limiting the \p required threading level
 *   or to enforce a specific one.
 *
 * \details This function will limit the \p required MPI threading level or set
 *  it to a specific value depending on the environment and configuration.
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
  /* First of all, check if a specific MPI threading level should be enforced.
   * If it doesn't match the required level, it will be altered to the forced
   * one. */
  int level;
  if (((level = get_threading_level_force()) != THREAD_LEVEL_NONE) &&
      (required != level))
    {
      PNMPI_Debug(PNMPI_DEBUG_CALL, "Application asked for MPI threading level "
                                    "%d, but %d should be enforced.\n",
                  required, level);

      /* Instead of just updating the required MPI level, call this tiny little
       * wrapper to check the threading level provided by MPI_Init_thread. If it
       * doesn't match the level to be enforced, it'll print a warning to inform
       * the user. */
      return XMPI_Init_thread_check(argc, argv, level, provided);
    }

  /* If the MPI threading level should be limited to a specific level and the
   * required level exceeds this limit, update it before passing it to the MPI
   * library below. */
  else if (((level = get_threading_level_limit()) != THREAD_LEVEL_NONE) &&
           (level < required))
    {
      PNMPI_Debug(PNMPI_DEBUG_CALL, "Limiting the threading level to %d.\n",
                  level);
      required = level;
    }

  return XMPI_Init_thread(argc, argv, required, provided);
}


/** \brief MPI_Init wrapper to enforce the MPI threading level.
 *
 * \details If the application does MPI initialization without multithreading
 *  enabled, this wrapper ensures a specific MPI level will be enforced if
 *  required.
 *
 *
 * \param argc Pointer to argc of `main`.
 * \param argv Pointer to argv of `main`.
 *
 * \return Status of `MPI_Init` or `MPI_Init_thread` returned by the MPI
 *  library.
 */
int MPI_Init(int *argc, char ***argv)
{
  /* Check if a specific MPI threading level should be enforced and route the
   * MPI call to the multithreaded initialization if required. */
  int level = get_threading_level_force();
  if (level != THREAD_LEVEL_NONE)
    {
      PNMPI_Debug(PNMPI_DEBUG_CALL, "Application didn't initialize MPI "
                                    "multithreaded. MPI threading level %d "
                                    "will be enforced.\n",
                  level);

      int provided;
      return XMPI_Init_thread_check(argc, argv, level, &provided);
    }

  /* If no MPI threading level should be enforced, the call will be passed
   * through the stack and a classical MPI_Init will be performed without any
   * multithreading enabled. */
  return XMPI_Init(argc, argv);
}
