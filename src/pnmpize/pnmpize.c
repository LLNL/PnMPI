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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <argp.h>
#include <unistd.h>

#include "pnmpi-config.h"
#include <pnmpi/debug_io.h>


/* Define the P^nMPI library to use for preloading. If Fortran is supported, the
 * Fortran PnMPI will be used, thus it covers C and Fortran. In any other case
 * the C library will be used, so Fortran binaries can't be executed with
 * preloaded P^nMPI. */
#ifdef PNMPI_HAVE_FORTRAN
#define PNMPI_LIBRARY_NAME "libpnmpif"
#else
#define PNMPI_LIBRARY_NAME "libpnmpi"
#endif


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
  { "debug", 'd', "LEVEL", 0, "Print debug messages. Level may be one of the "
                              "following keywords: init, modules or call. "
                              "Multiple levels may be combined by calling this "
                              "option for each debug level." },
  { "modules", 'm', "PATH", 0, "Module path" },
  { "debug-node", 'n', "RANK", 0, "Only print debug messages for this rank. "
                                  "Note: This option has no effect on debug "
                                  "levels init and modules." },
  { "quiet", 'q', 0, 0, "Don't produce any output" },
  { "silent", 's', 0, OPTION_ALIAS },
  { 0 }
};

/* Initialize argp parser. We'll use above defined parameters for documentation
 * strings of argp. A forward declaration for parse_arguments is added to define
 * parse_arguments together with the other functions below. */
static error_t parse_arguments(int key, char *arg, struct argp_state *state);
static struct argp argp = { options, &parse_arguments, args_doc, doc };


/** \brief Set environment variable \p name to \p value or append to it.
 *
 * \details This function will set environment variable \p name to \p value. If
 *  \p name is already present in the environment, \p value will be appended to
 *  it, separated by a colon.
 *
 *
 * \param name Name of the environment variable.
 * \param value Value to be set or appended.
 *
 * \return This function returns the return value of setenv. See the setenv
 *  manual for more information about it.
 */
static int appendenv(const char *name, const char *value)
{
  char *temp = getenv(name);

  /* If variable name is not present in environment, set it as new variable in
   * the environment and reutrn the return value. */
  if (temp == NULL)
    return setenv(name, value, 0);

  /* Allocate a new buffer and save the current and new value concatenated and
   * separated by colon into it. Overwrite the current environment variable with
   * the value from buffer. */
  size_t len = strlen(temp) + strlen(value);
  char buffer[len + 2];
  snprintf(buffer, len + 2, "%s:%s", temp, value);

  return setenv(name, buffer, 1);
}


/** \brief Enable debug-level \p name.
 *
 * \details This function enables the debug-level \p name. It will be OR'ed with
 *  the current enabled levels.
 *
 *
 * \param state The argp parser state.
 * \param name Name of the level to be enabled.
 */
static void set_dbglevel(const struct argp_state *state, const char *name)
{
  /* Store the selected level in a static int, so it doesn't have to be looked
   * up in the next invocation of this function and can be OR'ed directly. */
  static int level = 0;


  /* Compare the debug level name with the available debug levels of PnMPI. If
   * the name is known, it will be OR'ed with the current debug level to get the
   * new one. If the level is unknown an error will be printed and PnMPIze
   * exits. */
  if (strcmp(name, "init") == 0)
    level = level | PNMPI_DEBUG_INIT;

  else if (strcmp(name, "modules") == 0)
    level = level | PNMPI_DEBUG_MODULE;

  else if (strcmp(name, "call") == 0)
    level = level | PNMPI_DEBUG_CALL;

  else
    argp_error(state, "Unknown debug level %s.", name);


  /* Set the new debug level as PNMPI_DBGLEVEL environment variable. For this
   * the level must be converted into a string. */
  char buffer[11];
  snprintf(buffer, 11, "%d", level);
  setenv("PNMPI_DBGLEVEL", buffer, 1);
}


/** \brief Argument parser for argp.
 *
 * \note See argp parser documentation for detailed information about the
 *  structure and functionality of function.
 */
static error_t parse_arguments(int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case 'c': setenv("PNMPI_CONF", arg, 1); break;
    case 'd': set_dbglevel(state, arg); break;
    case 'm': setenv("PNMPI_LIB_PATH", arg, 1); break;
    case 'n': setenv("PNMPI_DBGNODE", arg, 1); break;
    case 'q':
    case 's':
      setenv("PNMPI_BE_SILENT", "1", 1);
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


int main(int argc, char **argv)
{
  /* Parse our arguments: Options will be parsed until the first non-option is
   * found. Parsed arguments will manipulate the current environment to initial-
   * ize it for use with P^nMPI. If there are no additional non-options in our
   * arguments, argp_parse will print the usage message and exit immediatly, no
   * extra evaluation is required here. The index of the first non-option will
   * be stored in ind. */
  int ind;
  argp_parse(&argp, argc, argv, ARGP_IN_ORDER, &ind, NULL);

#ifdef __APPLE__
  /* For apple systems, add libpnmpif to DYLD_INSERT_LIBRARIES, the apple ver-
   * sion of LD_PRELOAD. DYLD_FORCE_FLAT_NAMESPACE has to be set, to get all
   * symbols in the same namespace. Otherwise P^nMPI and libmpi won't see each
   * other and no preloading will happen. */
  appendenv("DYLD_INSERT_LIBRARIES",
            PNMPI_LIBRARY_DIR "/" PNMPI_LIBRARY_NAME ".dylib");
  setenv("DYLD_FORCE_FLAT_NAMESPACE", "1", 1);

#else
  /* For other systems (Linux and other UNIX platforms), add libpnmpif to
   * LD_PRELOAD to load P^nMPI in front of libmpi. */
  appendenv("LD_PRELOAD", PNMPI_LIBRARY_DIR "/" PNMPI_LIBRARY_NAME ".so");
#endif

  /* Execute the utility. If the utility could be started, pnmpize will exit
   * here. In any other case, the following error processing will be called. */
  if (execvp(argv[ind], argv + ind) < 0)
    {
      fprintf(stderr, "Could not execute %s: %s\n", argv[ind], strerror(errno));
      return EXIT_FAILURE;
    }
}
