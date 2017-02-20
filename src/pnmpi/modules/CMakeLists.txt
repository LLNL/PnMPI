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

include(PnMPI_doc)
include(PnMPI_headers)


# Install and manage the public and private header files of the PnMPI sources.
# This has to be done to use the same headers for the PnMPI sources and the API
# without getting conflicts about the used paths. Otherwise you'd have to take
# care about file locations after installation, especially if you use the
# headers over multiple directories.
pnmpi_add_private_header(modules.h)
pnmpi_add_header(hooks.h)

# Install the public man pages for this object.
pnmpi_add_man(pnmpi_module_hooks PNMPI_SupportedThreadingLevel)
pnmpi_add_man(pnmpi_module_hooks PNMPI_RegistrationPoint)
pnmpi_add_man(pnmpi_module_hooks PNMPI_RegistrationComplete)
pnmpi_add_man(pnmpi_module_hooks PNMPI_AppStartup)
pnmpi_add_man(pnmpi_module_hooks PNMPI_AppShutdown)


include_directories(
  ${PNMPI_HEADER_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${PROJECT_BINARY_DIR}
  ${MPI_C_INCLUDE_PATH})

easy_add_library(pnmpi_modules OBJECT
  valid_modhandle.c)

foreach (target pnmpi_modules pnmpi_modules_pic)
  add_dependencies(${target} generate)

  add_coverage(${target})
  add_sanitizers(${target})
endforeach ()
