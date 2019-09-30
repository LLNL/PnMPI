/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2019 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2019 RWTH Aachen University, Federal Republic of Germany
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

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/force_link.h>
#include <pnmpi/service.h>


/** \brief Dummy function to force the linker to link all required symbols.
 *
 * \details PnMPI provides some functionality for the loaded modules. However,
 *  the required symbols won't be linked into the application, if PnMPI is
 *  linked statically. This function will reference all required symbols and is
 *  called from \ref MPI_Init and \ref MPI_Init_thread, forcing the linker to
 *  link all required symbols into the binary.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_force_link(void)
{
  /* Module service functions. */
  PNMPI_status_t (*volatile ptr_service_01)(const char *name) =
    PNMPI_Service_RegisterModule;
  PNMPI_status_t (*volatile ptr_service_02)(const char *name,
                                            PNMPI_modHandle_t *handle) =
    PNMPI_Service_GetModuleByName;
  PNMPI_status_t (*volatile ptr_service_03)(PNMPI_modHandle_t * handle) =
    PNMPI_Service_GetModuleSelf;
  PNMPI_status_t (*volatile ptr_service_04)(
    const char *name, PNMPI_modHandle_t *handle) = PNMPI_Service_GetStackByName;
  PNMPI_status_t (*volatile ptr_service_05)(
    const PNMPI_Global_descriptor_t *global) = PNMPI_Service_RegisterGlobal;
  PNMPI_status_t (*volatile ptr_service_06)(
    PNMPI_modHandle_t handle, const char *name, const char sig,
    PNMPI_Global_descriptor_t *dest) = PNMPI_Service_GetGlobalByName;
  PNMPI_status_t (*volatile ptr_service_07)(
    const PNMPI_Service_descriptor_t *service) = PNMPI_Service_RegisterService;
  PNMPI_status_t (*volatile ptr_service_08)(
    PNMPI_modHandle_t handle, const char *name, const char *sig,
    PNMPI_Service_descriptor_t *dest) = PNMPI_Service_GetServiceByName;
  PNMPI_status_t (*volatile ptr_service_09)(
    PNMPI_modHandle_t handle, const char *name, const char **dest) =
    PNMPI_Service_GetArgument;
  PNMPI_status_t (*volatile ptr_service_10)(
    PNMPI_modHandle_t handle, const char *name, const char **dest) =
    PNMPI_Service_GetArgument;
  const char *(*volatile ptr_service_11)(const char *name) =
    PNMPI_Service_GetArgumentSelf;
  PNMPI_status_t (*volatile ptr_service_12)(
    const PNMPI_modHandle_t handle, int *flag) = PNMPI_Service_GetPcontrol;
  int (*volatile ptr_service_13)() = PNMPI_Service_GetPcontrolSelf;
  const char *(*volatile ptr_service_14)(PNMPI_status_t err) =
    PNMPI_Service_strerror;
  PNMPI_status_t (*volatile ptr_service_15)(void **ptr) =
    PNMPI_Service_GetReturnAddress;
  PNMPI_status_t (*volatile ptr_service_16)(void **ptr) =
    PNMPI_Service_GetFunctionAddress;


  /* Debug printing functions. */
  void (*volatile ptr_debug_01)(PNMPI_debug_level_t level, const char *format,
                                ...) = pnmpi_print_debug;
  void (*volatile ptr_debug_02)(const char *format, ...) = pnmpi_print_warning;
  void (*volatile ptr_debug_03)(const char *prefix, const char *function,
                                const int line, const char *format, ...) =
    pnmpi_print_error;


  /* This if-clause has no special meaning, except keeping the compiler silent,
   * not to warn about unused variables. */
  if ((ptr_service_01 || ptr_service_02 || ptr_service_03 || ptr_service_04 ||
       ptr_service_05 || ptr_service_06 || ptr_service_07 || ptr_service_08 ||
       ptr_service_09 || ptr_service_10 || ptr_service_11 || ptr_service_12 ||
       ptr_service_13 || ptr_service_14 || ptr_service_15 || ptr_service_16) ||
      (ptr_debug_01 || ptr_debug_02 || ptr_debug_03))
    return;
}
