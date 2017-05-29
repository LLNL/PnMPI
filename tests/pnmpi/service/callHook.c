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

/* This test case will check, if the module is able to call recursive hooks. */

#include <stdio.h>

#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


#ifndef HOOK_NAME
#define HOOK_NAME PNMPI_HOOK_SAME
#endif


void hook_foo()
{
  PNMPI_modHandle_t h;
  PNMPI_Service_GetModuleSelf(&h);
  printf("module %d: %s\n", h, __FUNCTION__);
}

void PNMPI_AppStartup()
{
  PNMPI_modHandle_t h;
  PNMPI_Service_GetModuleSelf(&h);
  printf("module %d: %s\n", h, __FUNCTION__);

#ifdef NEWSTACK
  if (h != 3)
    {
      PNMPI_modHandle_t s;
      PNMPI_Service_GetStackByName(NEWSTACK, &s);
      PNMPI_Service_CallHook_NewStack(HOOK_NAME, s);
    }
  else
#endif
    PNMPI_Service_CallHook(HOOK_NAME);
}


/* CONFIGS: default_stack other_hook newstack
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: stack foo\n
 * PNMPICONF: module @MODNAME@\n
 *
 * RUN: @PNMPIZE@ -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@ @TESTBIN_MPI_C@
 *
 * PASS-default_stack: module 0: PNMPI_AppStartup\nmodule 1: PNMPI_AppStartup\n
 *
 * COMPILE_FLAGS-other_hook: -DHOOK_NAME=\"hook_foo\"
 * PASS-other_hook: module 0: PNMPI_AppStartup\nmodule 1: hook_foo\n
 *
 * COMPILE_FLAGS-newstack: -DNEWSTACK=\"foo\"
 * PASS-newstack: module 0: PNMPI_AppStartup\nmodule 3: PNMPI_AppStartup\n
 */
