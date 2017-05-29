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

include(GNUInstallDirs)
include(PnMPI_doc)


#
# Options
#
option("BUILD_DOC" "Build HTML documentation" Off)
if (BUILD_DOC)
  option("BUILD_DOC_INTERNAL" "Including private documentation." Off)
endif ()

#
# Generate Doxygen docs.
#
if (BUILD_DOC)
  find_package(Doxygen REQUIRED)

  if (NOT DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_DOT_EXECUTABLE "")
  endif ()

  set(DOXYGEN_DOC_INTERNAL "NO")
  if (BUILD_DOC_INTERNAL)
    set(DOXYGEN_DOC_INTERNAL "YES")
  endif ()

  set(PNMPI_DOXYFILE "${PROJECT_BINARY_DIR}/doc/doxygen.conf")
  configure_file(doxygen.conf.in ${PNMPI_DOXYFILE} @ONLY)

  set(DOXYGEN_HTML_INDEX "${CMAKE_CURRENT_BINARY_DIR}/html/index.html")


  add_custom_command(OUTPUT ${DOXYGEN_HTML_INDEX}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${PNMPI_DOXYFILE}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     MAIN_DEPENDENCY ${PNMPI_DOXYFILE}
                     DEPENDS
                       pnmpi
                       pnmpize
                       commsub-empty commsub-print
                       empty
                       metrics-counter metrics-timing
                         ${PROJECT_SOURCE_DIR}/modules/metrics/doc.c
                       requests
                       sample1 sample2 sample3 sample4
                       status
                       switch-matrix
                       timelapse
                       virtual
                       wait-for-debugger
                     COMMENT "Generating documentation")
  add_custom_target(doc ALL DEPENDS ${DOXYGEN_HTML_INDEX})


  # install documentation
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
          DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif ()
