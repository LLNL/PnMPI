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

easy_add_test(PREFIX "service_registerModule" SOURCES registerModule.c)
easy_add_test(PREFIX "service_getModuleSelf" SOURCES getModuleSelf.c)
easy_add_test(PREFIX "service_getModuleByName" SOURCES getModuleByName.c)

easy_add_test(PREFIX "service_getStackByName" SOURCES getStackByName.c)

easy_add_test(PREFIX "service_registerGlobal" SOURCES registerGlobal.c)
easy_add_test(PREFIX "service_getGlobalByName" SOURCES getGlobalByName.c)

easy_add_test(PREFIX "service_registerService" SOURCES registerService.c)
easy_add_test(PREFIX "service_getServiceByName" SOURCES getServiceByName.c)

easy_add_test(PREFIX "service_getArgument" SOURCES getArgument.c)
easy_add_test(PREFIX "service_getArgumentSelf" SOURCES getArgumentSelf.c)

easy_add_test(PREFIX "service_getPcontrol" SOURCES getPcontrol.c)
easy_add_test(PREFIX "service_getPcontrolSelf" SOURCES getPcontrolSelf.c)

easy_add_test(PREFIX "service_callHook" SOURCES callHook.c)

if (HAVE_BUILTIN_RETURN_ADDRESS)
  easy_add_test(PREFIX "service_getReturnAddress"
                SOURCES getReturnAddress.c
                CONFIGS C_Init C_Send)
  if (ENABLE_FORTRAN)
    easy_add_test(PREFIX "service_getReturnAddress"
                  SOURCES getReturnAddress.c
                  CONFIGS Fortran_Init Fortran_Send)
  endif ()
endif ()
