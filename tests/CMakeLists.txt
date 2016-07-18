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

find_package(Sanitizers)
find_package(MPI REQUIRED)


# Helper function to add a new test case.
function (pnmpi_test NAME PASS_EXPRESSION COMMAND)
  # Remove ${NAME} from ${ARGV} to use ${ARGV} as ${COMMAND}.
  list(REMOVE_AT ARGV 0 1)

  # Add test case with optional pass regular expression.
  add_test(NAME ${NAME} COMMAND ${ARGV})
  if (PASS_EXPRESSION)
    set_tests_properties(${NAME} PROPERTIES PASS_REGULAR_EXPRESSION
      "${PASS_EXPRESSION}")
  endif ()

  # If PASS_EXPRESSION is set, the following FAIL_REGULAR_EXPRESSION will have
  # no effect.
  set_tests_properties(${NAME} PROPERTIES FAIL_REGULAR_EXPRESSION
    "Error;ERROR;Failed")
endfunction ()


# Helper function to add a new MPI test case. In addition to the above helper
# function, this function additionaly sets an ENV variable to disable leak
# checks, if AddressSanitizer is enabled, because some MPI librarys have memory
# leaks.
function (pnmpi_mpi_test NAME PASS_EXPRESSION COMMAND)
  # Remove ${NAME} from ${ARGV} to use ${ARGV} as ${COMMAND}.
  list(REMOVE_AT ARGV 0 1)
  pnmpi_test("${NAME}" "${PASS_EXPRESSION}" ${ARGV})

  if (SANITIZE_ADDRESS)
    set_tests_properties(${NAME} PROPERTIES
      ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
  endif ()
endfunction ()


# The following executable may be used to check the general functionality of a
# test case. It does not call any MPI functions, so it is useful for checking
# non-MPI functionality like the PnMPI startup. Some memory leaks may be tracked
# with this executable, because every application calling MPI will definitly
# have memory leaks (caused by some MPI implementations).
add_executable("test-nompi" test-nompi.c)
add_coverage(test-nompi)
add_sanitizers(test-nompi)

# The following target may be used to check general MPI functionality. It uses
# the common MPI functions MPI_Init, MPI_Finalize, MPI_Send and MPI_Recv and may
# be used with at least 2 ranks.
add_executable("test-mpi-c" test-mpi.c)
target_include_directories(test-mpi-c PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(test-mpi-c ${MPI_C_LIBRARIES})
add_coverage(test-mpi-c)
add_sanitizers(test-mpi-c)

# The following target may be used to check general MPI functionality with
# Fortran code. It uses the common MPI functions MPI_Init, MPI_Finalize,
# MPI_Send and MPI_Recv and may be used with at least 2 ranks.
if (CMAKE_Fortran_COMPILER_WORKS)
  add_executable("test-mpi-f" test-mpi.f)
  target_include_directories(test-mpi-f PRIVATE ${MPI_Fortran_INCLUDE_PATH})
  target_link_libraries(test-mpi-f ${MPI_Fortran_LIBRARIES})
  add_coverage(test-mpi-f)
  add_sanitizers(test-mpi-f)
endif ()


# Recurse into subdirectories.
add_subdirectory(pnmpize)
