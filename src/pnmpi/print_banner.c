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

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/print.h>


/** \brief Print the PnMPI banner.
 *
 * \details Print the PnMPI banner and information about loaded modules. The
 *  banner will be printed at rank 0 and only, if the PNMPI_BE_SILENT
 *  environment variable is not set.
 *
 * \note MPI must be initialized before calling this function, so the rank can
 *  be determined.
 *
 * \note Once this function printed the banner, further calls will be ignored.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_print_banner()
{
  /* Wait until all ranks have flushed their buffers to avoid output in between
   * the PnMPI header. This is required, as PnMPI or the application itself may
   * have printed messages before MPI_Init (e.g. debug options are enabled). */
  fflush(stdout);
  fflush(stderr);
  if (PMPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS)
    pnmpi_error("PMPI_Barrier failed.\n");


  /* If we have printed the banner before, the process is not the first rank, or
   * PNMPI_BE_SILENT is set, the banner should not be printed. Return can't be
   * used here, as all ranks must meet at the barrier below to synchronize the
   * header output.
   * If the header will be printed, save the printed state so future calls won't
   * print the banner a second time. */
  static int printed = 0;
  if (printed || (pnmpi_get_rank() != 0) || (getenv("PNMPI_BE_SILENT") != NULL))
    goto wait_header_printed;

  printed = 1;


  /* The PnMPI banner is a modified version of an ASCII-art generated with
   * http://patorjk.com/software/taag/ */
  printf("  _____   _ __   __  __  _____   _____\n"
         " |  __ \\ | '_ \\ |  \\/  ||  __ \\ |_   _|\n"
         " | |__) || | | || \\  / || |__) |  | |\n"
         " |  ___/ |_| |_|| |\\/| ||  ___/   | |\n"
         " | |            | |  | || |      _| |_\n"
         " |_|            |_|  |_||_|     |_____|\n"
         "\n");


  /* If no modules have been loaded, no information can be printed about them,
   * so a message will be printed and the function returns.  */
  if (modules.num == 0)
    {
      printf(" No modules loaded.\n\n");
      goto wait_header_printed;
    }


  /* Print some basic information about the loaded modules. By default the
   * module and stack names will be printed only. */
  printf(" Loaded modules:\n"
         "  Stack default:\n");

  size_t i;
  for (i = 0; i < modules.num; i++)
    {
      /* If module is a stack delimiter, print a new stack header instead of
       * handling the modules configuration. */
      if (modules.module[i]->stack_delimiter)
        {
          printf("  Stack %s:\n", modules.module[i]->name);
          continue;
        }


      /* If the module has registered itself, the registered module name will be
       * printed, otherwise the name from the configuration file. If the module
       * handles Pcontrol a separate keyword will be printed near the module
       * name. */
      printf("    %s%s%s\n",
             modules.module[i]->registered ? modules.module[i]->username
                                           : modules.module[i]->name,
             modules.module[i]->registered ? " [registered]" : "",
             (modules.module[i]->pcontrol != 0) ? " [Pcontrol]" : "");
    }
  printf("\n");


wait_header_printed:
  /* Wait until rank 0 has flushed its buffers to avoid other ranks printing
   * output in other functions, while rank 0 is printing the header. */
  fflush(stdout);
  fflush(stderr);
  if (PMPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS)
    pnmpi_error("PMPI_Barrier failed.\n");
}
