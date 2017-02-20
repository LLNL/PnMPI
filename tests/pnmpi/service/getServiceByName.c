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

/* This test case checks, if a registered service may be queried by an other
 * module. There will be a valid test and two failure-tests for unknown name and
 * not-matching signature in this test file. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


#define service_name "test"
#define service_sig "d"

#ifndef TEST_SERVICENAME
#define TEST_SERVICENAME service_name
#endif

#ifndef TEST_SERVICESIG
#define TEST_SERVICESIG service_sig
#endif

#ifndef TEST_MODULE
#define TEST_MODULE 0
#endif


int foo(int a)
{
  printf("%s: %d\n", __FUNCTION__, a);
  return a;
}


void registerService()
{
  PNMPI_Service_descriptor_t service;
  service.fct = &foo;
  strncpy(service.name, service_name, PNMPI_SERVICE_NAMELEN);
  strncpy(service.sig, service_sig, PNMPI_SERVICE_SIGLEN);

  int ret = PNMPI_Service_RegisterService(&service);
  if (ret != PNMPI_SUCCESS)
    pnmpi_error("Error: %d\n", ret);
}


void queryService()
{
  /* Query the service and print a message with the return code. */
  PNMPI_Service_descriptor_t buffer;
  int ret = PNMPI_Service_GetServiceByName(TEST_MODULE, TEST_SERVICENAME,
                                           TEST_SERVICESIG, &buffer);
  switch (ret)
    {
    case PNMPI_SUCCESS:
      printf("getServiceByName: %p\n", (void *)buffer.fct);
      buffer.fct(__LINE__);
      break;
    case PNMPI_NOSERVICE:
      pnmpi_warning("getServiceByName: service not found\n");
      break;
    case PNMPI_SIGNATURE:
      pnmpi_warning("getServiceByName: signature not found\n");
      break;

    default: pnmpi_error("Unknown error: %d\n", ret); break;
    }
}


void PNMPI_RegistrationPoint()
{
  /* The first loaded module will register the service, others try to access the
   * service. */
  int self;
  if (PNMPI_Service_GetModuleSelf(&self) != PNMPI_SUCCESS)
    pnmpi_error("Can't get module ID.\n");
  if (self == 0)
    registerService();
  else
    queryService();
}


/* CONFIGS: found no_service_self no_service_other no_signature
 *
 * MODTYPE: XMPI
 *
 * PNMPICONF: module @MODNAME@\n
 * PNMPICONF: module @MODNAME@\n
 *
 * RUN: @PNMPIZE@ -m @CMAKE_CURRENT_BINARY_DIR@ -c @PNMPICONF@ @TESTBIN_MPI_C@
 *
 * PASS-found: getServiceByName: 0x[0-f]+\nfoo: [0-9]+
 *
 * COMPILE_FLAGS-no_service_self: -DTEST_SERVICENAME=\"foo\"
 * PASS-no_service_self: getServiceByName: service not found
 *
 * COMPILE_FLAGS-no_service_other: -DTEST_MODULE=1
 * PASS-no_service_other: getServiceByName: service not found
 *
 * COMPILE_FLAGS-no_signature: -DTEST_SERVICESIG=\"p\"
 * PASS-no_signature: getServiceByName: signature not found
 */
