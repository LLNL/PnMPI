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

#include <mpi.h>
#include <newstack.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/service.h>
#include <xmpi.h>


int MPI_Init(int *argc, char ***argv)
{
  if (getenv("PNMPI_TEST_STACKNAME") == NULL)
    pnmpi_error(
      "Set environment variable PNMPI_TEST_STACKNAME to module name to "
      "be searched!\n");

  PNMPI_modHandle_t stack;
  int ret =
    PNMPI_Service_GetStackByName(getenv("PNMPI_TEST_STACKNAME"), &stack);
  switch (ret)
    {
    case PNMPI_SUCCESS: printf("GetStackByName: %d\n", stack); break;
    case PNMPI_NOSTACK: pnmpi_warning("GetStackByName: not found\n"); break;

    default: pnmpi_error("Unknown error: %d\n", ret); break;
    }

  return XMPI_Init(argc, argv);
}
