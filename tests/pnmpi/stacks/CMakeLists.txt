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
find_package(MPI REQUIRED)
find_package(Sanitizers)


# Add test modules for app_start and app_shutdown hooks. These modules will not
# be installed, but used for the test cases below.

include_directories(
  ${MPI_C_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/pnmpi
  ${PROJECT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/src/pnmpi
  ${PNMPI_HEADER_DIR})

set(MODS stack)
foreach(mod ${MODS})
  add_pnmpi_module(${mod} ${mod}.c)
  add_coverage(${mod})
  add_sanitizers(${mod})
endforeach()


# Check the module.
pnmpi_generate_config(stack.conf
                      "module stack"
                      "stack sample"
                      "module empty")
pnmpi_pnmpize_test(stack
  NUMPROCS 2
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/stack.conf"
    -m "${CMAKE_CURRENT_BINARY_DIR}"
  PASS_EXPRESSION "Switching the stack."
  COMMAND $<TARGET_FILE:test-mpi-c>)
