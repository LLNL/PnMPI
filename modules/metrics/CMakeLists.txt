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

find_package(C11 OPTIONAL_COMPONENTS ATOMICS)


# Add C11 flags, if C11 is available.
if (C11_FOUND)
  add_definitions(${C11_FLAGS})
endif ()

# Check if atomics are available for thread safe storage. First we try to use
# C11 atomics, next the GCC builtin atomic_fetch_add method and finaly
# sync_fetch_and_add.
if (NOT C11_ATOMICS_FOUND)
  find_package(BuiltinAtomic)
  if (NOT BUILTINATOMIC_FOUND)
    find_package(BuiltinSync)
  endif ()
endif ()

configure_file(config.h.in config.h)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PNMPI_HEADER_DIR}
  ${LLNL-HIRES-TIMERS_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/
  ${PROJECT_BINARY_DIR}/src/pnmpi
  ${PROJECT_SOURCE_DIR}/src/pnmpi
  ${MPI_C_INCLUDE_PATH})


#
# Add the counter module
#
add_wrapped_file(counter.c counter.c.w)

pnmpi_add_xmpi_module(metrics-counter "${CMAKE_CURRENT_BINARY_DIR}/counter.c")
add_coverage(metrics-counter)
add_sanitizers(metrics-counter)


#
# Add the timing module.
#
add_wrapped_file(timing.c timing.c.w)

# Set the RPATH for this module, so it can find LLNL-hires-timers.
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

pnmpi_add_xmpi_module(metrics-timing "${CMAKE_CURRENT_BINARY_DIR}/timing.c")
target_link_libraries(metrics-timing ${LLNL-HIRES-TIMERS_LIBRARY})
add_coverage(metrics-timing)
add_sanitizers(metrics-timing)


install(TARGETS metrics-counter metrics-timing DESTINATION lib/pnmpi-modules)
