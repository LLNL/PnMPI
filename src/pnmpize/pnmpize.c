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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <argp.h>
#include <unistd.h>

#include "pnmpi-config.h"


/** \brief Struct to store PnMPI options in.
 */
struct arguments
{
  bool silent;        //< True, if PnMPI should suppress any output.
  const char *config; //< Path to an optional configuration file.
};


/* Configure argp.
 *
 * Argp is used to parse command line options. It handles the most common
 * options like --help and --version, so that a manual coding of getopt code is
 * not required anymore. For detailed information about the varaibles below, see
 * the argp documentation.
 */
const char *argp_program_version = "P^nMPI " PNMPI_VERSION;

const char *argp_program_bug_address = "https://github.com/LLNL/PnMPI/issues";

static char doc[] =
  "P^nMPI -- Virtualization Layer for the MPI Profiling Interface";

static char args_doc[] = "utility [utility options]";

static struct argp_option options[] = {
  { "config", 'c', "FILE", 0, "Configuration file" },
  { "quiet", 'q', 0, 0, "Don't produce any output" },
  { "silent", 's', 0, OPTION_ALIAS },
  { 0 }
};

/* Initialize argp parser. We'll use above defined parameters for documentation
 * strings of argp. A forward declaration for parse_arguments is added to define
 * parse_arguments together with the other functions below. */
static error_t parse_arguments(int key, char *arg, struct argp_state *state);
static struct argp argp = { options, &parse_arguments, args_doc, doc };


/** \brief Argument parser for argp.
 *
 * \info See argp parser documentation for detailed information about the
 *  structure and functionality of function.
 */
static error_t parse_arguments(int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse. It is a pointer to the arguments
   * struct to store any parsed arguments into. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'c': arguments->config = arg; break;
    case 'q':
    case 's':
      arguments->silent = true;
      break;

    /* If we have parsed all options, iterate through all non-options in argv.
     * If at there are no non-options in our arguments (argv), the user
     * specified no utility to call, so we'll print the argp usage here, which
     * will exit pnmpize immediatly after printing the usage message. */
    case ARGP_KEY_END:
      if (state->arg_num == 0)
        argp_usage(state);
      break;

    default: return ARGP_ERR_UNKNOWN;
    }

  return 0;
}


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
  struct arguments options;

  /* Set default options. */
  options.silent = false;
  options.config = NULL;


  /* Parse our arguments: Options will be parsed until the first non-option is
   * found. Arguments will be stored in arguments struct options. If there are
   * no additional non-options in our arguments, argp_parse will print the usage
   * message and exit immediatly. The index of the first non-option will be
   * stored in ind. */
  int ind;
  argp_parse(&argp, argc, argv, ARGP_IN_ORDER, &ind, &options);


  /* Prepare the environment for PnMPI. The following environment variables will
   * be set before the urility is executed:
   *
   *  - PNMPI_BE_SILENT=1 if options.silent is true.
   */
  if (options.silent)
    setenv("PNMPI_BE_SILENT", "1", 1);
  if (options.config != NULL)
    setenv("PNMPI_CONF", options.config, 1);

  appendenv("PNMPI_LIB_PATH", PNMPI_MODULES_DIR);
  appendenv("LD_PRELOAD", PNMPI_LIBRARY_DIR "/libpnmpif.so");

  execvp(argv[ind], argv + ind);
  fprintf(stderr, "error: %s\n", strerror(errno));
  return EXIT_FAILURE;
}
