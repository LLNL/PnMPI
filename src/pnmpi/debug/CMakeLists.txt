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

include(PnMPI_doc)


# Install and manage the public and private header files of the PnMPI sources.
# This has to be done to use the same headers for the PnMPI sources and the API
# without getting conflicts about the used paths. Otherwise you'd have to take
# care about file locations after installation, especially if you use the
# headers over multiple directories.
pnmpi_add_header(debug_io.h)
pnmpi_add_private_header(print.h)

# Generate a new header for PMPI functions. This header may be used as shortcut
# for calling the function and printing an error if the error code does not
# match MPI_SUCCESS.
add_wrapped_file("../header/pnmpi/private/pmpi_assert.h" pmpi_assert.h.w -s)
add_custom_target(pnmpi_debug_pmpi
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/../header/pnmpi/private/pmpi_assert.h")


# Install the public man pages for this object.
pnmpi_add_man(pnmpi_debug_io PNMPI_MESSAGE_PREFIX)
pnmpi_add_man(pnmpi_debug_io PNMPI_NO_DEBUG)
pnmpi_add_man(pnmpi_debug_io PNMPI_debug_level_t)
pnmpi_add_man(pnmpi_debug_io PNMPI_Debug)
pnmpi_add_man(pnmpi_debug_io PNMPI_Warning)
pnmpi_add_man(pnmpi_debug_io PNMPI_Error)


easy_add_library(pnmpi_debug OBJECT
  get_rank.c
  print_debug.c
  print_error.c
  print_prefix_rank.c
  print_warning.c)

easy_target_link_libraries(pnmpi_debug ${MPI_C_LIBRARIES} m)

foreach (target pnmpi_debug pnmpi_debug_pic)
  add_dependencies(${target} pnmpi_debug_pmpi)
endforeach ()

add_coverage(pnmpi_debug_pic)
add_sanitizers(pnmpi_debug_pic)
