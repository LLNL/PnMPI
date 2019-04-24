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

## \brief Add a native PnMPI module with XMPI calls.
#
# \details This function adds a new module \p targetname. The module will not be
#  patched, as the module itself uses XMPI calls.
#
function (pnmpi_add_xmpi_module targetname)
  if (ARGC LESS 1)
    message(FATAL_ERROR "add_pnmpi_xmpi_module() called with no source files!")
  endif ()


  add_library(${targetname} MODULE ${ARGN})
  set_target_properties(${targetname} PROPERTIES PREFIX "")

  # For Apple set that undefined symbols should be looked up dynamically (On
  # linux this is already the default).
  if(APPLE)
    set_target_properties(${targetname} PROPERTIES
                          LINK_FLAGS "-undefined dynamic_lookup")
  endif()

  add_dependencies(${targetname} pnmpi)
endfunction ()


## \brief Add a PMPI PnMPI module.
#
# \details This function adds a new module \p targetname. The module will be
#  patched, so PMPI calls become XMPI calls.
#
function (pnmpi_add_pmpi_module targetname)
  if (ARGC LESS 1)
    message(FATAL_ERROR "add_pnmpi_pmpi_module() called with no source files!")
  endif ()

  find_package(UnixCommands REQUIRED)

  pnmpi_add_xmpi_module(${targetname} ${ARGN})

  # Patch the library in place once it's built so that it can be installed
  # normally, like any other library. Also keeps build dir libraries consistent
  # with installed libs.
  set(tmplib ${targetname}-unpatched.so)

  add_custom_command(TARGET ${targetname} POST_BUILD
    COMMAND ${MV} ARGS $<TARGET_FILE:${targetname}> ${tmplib}
    COMMAND $<TARGET_FILE:pnmpi-patch>
      ARGS ${tmplib} $<TARGET_FILE:${targetname}>
    COMMAND ${RM} ARGS -f ${tmplib}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Patching ${targetname}"
    VERBATIM)

  add_dependencies(${targetname} pnmpi-patch)
endfunction ()
