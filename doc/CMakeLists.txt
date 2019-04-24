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

include(GNUInstallDirs)
include(PnMPI_doc)


## \brief Convert booleans for Doxygen config.
#
# \details This macro converts CMake booleans to the accepted values of Doxygen
#  `YES` and `NO`.
#
#
# \param VAR Output variable name, will be prefixed by `DOXYGEN_`.
# \param INPUT Name of the variable to be converted.
#
macro(doxygen_convert_bool VAR INPUT)
  set("DOXYGEN_${VAR}" "NO")
  if (${INPUT})
    set("DOXYGEN_${VAR}" "YES")
  endif ()
endmacro()


# Options
#
option("BUILD_DOC" "Build documentation." Off)
if (BUILD_DOC)
  option("BUILD_DOC_INTERNAL" "Include private documentation." Off)
  option("BUILD_DOC_HTML" "Generate a HTML documentation." On)
endif ()


# Generate Doxygen docs.
#
if (BUILD_DOC)
  find_package(Doxygen REQUIRED)

  # Generate the Doxygen configuration file. Some variables need to be converted
  # before their contents can be substituted into the file, as Doxygen doesn't
  # understand the default CMake booleans and empty values.
  doxygen_convert_bool(DOC_INTERNAL BUILD_DOC_INTERNAL)
  doxygen_convert_bool(DOC_HTML BUILD_DOC_HTML)
  if (NOT DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_DOT_EXECUTABLE "")
  endif ()

  set(PNMPI_DOXYFILE "${PROJECT_BINARY_DIR}/doc/doxygen.conf")
  configure_file(doxygen.conf.in ${PNMPI_DOXYFILE} @ONLY)


  # Generate a list of dependencies for Doxygen. This list is required, so the
  # documentation will be rebuild, when a target needs to be rebuild, too. The
  # list of dependencies depends on the current configuration of PnMPI.
  set(DOXYGEN_DEPENDENCIES pnmpi pnmpi-modules)
  if (ENABLE_PNMPIZE)
    list(APPEND DOXYGEN_DEPENDENCIES pnmpize)
  endif ()
  if (ENABLE_MODULES)
    list(APPEND DOXYGEN_DEPENDENCIES
         "${PROJECT_SOURCE_DIR}/src/modules/metrics/doc.c")
  endif ()

  # Generate the documentation with Doxygen. The output file will be a main
  # man page, as man pages will be build in any case, but HTML docs may be
  # disabled.
  set(DOXYGEN_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/man/man3/pnmpi.h.3")
  add_custom_command(OUTPUT ${DOXYGEN_OUTPUT_FILE}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${PNMPI_DOXYFILE}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     MAIN_DEPENDENCY ${PNMPI_DOXYFILE}
                     DEPENDS ${DOXYGEN_DEPENDENCIES}
                     COMMENT "Generating documentation")
  add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUTPUT_FILE})


  if (BUILD_DOC_HTML)
    # Install HTML documentation only, if the user wishes to install it, as it
    # will be used by internal developers most of the time and doesn't need to
    # be installed most of the time.
    option("INSTALL_DOC_HTML" "Install the generated HTML documentation." Off)
    if (INSTALL_DOC_HTML)
      install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
              DESTINATION ${CMAKE_INSTALL_DOCDIR})
    endif ()
  endif ()
endif ()
