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

pnmpi_generate_config(wait-for-debugger.conf "module wait-for-debugger")


# Check if the module prints PID and hostname of the rank.
pnmpi_pnmpize_test(mod_wait-for-debugger_print
  NUMPROCS 1
  PASS_EXPRESSION "Rank [0-9]+ at .* has pid [0-9]+."
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/wait-for-debugger.conf"
  COMMAND $<TARGET_FILE:test-mpi-c>)

# Check if the module waits a given time at startup.
pnmpi_pnmpize_test(mod_wait-for-debugger_wait
  NUMPROCS 1
  PASS_EXPRESSION "Waiting for 5 seconds."
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/wait-for-debugger.conf"
COMMAND $<TARGET_FILE:test-mpi-c>)
set_property(TEST mod_wait-for-debugger_wait APPEND PROPERTY ENVIRONMENT
  "WAIT_AT_STARTUP=5")

# Check if only one module prints the waiting message.
pnmpi_pnmpize_test(mod_wait-for-debugger_waitmessage
  NUMPROCS 2
  FAIL_EXPRESSION "Waiting.*Waiting"
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_BINARY_DIR}/wait-for-debugger.conf"
COMMAND $<TARGET_FILE:test-mpi-c>)
set_property(TEST mod_wait-for-debugger_waitmessage APPEND PROPERTY ENVIRONMENT
  "WAIT_AT_STARTUP=1")
