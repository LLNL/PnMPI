/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

#include <stdlib.h>
#include <string.h>

#include <argp.h>
#include <unistd.h>

#include "pnmpi-config.h"


/* Configure argp.
 *
 * Argp is used to parse command line options. It handles the most common
 * options like --help and --version, so that a manual coding of getopt code is
 * not required anymore.
 */
const char *argp_program_version = "P^nMPI " PNMPI_VERSION;

const char *argp_program_bug_address = "https://github.com/LLNL/PnMPI/issues";

static char doc[] =
  "P^nMPI -- Virtualization Layer for the MPI Profiling Interface";

static char args_doc[] = "utility [utility options]";

/* Initialize argp parser. We'll use above defined parameters for documentation
 * strings of argp. */
static struct argp argp = { NULL, NULL, args_doc, doc };


static int appendenv(const char *name, const char *value)
{
  char *temp = getenv(name);
  if (temp == NULL)
    return setenv(name, value, 0);

  size_t len_temp = strlen(temp);
  size_t len_value = strlen(value);

  char buffer[len_temp + len_value + 2];

  strcpy(buffer, temp);
  strcat(buffer, ":");
  strcat(buffer, value);

  return setenv(name, buffer, 1);
}


int main(int argc, char **argv)
{
  /* Parse our arguments; every option seen by parse_opt will be reflected in
   * arguments. */
  int ind;
  argp_parse(&argp, argc, argv, 0, &ind, NULL);

  if (ind < argc)
    {
      /* Append */
      appendenv("PNMPI_LIB_PATH", PNMPI_MODULES_DIR);
      appendenv("LD_PRELOAD", PNMPI_LIBRARY_DIR "/libpnmpif.so");

      execvp(argv[ind], argv + ind);
      fprintf(stderr, "error: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
}
