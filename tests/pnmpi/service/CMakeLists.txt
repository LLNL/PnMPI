# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2016 Lawrence Livermore National Laboratories, United States of America
#  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

include(PnMPI_headers)

find_package(codecov)
find_package(Sanitizers)


include_directories(
  ${MPI_C_INCLUDE_PATH}
  ${PROJECT_BINARY_DIR}/src/pnmpi
  ${PNMPI_HEADER_DIR})

set(MODS getModuleSelf getModuleByName getStackByName register)
foreach(mod ${MODS})
  pnmpi_add_xmpi_module(${mod} ${mod}.c)
  add_coverage(${mod})
  add_sanitizers(${mod})
endforeach()


# Check the module registration.
pnmpi_generate_config(registerModule.conf "module register")
pnmpi_pnmpize_test(service_registerModule
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/registerModule.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "registered as: registerTest"
  COMMAND $<TARGET_FILE:test-mpi-c>)


# Check the correctness of getting module handles.
pnmpi_generate_config(getModuleSelf.conf
                      "module empty"
                      "module getModuleSelf")
pnmpi_pnmpize_test(service_self_getModule
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/getModuleSelf.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "My module ID: 1"
  COMMAND $<TARGET_FILE:test-mpi-c>)

# Check to get a module handly ba name.
pnmpi_generate_config(getModuleByName.conf
                      "module empty"
                      "module getModuleByName")

pnmpi_pnmpize_test(service_getModuleByName_found
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/getModuleByName.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "GetModuleByName: 1"
  COMMAND $<TARGET_FILE:test-mpi-c>)
set_property(TEST service_getModuleByName_found APPEND PROPERTY ENVIRONMENT
  "PNMPI_TEST_MODNAME=testmodule")

pnmpi_pnmpize_test(service_getModuleByName_not_found
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/getModuleByName.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "GetModuleByName: not found"
  COMMAND $<TARGET_FILE:test-mpi-c>)
set_property(TEST service_getModuleByName_not_found APPEND PROPERTY ENVIRONMENT
  "PNMPI_TEST_MODNAME=testmodule-notfound")


# Check to get stack handles.
pnmpi_generate_config(getStackByName.conf
                      "module getStackByName"
                      "stack sample"
                      "module empty")

pnmpi_pnmpize_test(service_getStackByName_found
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/getStackByName.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "GetStackByName: 2"
  COMMAND $<TARGET_FILE:test-mpi-c>)
set_property(TEST service_getStackByName_found APPEND PROPERTY ENVIRONMENT
  "PNMPI_TEST_STACKNAME=sample")

pnmpi_pnmpize_test(service_getStackByName_not_found
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/getStackByName.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "GetStackByName: not found"
  COMMAND $<TARGET_FILE:test-mpi-c>)
set_property(TEST service_getStackByName_not_found APPEND PROPERTY ENVIRONMENT
  "PNMPI_TEST_STACKNAME=unknown")
