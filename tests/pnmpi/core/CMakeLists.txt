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

find_package(MPI REQUIRED)


#
# PnMPI header.
#

# No modules loaded.
pnmpi_pnmpize_test(pnmpi_header_no_modules
  PASS_EXPRESSION "No modules loaded."
  COMMAND $<TARGET_FILE:test-mpi-c>)

# One module loaded.
pnmpi_generate_config(one_module.conf "module test-module")
pnmpi_pnmpize_test(pnmpi_header_one_module
  PASS_EXPRESSION
    "Loaded modules:.*Stack default:.*test-module \\(Pcontrol: 1\\)"
  PNMPIZE_ARGS -c "${CMAKE_CURRENT_BINARY_DIR}/one_module.conf"
  COMMAND $<TARGET_FILE:test-mpi-c>)

# Header with stacks.
pnmpi_generate_config(stack.conf
                      "module empty"
                      "stack sample"
                      "module empty")
pnmpi_pnmpize_test(pnmpi_header_stacks
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/stack.conf"
  PASS_EXPRESSION "Loaded modules:.*Stack default:.*Stack sample"
  COMMAND $<TARGET_FILE:test-mpi-c>)


#
# Threading-Downgrade test.
#
add_pnmpi_module(threading_downgrade threading_downgrade.c)
target_include_directories(threading_downgrade PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(threading_downgrade ${MPI_C_LIBRARIES})
add_coverage(threading_downgrade)
add_sanitizers(threading_downgrade)

pnmpi_generate_config(threading_downgrade.conf "module threading_downgrade")
pnmpi_pnmpize_test(pnmpi_threading_downgrade
  NUMPROCS 2
  PASS_EXPRESSION "provide a maximum of 0."
  PNMPIZE_ARGS
	-c "${CMAKE_CURRENT_BINARY_DIR}/threading_downgrade.conf"
	-m "${CMAKE_CURRENT_BINARY_DIR}"
COMMAND $<TARGET_FILE:test-mpi-threaded>)
