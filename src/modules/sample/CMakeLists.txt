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

#modules to build:
set(MODS sample1 sample2 sample3 sample4 sample1f)

#loop over modules and build them
foreach(mod ${MODS})
    #Use macro to: add target for module, version it, install it, patch it
    pnmpi_add_pmpi_module(${mod} ${mod}.c)
    install(TARGETS ${mod} DESTINATION ${PnMPI_MODULES_DIR})
endforeach()

# Create special non-patched version of sample1 (as SHARED and not as MODULE)
# Used as part of the demo to show simple preloading of PMPI libraries.
add_library(sample1-non-patched SHARED sample1.c)
target_link_libraries(sample1-non-patched ${MPI_C_LIBRARIES})
