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


# The following executable may be used to check the general functionality of a
# test case. It does not call any MPI functions, so it is useful for checking
# non-MPI functionality like the PnMPI startup. Some memory leaks may be tracked
# with this executable, because every application calling MPI will definitly
# have memory leaks (caused by some MPI implementations).
add_executable("test-nompi" test-nompi.c)
add_coverage(test-nompi)
add_sanitizers(test-nompi)
pnmpi_test("binary_nompi" COMMAND $<TARGET_FILE:test-nompi>)

# The following target may be used to check general MPI functionality. It uses
# the common MPI functions MPI_Init, MPI_Finalize, MPI_Send and MPI_Recv and may
# be used with at least 2 ranks.
add_executable("test-mpi-c" test-mpi.c)
target_include_directories(test-mpi-c PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(test-mpi-c ${MPI_C_LIBRARIES})
add_coverage(test-mpi-c)
add_sanitizers(test-mpi-c)
pnmpi_mpi_test("binary_mpi_c" COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
  $<TARGET_FILE:test-mpi-c>)

# The following target may be used to check general MPI functionality. It uses
# the common MPI functions MPI_Init_thread, MPI_Finalize, MPI_Send and MPI_Recv
# and may be used with at least 2 ranks.
add_executable(test-mpi-threaded test-mpi-threaded.c)
target_include_directories(test-mpi-threaded PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(test-mpi-threaded ${MPI_C_LIBRARIES})
add_coverage(test-mpi-threaded)
add_sanitizers(test-mpi-threaded)
pnmpi_mpi_test("binary_mpi_threaded" COMMAND ${MPIEXEC}
  ${MPIEXEC_NUMPROC_FLAG} 2 $<TARGET_FILE:test-mpi-threaded>)

# The following target may be used to check general MPI functionality with
# Fortran code. It uses the common MPI functions MPI_Init, MPI_Finalize,
# MPI_Send and MPI_Recv and may be used with at least 2 ranks.
if (CMAKE_Fortran_COMPILER_WORKS)
  add_executable("test-mpi-fortran" test-mpi.f)
  target_include_directories(test-mpi-fortran
    PRIVATE ${MPI_Fortran_INCLUDE_PATH})
  target_link_libraries(test-mpi-fortran ${MPI_Fortran_LIBRARIES})
  add_coverage(test-mpi-fortran)
  add_sanitizers(test-mpi-fortran)
  pnmpi_mpi_test("binary_mpi_fortran" COMMAND
    ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 $<TARGET_FILE:test-mpi-fortran>)
endif ()


# The following target may be used as a PnMPI module for test cases. It will
# print messages before MPI_Send and after MPI_Recv.
add_pnmpi_module(test-module module.c)
target_include_directories(test-module PRIVATE
  ${MPI_C_INCLUDE_PATH}
  ${PROJECT_BINARY_DIR})

set(TEST_MODULE_CONF "${CMAKE_CURRENT_SOURCE_DIR}/module.conf" CACHE INTERNAL
  "Path to test-module PnMPI config file")
