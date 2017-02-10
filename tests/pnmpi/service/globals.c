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
#include <string.h>

#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


int global_int = 42;


void PNMPI_RegistrationPoint()
{
  int ret;

  /* First test case for a global with invalid signature. */
  PNMPI_Global_descriptor_t global;
  global.addr.i = &global_int;
  strncpy(global.name, "test", PNMPI_SERVICE_NAMELEN);
  global.sig = 'd';

  /* First test case for a global with valid data. */
  ret = PNMPI_Service_RegisterGlobal(&global);
  if (ret != PNMPI_SUCCESS)
    {
      printf("Test 1 failed: error %d.\n", ret);
      exit(EXIT_FAILURE);
    }
  printf("Registered global '%s'\n", global.name);


  PNMPI_modHandle_t self;
  if (PNMPI_Service_GetModuleSelf(&self) != PNMPI_SUCCESS)
    pnmpi_error("Can't get module handle.\n");


  /* Second test case to get a global with invalid name. */
  PNMPI_Global_descriptor_t buffer;
  ret = PNMPI_Service_GetGlobalByName(self, "test-1234", 'd', &buffer);
  if (ret != PNMPI_NOGLOBAL)
    {
      printf("Test 3 failed: error %d.\n", ret);
      exit(EXIT_FAILURE);
    }

  /* Third test case to get a global with not matching signature. */
  ret = PNMPI_Service_GetGlobalByName(self, "test", '\0', &buffer);
  if (ret != PNMPI_SIGNATURE)
    {
      printf("Test 4 failed: error %d.\n", ret);
      exit(EXIT_FAILURE);
    }

  /* Fourth test case to get a global with valid arguments. */
  ret = PNMPI_Service_GetGlobalByName(self, "test", 'd', &buffer);
  if (ret != PNMPI_SUCCESS)
    {
      printf("Test 5 failed: error %d.\n", ret);
      exit(EXIT_FAILURE);
    }
  printf("Got global data: %d\n", *(buffer.addr.i));
}
