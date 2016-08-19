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


# Helper function to add a new test case.
function (pnmpi_test NAME)
  # Parse options.
  set(oneValueArgs PASS_EXPRESSION FAIL_EXPRESSION)
  set(multiValueArgs COMMAND)
  cmake_parse_arguments("OPT" "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Add test case with optional pass or fail regular expression.
  add_test(NAME ${NAME} COMMAND ${OPT_COMMAND})
  if (OPT_PASS_EXPRESSION)
    set_tests_properties(${NAME} PROPERTIES PASS_REGULAR_EXPRESSION
      "${OPT_PASS_EXPRESSION}")
  else ()
    if (NOT OPT_FAIL_EXPRESSION)
      set(OPT_FAIL_EXPRESSION "Error;ERROR;Failed")
    endif ()

    set_tests_properties(${NAME} PROPERTIES FAIL_REGULAR_EXPRESSION
      "${OPT_FAIL_EXPRESSION}")
  endif ()

  # Append the 'src' directory as PNMPI_LIB_PATH to get access to the test
  # module for all test cases.
  set_property(TEST ${NAME} APPEND PROPERTY ENVIRONMENT
    "PNMPI_LIB_PATH=${CMAKE_BINARY_DIR}/tests/src")
endfunction ()


# Helper function to add a new MPI test case. In addition to the above helper
# function, this function additionaly sets an ENV variable to disable leak
# checks, if AddressSanitizer is enabled, because some MPI librarys have memory
# leaks.
function (pnmpi_mpi_test NAME)
  # Add a new test case.
  pnmpi_test(${ARGV})

  # If AddressSanitizer is enabled, disable leak sanitizer.
  if (SANITIZE_ADDRESS)
    set_property(TEST ${NAME} APPEND PROPERTY ENVIRONMENT
      "ASAN_OPTIONS=detect_leaks=0")
  endif ()
endfunction ()


# Recurse into subdirectories.
add_subdirectory(src)
add_subdirectory(pnmpize)
add_subdirectory(pnmpi)
