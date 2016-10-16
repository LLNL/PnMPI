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

include(CheckCSourceCompiles)
include(PnMPI_headers)

find_package(C11 COMPONENTS atomics)


# Check if atomics are available for the counter module, so it will be thread-
# safe. First we try to use C11 atomics, next the GCC builtin sync_fetch_and_add
# method.
if (C11_FOUND)
  set(HAVE_C11_ATOMICS ${C11_atomics_FOUND})
  add_definitions(${C11_FLAGS})
else ()
  check_c_source_compiles("
    int main ()
    {
      int i;
      __sync_fetch_and_add(&i, 0);
    }
    " HAVE_SYNC_FETCH_AND_ADD)
endif ()

configure_file(config.h.in config.h)


include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PNMPI_HEADER_DIR}
  ${PROJECT_BINARY_DIR}/
  ${PROJECT_BINARY_DIR}/src/pnmpi
  ${PROJECT_SOURCE_DIR}/src/pnmpi
  ${MPI_C_INCLUDE_PATH})

add_wrapped_file(counter.c counter.c.w)

add_pnmpi_module(counter "${CMAKE_CURRENT_BINARY_DIR}/counter.c")
add_coverage(counter)
add_sanitizers(counter)

install(TARGETS counter DESTINATION lib/pnmpi-modules)
