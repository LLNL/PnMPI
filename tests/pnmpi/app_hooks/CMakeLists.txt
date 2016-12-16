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
)

set(MODS app_startup app_shutdown)
foreach(mod ${MODS})
  add_pnmpi_module(${mod} ${mod}.c)
  add_coverage(${mod})
  add_sanitizers(${mod})
endforeach()




set(TEST_LANGUAGES "c")
if (CMAKE_Fortran_COMPILER_WORKS)
  set(TEST_LANGUAGES "c;fortran")
endif ()

foreach (LANG ${TEST_LANGUAGES})
  # The following test should check, that PnMPI does ignore app_start and
  # app_shutdown, if no module provides these hook.
  pnmpi_pnmpize_test(app_hooks_${LANG}_no_hook
    NUMPROCS 2
    FAIL_EXPRESSION "hook"
    COMMAND $<TARGET_FILE:test-mpi-${LANG}>)

  # Check app_startup will be called.
  pnmpi_generate_config(app_startup.conf "module app_startup")
  pnmpi_pnmpize_test(app_hooks_${LANG}_startup
    NUMPROCS 2
    PASS_EXPRESSION "startup hook"
    PNMPIZE_ARGS
      -c "${CMAKE_CURRENT_BINARY_DIR}/app_startup.conf"
      -m "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND $<TARGET_FILE:test-mpi-${LANG}>)

  # Check app_shutdown will be called.
  pnmpi_generate_config(app_shutdown.conf "module app_shutdown")
  pnmpi_pnmpize_test(app_hooks_${LANG}_shutdown
    NUMPROCS 2
    PASS_EXPRESSION "shutdown hook"
    PNMPIZE_ARGS
      -c "${CMAKE_CURRENT_BINARY_DIR}/app_shutdown.conf"
      -m "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND $<TARGET_FILE:test-mpi-${LANG}>)

  # Check app hooks. No PASS_EXPRESSION will be used, to check the exit status.
  # If any sanitizer or mpiexec fails, the exit status should be non-null.
  pnmpi_generate_config(app_startup.conf
                        "module app_startup"
                        "module app_shutdown")
  pnmpi_pnmpize_test(app_hooks_${LANG}_all
    NUMPROCS 2
    PNMPIZE_ARGS
      -c "${CMAKE_CURRENT_BINARY_DIR}/app_hooks.conf"
      -m "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND $<TARGET_FILE:test-mpi-${LANG}>)
endforeach ()


# Check if the MPI threading level may be adjusted by environment variables.
pnmpi_pnmpize_test(app_hooks_threading_level_env
  NUMPROCS 2
  PASS_EXPRESSION "0"
  PNMPIZE_ARGS
	-c "${CMAKE_CURRENT_BINARY_DIR}/app_startup.conf"
	-m "${CMAKE_CURRENT_BINARY_DIR}"
  COMMAND $<TARGET_FILE:test-mpi-threaded>)
set_property(TEST app_hooks_threading_level_env APPEND PROPERTY ENVIRONMENT
  "PNMPI_THREADING_LEVEL=0")
