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

#Add the target for the patcher
if (PATCHER_FRONTEND_COMPILER)
   set(CMAKE_C_COMPILER ${PATCHER_FRONTEND_COMPILER})
   set(CMAKE_C_FLAGS "")
   set(CMAKE_C_FLAGS_DEBUG "")
   set(CMAKE_BUILD_TYPE "Debug")
   set(BFD_FOUND False)
   set(DPNMPI_HAVE_BFD False)

   add_executable(pnmpi-patch ../patch.c)
   add_coverage(pnmpi-patch)
   add_sanitizers(pnmpi-patch)
   install(TARGETS pnmpi-patch EXPORT pnmpi-patch RUNTIME DESTINATION bin)

   # Allow patch target to be used by subprojects.
   install(EXPORT pnmpi-patch DESTINATION share/cmake/PnMPI)
endif()

include_directories(${PROJECT_BINARY_DIR})
