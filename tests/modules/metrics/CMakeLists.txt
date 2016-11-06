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

# Check if counter module counts the MPI calls.
pnmpi_pnmpize_test(mod_metrics_counter
  NUMPROCS 2
  PASS_EXPRESSION "Stats:\nRank 0:\n.*1 MPI_Init.*Total:.*2 MPI_Init"
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_SOURCE_DIR}/counter.conf"
  COMMAND $<TARGET_FILE:test-mpi-c>)


# Check if timing module measures the time of the MPI calls.
pnmpi_pnmpize_test(mod_metrics_timing
  NUMPROCS 2
  PASS_EXPRESSION "Timing stats:\nRank 0:\n.*MPI_Init\n.*Total:.*MPI_Init"
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_SOURCE_DIR}/timing.conf"
  COMMAND $<TARGET_FILE:test-mpi-c>)

# Check if timing module measures the time of the MPI calls.
pnmpi_pnmpize_test(mod_metrics_timing_pcontrol
  PASS_EXPRESSION
    "Timing stats:\nRank 0:\n.*MPI_Pcontrol\n.*Total:.*MPI_Pcontrol"
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_SOURCE_DIR}/timing-pcontrol-valid.conf"
COMMAND $<TARGET_FILE:test-mpi-pcontrol>)

# Check if timing module throws an error, if Pcontrol is measured with wrong
# configuration.
pnmpi_pnmpize_test(mod_metrics_timing_pcontrol_invalid
  PASS_EXPRESSION
    "metric-timing can measure the time of MPI_Pcontrol in advanced mode, only."
  PNMPIZE_ARGS
    -c "${CMAKE_CURRENT_SOURCE_DIR}/timing-pcontrol-invalid.conf"
COMMAND $<TARGET_FILE:test-mpi-pcontrol>)
