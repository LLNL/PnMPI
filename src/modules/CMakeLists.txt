# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2017 Lawrence Livermore National Laboratories, United States of America
#  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2017 RWTH Aachen University, Federal Republic of Germany
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

# Modules will be built by default. However, one may disable building the
# modules, e.g. if only the PnMPI infrastructure is required.
option(ENABLE_MODULES "Selects whether PnMPI modules are built." TRUE)
if (NOT ENABLE_MODULES)
  return()
endif ()


include(PnMPI_headers)
include(PnMPI_modules)
include(GNUInstallDirs)


# Add a custom target to bundle all module targets. This target may be either
# used by the user, but will be used especially for building the Doxygen
# documentation, so the documentation will be rebuild, if any module changes.
add_custom_target(pnmpi-modules)

# Wrap calls to pnmpi_add_xmpi_module to add the module's target to the global
# pnmpi-modules target. This can't be done directly in the original function, as
# external projects and the testcases don't require this behaviour.
#
# Note: The PMPI version of this macro doesn't need to be patched, as it calls
#       this macro internaly.
macro(pnmpi_add_xmpi_module TARGET)
  # Call the original macro to add a new target for the module to be built and
  # add it to the global pnmpi-modules target.
  _pnmpi_add_xmpi_module(${ARGV})
  add_dependencies(pnmpi-modules ${TARGET})

  # Add code coverage and sanitizers for the module. These can be enabled on-
  # demand at configuration time.
  add_coverage(${TARGET})
  add_sanitizers(${TARGET})
endmacro()


# Use a common path where the PnMPI modules get installed. Modules should use
# this variable instead of hardcoding a path. Similar modules may be grouped
# in subdirectories.
set(PnMPI_MODULES_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/pnmpi-modules"
                      CACHE PATH "PnMPI module install path")


# Include directories for all modules.
include_directories(
  ${MPI_C_INCLUDE_PATH}
  ${PNMPI_HEADER_DIR}
)


# Recurse to sub-directories
add_subdirectory(comm)
add_subdirectory(datatype)
add_subdirectory(empty)
add_subdirectory(limit-threading)
add_subdirectory(metrics)
add_subdirectory(requests)
add_subdirectory(sample)
add_subdirectory(status)
add_subdirectory(switch-matrix)
add_subdirectory(timelapse)
add_subdirectory(virtual)
add_subdirectory(wait-for-debugger)
