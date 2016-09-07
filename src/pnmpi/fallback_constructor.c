/*
Copyright (c) 2008
Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory.
Written by Martin Schulz, schulzm@llnl.gov.
LLNL-CODE-402774,
All rights reserved.

This file is part of P^nMPI.

Please also read the file "LICENSE" included in this package for
Our Notice and GNU Lesser General Public License.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
(as published by the Free Software Foundation) version 2.1
dated February 1999.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY
OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
terms and conditions of the GNU General Public License for more
details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the

Free Software Foundation, Inc.,
59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Only enable the fallback constructor for builds without individual
 * constructors enabled. */
#ifndef __GNUC__


/* Declaration of all constructors. They are not in a seperate header file, as
 * they will be used in this file only. If the normal constructors are enabled,
 * no other files call them. */
void initialize_pnmpi_threaded();
void pnmpi_PreInit();
void pnmpi_app_startup(int argc, char **argv);


/** \brief Variable to save if \ref _init was called by the dynamic loader. */
int pnmpi_fallback_init_called = 0;


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
    {
      fprintf(stderr, "ERROR could not open arguments\n");
      exit(1);
    }

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
  fprintf(stderr,
          "PnMPI falback constructor: No mechanism to get argc/argv!\n");

  *argc = 0;
  *argv = (char **)"";
}

#endif


/** \brief Call constructors that may be called later.
 *
 * \details If the regular call of \ref _init fails, this function will be
 *  called later to call all constructors that don't need to be called at
 *  program initialization.
 */
static void pnmpi_call_constructors()
{
  initialize_pnmpi_threaded();
  pnmpi_PreInit();
}


/** \brief Fallback constructor for failed \ref _init call.
 *
 * \details If the compiler does not support '__attribute__((constructor))', the
 *  fallback constructor \ref _init will be used to call the independend
 *  constructors of PnMPI. If the dynamic loader of the system does not call
 *  \ref _init, this function may be called by the \ref MPI_Init later.
 */
void pnmpi_fallback_init()
{
  /* If the fallback constructor was called before (by _init), we may skip it
   * now. */
  if (pnmpi_fallback_init_called)
    return;


  /* Call the regular constructors. */
  pnmpi_call_constructors();


  /* Check if the user is using app_startup or app_shutdown, due these are not
   * supported in this scenario. */
  if (pnmpi_hook_activated("app_startup") ||
      pnmpi_hook_activated("app_shutdown"))
    {
      fprintf(stderr, "You are using modules which require the 'app_startup' "
                      "or 'app_shutdown' hooks, but your system does not "
                      "support them. Please deactivate them or check your "
                      "system.\n");
      exit(EXIT_FAILURE);
    }
}


/** \brief Fallback constructor.
 *
 * \details If the compiler does not support '__attribute__((constructor))', the
 *  following fallback constructor will be used to call the independend
 *  constructors of PnMPI.
 */
void _init()
{
  /* Save that _init was called (e.g. by the dynamic loader) to indicate that
   * the constructors have been called. If this flag won't be set,
   * pnmpi_fallback_init is able to determine that _init was not called and may
   * call some constructors later. */
  pnmpi_fallback_init_called = 1;


  /* Call the regular constructors. */
  pnmpi_call_constructors();


  /* Call the constructors which need to be called before main is started. */

  /* Get argc and argv. */
  int argc;
  char **argv;
  read_cmdline(&argc, &argv);

  pnmpi_app_startup(argc, argv);
}


#endif
