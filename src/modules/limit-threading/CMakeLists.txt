# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2019 Lawrence Livermore National Laboratories, United States of America
#  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2019 RWTH Aachen University, Federal Republic of Germany
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

include(CheckMPIFunctionExists)


# Check if MPI_Init_thread is available for C. Otherwise this module can't be
# compiled and will be ignored.
check_mpi_function_exists(MPI_Init_thread HAVE_MPI_INIT_THREAD_C)
if (NOT HAVE_MPI_INIT_THREAD_C)
  return()
endif()

pnmpi_add_xmpi_module(limit-threading limit-threading.c)

pnmpi_add_module_man(limit-threading module_limit_threading)

install(TARGETS limit-threading DESTINATION ${PnMPI_MODULES_DIR})
