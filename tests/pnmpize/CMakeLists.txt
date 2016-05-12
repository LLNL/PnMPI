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


# Store the path of PnMPIze in a variable. If AddressSanitizer is enabled,
# prepend the asan-wrapper, so that P^nMPI can be sanitized.
set(PNMPIZE_BIN $<TARGET_FILE:pnmpize>)
if (SANITIZE_ADDRESS)
  set(PNMPIZE_BIN ${ASan_WRAPPER} ${PNMPIZE_BIN})
endif ()


# The following test should not check the whole functionality of PnMPIze, but
# should check, if PnMPIze is able to execute a application. A second purpose
# for this test is to check, if PnMPIze (or PnMPI) has memory leaks, what can't
# be checked with MPI applications due memory leaks in some MPI libraries.
pnmpi_test(pnmpize_basic "" ${PNMPIZE_BIN} $<TARGET_FILE:test-nompi>)

# The following test checks, if pnmpize starts PnMPI. If PnMPI header is not
# found in stdout, the test will fail.
pnmpi_mpi_test(pnmpize_preload_pnmpi "Number of modules:"
  ${PNMPIZE_BIN} $<TARGET_FILE:simple>)
