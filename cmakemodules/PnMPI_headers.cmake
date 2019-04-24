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

# This CMake module should help to organize PnMPIs header files. It provides
# functions to install private and public header files and variables to find the
# location of these files to pass these directories to include_directories.
#
# All this is done to provide a uniform interface for accessing PnMPIs header
# files, which are spread all over the projects directories as if they were
# installed at the platform if PnMPI is installed. C files may access them then
# as #include <pnmpi/core.h> instead of searching the actual location of the
# core.h header file.

include(GNUInstallDirs)


# First we have to define a temporary location for all header files to copy them
# to this location, so all targets may simply include these path.
set(PNMPI_HEADER_DIR "${PROJECT_BINARY_DIR}/src/pnmpi/header")


# Add a public header file.
#
# This function puts a copy of FILE into PNMPI_HEADER_DIR (into the pnmpi
# subdirectory to make #include <pnmpi/...> possible) and adds it to the install
# list.
function (pnmpi_add_header FILE)
  if (ARGV1)
    set(NAME ${ARGV1})
  else ()
    get_filename_component("NAME" ${FILE} NAME)
  endif ()

  configure_file(${FILE} "${PNMPI_HEADER_DIR}/pnmpi/${NAME}")

  install(FILES "${PNMPI_HEADER_DIR}/pnmpi/${NAME}"
          DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/pnmpi")
endfunction ()


# Add a private header file.
#
# This function does the same except the file is placed in pnmpi/private instead
# of pnmpi and the file will not be installed.
function (pnmpi_add_private_header FILE)
  if (ARGV1)
    set(NAME ${ARGV1})
  else ()
    get_filename_component("NAME" ${FILE} NAME)
  endif ()

  configure_file(${FILE} "${PNMPI_HEADER_DIR}/pnmpi/private/${NAME}")
endfunction ()
