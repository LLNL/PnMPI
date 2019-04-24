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

# PnMPIze should be build by default. However, some platforms (like BlueGene/Q)
# do not support switching the process with system calls like 'execvp', so the
# PnMPIze tool can't be used at these platforms and may be disabled.
option(ENABLE_PNMPIZE "Build the PnMPI invocation tool." on)
if (NOT ENABLE_PNMPIZE)
  return()
endif ()


include(GNUInstallDirs)

find_package(argp REQUIRED)
find_package(help2man)
find_package(C99)

if (C99_FOUND)
  add_definitions(${C99_FLAGS})
endif ()

# Set the PnMPI search paths for mac OS.
#
# DYLD_INSERT_LIBRARIES works with paths only, so PnMPIze will search for the
# PnMPI library to preload. By default these path's are the following ones, but
# the install path should be appended to these.
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(PNMPIZE_SEARCH_PATHS
    ${CMAKE_INSTALL_FULL_LIBDIR}
    "/usr/local/lib"
    "/lib"
    "/usr/lib")

  # If PnMPI is installed in a default install directory, this duplicated path
  # should be stripped from the list.
  list(REMOVE_DUPLICATES PNMPIZE_SEARCH_PATHS)

  # Convert the list to a search-path list.
  string(REPLACE ";" ":" PNMPIZE_SEARCH_PATHS "${PNMPIZE_SEARCH_PATHS}")
endif ()

configure_file(config.h.in config.h)


# Include all required directories.
include_directories(${CMAKE_CURRENT_BINARY_DIR})


# Build and install PnMPIze.
add_executable(pnmpize pnmpize.c)
add_coverage(pnmpize)
add_sanitizers(pnmpize)

target_link_libraries(pnmpize ${ARGP_LIBRARIES})
set_target_properties(pnmpize PROPERTIES OUTPUT_NAME pnmpi)

install(TARGETS pnmpize EXPORT pnmpize DESTINATION ${CMAKE_INSTALL_BINDIR})
install(EXPORT pnmpize DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/PnMPI)

# If help2man could be found, generate a man page for PnMPIze. The help2man
# command will install this man page, too.
if (HELP2MAN_FOUND)
  help2man(pnmpize-man TARGET pnmpize NOINFO INSTALL RENAME pnmpi.1)
endif()
