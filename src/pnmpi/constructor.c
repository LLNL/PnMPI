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

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/private/app_hooks.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/fallback_init.h>
#include <pnmpi/private/modules.h>


/** \brief Global flag indicating a constructor has been called.
 *
 * \details This flag indicates, that either a compiler specific constructor or
 *  the fallback constructor \ref _init has been called. Other functions in
 *  PnMPI use this, to check if PnMPI constructors and destructors will be
 *  called, or the fallback mode has to be used.
 *
 *
 * \private
 */
PNMPI_INTERNAL
int pnmpi_constructor_called = 0;


/** \brief The PnMPI constructor.
 *
 * \details This function will be used to call all constructors in a specific
 *  order. Independend constructors can't be used, as their order gets random
 *  for static linking PnMPI to an executable.
 *
 *
 * \param argc Count of \p argv.
 * \param argv The argument vector of the executable.
 *
 *
 * \private
 */
PNMPI_CONSTRUCTOR(101)
void pnmpi_constructor(int argc, char **argv)
{
  /* Set a flag that the constructor has been called, so other functions may use
   * this flag to test what actions to do depending on the initialization. */
  pnmpi_constructor_called = 1;

  /* Call all constructors. */
  pnmpi_PreInit();
  pnmpi_app_startup(argc, argv);
}


/* If no compiler specific implementation of PNMPI_CONSTRUCTOR is available, the
 * fallback constructor has to be used.
 *
 * Note: Using _init is obsolute, dangerous and is only a fallback for legacy
 *       systems. If PnMPI has no compiler specific implementation of
 *       PNMPI_CONSTRUCTOR for your compiler yet, please file an issue. */
#ifdef PNMPI_HAVE_NO_CONSTRUCTOR

#if defined(__linux__) || defined(__linux) || defined(linux)

/** \brief Parse /proc/self/cmdline to get argc and argv.
 *
 *
 * \param argc Pointer to dest argc.
 * \param argv Pointer to dest argv.
 */
static void read_cmdline(int *argc, char ***argv)
{
  FILE *fp = fopen("/proc/self/cmdline", "rb");
  if (fp == NULL)
    PNMPI_Error("ERROR could not open arguments\n");

  int len = 1024;
  char *buffer = (char *)malloc(len * sizeof(char));
  int readBytes = 0;
  while (!feof(fp))
    {
      if (readBytes + 100 > len)
        {
          char *tmp = malloc(len * 2 * sizeof(char));
          memcpy(tmp, buffer, readBytes);
          free(buffer);
          buffer = tmp;
          len *= 2;
        }

      int read = fread(buffer + readBytes, sizeof(char), 100, fp);
      readBytes += read;
    }

  char *curstr = buffer;

  len = 30;
  *argv = (char **)malloc(sizeof(char *) * len);
  *argc = 0;
  while (curstr && (curstr - buffer) < readBytes)
    {
      if (*argc > len)
        {
          char **tmp = (char **)malloc(sizeof(char *) * len);
          memcpy(tmp, *argv, sizeof(char *) * (*argc));
          free(*argv);
          *argv = tmp;
          len *= 2;
        }
      (*argv)[(*argc)++] = curstr;
      curstr = strchr(curstr, '\0') + 1;
    }
  (*argv)[(*argc)] = NULL;
}

#else

/** \brief Fallback implementation to return empty \p argc and \p argv.
 *
 *
 * \param argc Pointer to dest argc.
 * \param argv Pointer to dest argv.
 */
static void read_cmdline(int *argc, char ***argv)
{
  PNMPI_Warning("PnMPI falback constructor: No mechanism to get argc/argv!\n");

  *argc = 0;
  *argv = (char **)"";
}

#endif


/** \brief Fallback constructor.
 *
 * \details If the compiler does not support an implementation for \ref
 *  PNMPI_CONSTRUCTOR, the following fallback constructor will be used to call
 *  the independend constructors of PnMPI.
 */
void _init()
{
  int argc;
  char **argv;
  read_cmdline(&argc, &argv);

  pnmpi_constrctor(argc, argv);
}

#endif


/** \brief Fallback constructor.
 *
 * \details If the compiler does not support an implementation for \ref
 *  PNMPI_CONSTRUCTOR nor does \ref _init get called by the dynamic loader, this
 *  function will be called by \ref MPI_Init as a backup.
 *
 * \note This function can't call all of the constructor functions, as some of
 *  them, especially \ref pnmpi_app_startup, must be called before the execution
 *  of `main`.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_fallback_init()
{
  /* If any constructor (either the compiler specific or fallback one) has been
   * called before, execution of this function should be skipped. */
  if (pnmpi_constructor_called)
    return;

  /* Call the regular constructors. */
  pnmpi_PreInit();


  /* Check if the user is using PNMPI_AppStartup or PNMPI_AppShutdown hooks, due
   * these are not supported in this scenario. */
  if (pnmpi_hook_activated("PNMPI_AppStartup") ||
      pnmpi_hook_activated("PNMPI_AppShutdown"))
    PNMPI_Error("You are using modules which require the 'PNMPI_AppStartup' or "
                "'PNMPI_AppShutdown' hooks, but your environment does not "
                "support them. Please deactivate them or check your "
                "environment.\n");

  /* Call the optional version of PNMPI_AppStartup, that may be called at
   * initialization time. */
  pnmpi_call_hook("PNMPI_AppStartupOptional");
}
