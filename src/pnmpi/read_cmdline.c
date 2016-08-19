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

#include "core.h"


/**
 * Tries to demangle the argc and argv application parameters out of
 * /proc/self/cmdline or in a OS specific way.
 *
 * Parameters are seperated by \0 characters in this file.
 *
 * TODO: Do these mechanism cover all relevant operating systems?
 *
 *
 * @param argc
 * @param argv
 */
#if defined(__linux__) || defined(linux) || defined(__linux)
void read_cmdline(int *argc, char ***argv)
{
  FILE *fp = fopen("/proc/self/cmdline", "rb");
  if (fp == NULL)
    {
      printf("ERROR could not open arguments\n");
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
#elif defined(__APPLE__)
extern int *_NSGetArgc(void);
extern char ***_NSGetArgv(void);
void read_cmdline(int *argc, char ***argv)
{
  *argc = *(_NSGetArgc());
  *argv = *(_NSGetArgv());
}
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32)
extern int __argc;
extern char **__argv;
void read_cmdline(int *argc, char ***argv)
{
  *argc = __argc;
  *argv = __argv;
}
#elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__ICC)
#include <features.h>

/* This only works for GLIBC 2.4 upwards as the GNU .init_array mechanism passes
 * argc + argv to the initialization functions (the System V ABI does NOT!). */
#if __GLIBC_PREREQ(2, 4)

int myargc = 0;
char **myargv = (char **)"";

void myinitfunc(int argc, char **argv, char **envp)
{
  myargc = argc;
  myargv = argv;
}

__attribute__((section(".init_array"))) static void *init_array_entry =
  &myinitfunc;
#else
#warning MPIZE: No mechanism to get argc/argv arguments for MPI_INIT!
#endif
void read_cmdline(int *argc, char ***argv)
{
  *argc = myargc;
  *argv = myargv;
}
#else
#warning MPIZE: No mechanism to get argc/argv arguments for MPI_INIT!
void read_cmdline(int *argc, char ***argv)
{
  *argc = 0;
  *argv = (char **)"";
}
#endif
