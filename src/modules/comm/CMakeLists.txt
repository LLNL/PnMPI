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

#Set names for targets
set(PREFIX commsub)
set(SUBMODULES print empty)
# TODO: checksum removed due to incompatibility with OpenMPI

#Loop over all targets (submods)
foreach(mod ${SUBMODULES})
    # Use macro to: add target for module, version it, install it, patch it
    set(modulename ${PREFIX}-${mod})
    pnmpi_add_pmpi_module(${modulename} ${modulename}.cpp comm.c)

    #  Add the submodname define as a flag
    ## TODO: Martin, please have a look at the "SUBMODNAME" define, is that what you want ???
    set_source_files_properties(${modulename}.cpp COMPILE_FLAGS "-DSUBMODNAME='\"${mod}\"'")

    install(TARGETS ${modulename} DESTINATION lib/pnmpi-modules)
endforeach()

include_directories(
  ${PROJECT_SOURCE_DIR}/src/modules/datatype
  ${PROJECT_SOURCE_DIR}/src/modules/requests
  ${PROJECT_SOURCE_DIR}/src/modules/status
)