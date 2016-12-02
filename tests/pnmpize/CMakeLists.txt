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
find_package(Sanitizers)


# Helper function to add a new PnMPIze test case.
function (pnmpi_pnmpize_test NAME)
  # Parse options.
  set(oneValueArgs NUMPROCS PASS_EXPRESSION FAIL_EXPRESSION)
  set(multiValueArgs PNMPIZE_ARGS COMMAND)
  cmake_parse_arguments("OPT" "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})


  # If AddressSanitizer is enabled, we have to use the asan wrapper, because
  # PnMPIze uses preloading.
  set(SANITIZE_WRAPPER "")
  if (SANITIZE_ADDRESS)
    set(SANITIZE_WRAPPER ${ASan_WRAPPER})
  endif ()


  # Handle non-MPI tests
  if ((NOT OPT_NUMPROCS) OR (OPT_NUMPROCS EQUAL 0))
    pnmpi_test(${NAME}
      PASS_EXPRESSION "${OPT_PASS_EXPRESSION}"
      FAIL_EXPRESSION "${OPT_FAIL_EXPRESSION}"
      COMMAND ${SANITIZE_WRAPPER}
      $<TARGET_FILE:pnmpize> ${OPT_PNMPIZE_ARGS} ${OPT_COMMAND})

  # Handle MPI tests.
  else ()
    pnmpi_mpi_test(${NAME}
      PASS_EXPRESSION "${OPT_PASS_EXPRESSION}"
      FAIL_EXPRESSION "${OPT_FAIL_EXPRESSION}"
      COMMAND ${MPIEXEC}
        ${MPIEXEC_NUMPROC_FLAG} ${OPT_NUMPROCS}
        ${MPIEXEC_PREFLAGS} ${SANITIZE_WRAPPER} $<TARGET_FILE:pnmpize>
        ${OPT_PNMPIZE_ARGS} ${OPT_COMMAND} ${MPIEXEC_POSTFLAGS})
  endif ()
endfunction ()




# The following test should not check the whole functionality of PnMPIze, but
# should check, if PnMPIze is able to execute a application. A second purpose
# for this test is to check, if PnMPIze (or PnMPI) has memory leaks, what can't
# be checked with MPI applications due memory leaks in some MPI libraries.
pnmpi_pnmpize_test(pnmpize_basic
  COMMAND $<TARGET_FILE:test-nompi>)

# The following test should not check the whole functionality of PnMPIze, but
# should check, if PnMPIze is able to execute a application after PnMPIze was
# invoked via mpiexec.
pnmpi_pnmpize_test(pnmpize_basic_mpiexec
  NUMPROCS 1
  COMMAND $<TARGET_FILE:test-nompi>)

# The following test checks, if PnMPIze starts PnMPI. If PnMPI header is not
# found in stdout, the test will fail.
pnmpi_pnmpize_test(pnmpize_preload_pnmpi
  NUMPROCS 1
  PASS_EXPRESSION "Number of modules:"
  COMMAND $<TARGET_FILE:test-mpi-c>)

# The following test checks, if PnMPIze is able to pass a configuration file to
# PnMPI. The sample module will be used for this check and should the output
# "WRAPPER 1: Before send" to stdout.
pnmpi_pnmpize_test(pnmpize_set_config
  NUMPROCS 2
  PASS_EXPRESSION "Wrapper: Sending"
  PNMPIZE_ARGS -c "${TEST_MODULE_CONF}"
  COMMAND $<TARGET_FILE:test-mpi-c>)


# Check Fortran functionality.
#
# If Fortran is not supported by PnMPI, return this script now to ignore the
# fortran checks.
if (NOT CMAKE_Fortran_COMPILER_WORKS)
  return()
endif ()

# The following test checks, if PnMPIze starts PnMPI. If PnMPI header is not
# found in stdout, the test will fail.
pnmpi_pnmpize_test(pnmpize_fortran_preload_pnmpi
  NUMPROCS 2
  PASS_EXPRESSION "Number of modules:"
  COMMAND $<TARGET_FILE:test-mpi-fortran>)

# The following test checks, if PnMPIze is able to pass a configuration file to
# PnMPI. The sample module will be used for this check and should the output
# "WRAPPER 1: Before send" to stdout.
pnmpi_pnmpize_test(pnmpize_fortran_set_config
  NUMPROCS 2
  PASS_EXPRESSION "Wrapper: Sending"
  PNMPIZE_ARGS -c "${TEST_MODULE_CONF}"
  COMMAND $<TARGET_FILE:test-mpi-fortran>)
