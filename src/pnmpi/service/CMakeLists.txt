# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2019 Lawrence Livermore National Laboratories, United States of America
#  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2019 RWTH Aachen University, Federal Republic of Germany
#
#
# P^nMPI is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation version 2.1 dated February 1999.
#
# P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with P^nMPI; if not, write to the
#
#   Free Software Foundation, Inc.
#   51 Franklin St, Fifth Floor
#   Boston, MA 02110, USA
#
#
# Written by Martin Schulz, schulzm@llnl.gov.
#
# LLNL-CODE-402774

include(PnMPI_doc)


# Install and manage the public and private header files of the PnMPI sources.
# This has to be done to use the same headers for the PnMPI sources and the API
# without getting conflicts about the used paths. Otherwise you'd have to take
# care about file locations after installation, especially if you use the
# headers over multiple directories.
pnmpi_add_header(service.h)


# Install the public man pages for this object.
pnmpi_add_man(PNMPI_Service_GetModuleByName "")
pnmpi_add_man(PNMPI_Service_GetModuleByName PNMPI_Service_GetModuleSelf)
pnmpi_add_man(PNMPI_Service_GetModuleByName PNMPI_Service_RegisterModule)
pnmpi_add_man(PNMPI_Service_GetStackByName "")
pnmpi_add_man(PNMPI_Service_GetStackByName PNMPI_Service_ChangeStack)
pnmpi_add_man(PNMPI_Service_GetGlobalByName "")
pnmpi_add_man(PNMPI_Service_GetGlobalByName PNMPI_Service_RegisterGlobal)
pnmpi_add_man(PNMPI_Service_GetGlobalByName PNMPI_Global_descriptor_t)
pnmpi_add_man(PNMPI_Service_GetServiceByName "")
pnmpi_add_man(PNMPI_Service_GetServiceByName PNMPI_Service_RegisterService)
pnmpi_add_man(PNMPI_Service_GetServiceByName PNMPI_Service_Fct_t)
pnmpi_add_man(PNMPI_Service_GetServiceByName PNMPI_Service_descriptor_t)
pnmpi_add_man(PNMPI_Service_GetArgument "")
pnmpi_add_man(PNMPI_Service_GetArgument PNMPI_Service_GetArgumentSelf)
pnmpi_add_man(PNMPI_Service_GetPcontrol "")
pnmpi_add_man(PNMPI_Service_GetPcontrol PNMPI_Service_GetPcontrolSelf)
pnmpi_add_man(PNMPI_Service_CallHook "")
pnmpi_add_man(PNMPI_Service_CallHook PNMPI_Service_CallHook_NewStack)
pnmpi_add_man(PNMPI_Service_GetReturnAddress "")
pnmpi_add_man(PNMPI_Service_GetReturnAddress PNMPI_Service_GetFunctionAddress)
pnmpi_add_man(PNMPI_Service_strerror "")
pnmpi_add_man(PNMPI_status_t "")


easy_add_library(pnmpi_service OBJECT
  callHook.c
  changeStack.c
  getArgument.c
  getFunctionAddress.c
  getGlobalByName.c
  getModuleByName.c
  getModuleSelf.c
  getPcontrol.c
  getReturnAddress.c
  getServiceByName.c
  getStackByName.c
  registerGlobal.c
  registerModule.c
  registerService.c
  strerror.c)

easy_target_link_libraries(pnmpi_service ${MPI_C_LIBRARIES})

foreach (target pnmpi_service pnmpi_service_pic)
  add_dependencies(${target} generate)
endforeach ()

add_coverage(pnmpi_service_pic)
add_sanitizers(pnmpi_service_pic)
