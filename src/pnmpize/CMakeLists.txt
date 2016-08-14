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

find_package(argp REQUIRED)
find_package(help2man REQUIRED)


include_directories(${CMAKE_BINARY_DIR})

add_executable(pnmpize pnmpize.c)
add_coverage(pnmpize)
add_sanitizers(pnmpize)
help2man(pnmpize-man TARGET pnmpize NOINFO INSTALL RENAME pnmpi.1)

target_link_libraries(pnmpize ${ARGP_LIBRARIES})
set_target_properties(pnmpize PROPERTIES OUTPUT_NAME pnmpi)

install(TARGETS pnmpize EXPORT pnmpi DESTINATION bin)
