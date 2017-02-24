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


set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in")
set(doxy_output_root "${CMAKE_CURRENT_BINARY_DIR}")
set(doxy_html_index_file "${doxy_output_root}/html/index.html")

if (BUILD_DOC OR BUILD_DOC_INTERNAL)
  find_package(Doxygen REQUIRED)

  if (NOT DOXYGEN_DOT_EXECUTABLE)
    set(DOXYGEN_DOT_EXECUTABLE "")
  endif ()

  set(DOXYGEN_DOC_INTERNAL "NO")
  if (BUILD_DOC_INTERNAL)
    set(DOXYGEN_DOC_INTERNAL "YES")
  endif ()

  configure_file(${doxyfile_in} ${PNMPI_DOXYFILE} @ONLY)

  pnmpi_doxygen(${doxy_html_index_file} "Generating documentation")
  add_custom_target(doc ALL DEPENDS ${doxy_html_index_file})


  # install documentation
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif ()
