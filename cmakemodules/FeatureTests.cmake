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

include(CheckCSourceCompiles)
include(CheckFortranMPIFunctionExists)
include(CheckFunctionExists)
include(CheckMPIConstCorrectness)
include(CheckMPIFunctionExists)
include(CheckMPISymbolExists)


# \brief Helper maco to execute a feature test.
#
# \param file Source file (will be searched in cmakemodules/FeatureTests)
# \param var Variable where the feature test result will be stored in
#
macro(featureTest file var)
  file(READ "${PROJECT_SOURCE_DIR}/cmakemodules/FeatureTests/${file}" SOURCE)
  check_c_source_compiles("${SOURCE}" ${var})
endmacro()


###Run all feature tests that influence config.h
#MPI-2 constants
check_mpi_symbol_exists(MPI_STATUS_IGNORE HAVE_MPI_STATUS_IGNORE)
check_mpi_symbol_exists(MPI_STATUSES_IGNORE HAVE_MPI_STATUSES_IGNORE)
check_mpi_symbol_exists(MPI_F_STATUS_IGNORE HAVE_MPI_F_STATUS_IGNORE)
check_mpi_symbol_exists(MPI_F_STATUSES_IGNORE HAVE_MPI_F_STATUSES_IGNORE)

#MPI 2 Functionality
check_mpi_function_exists(MPI_Init_thread HAVE_MPI_INIT_THREAD_C)

#MPI Fortran things
IF (CMAKE_Fortran_COMPILER_WORKS)
  check_fortran_mpi_function_exists(MPI_INIT_THREAD
    HAVE_MPI_INIT_THREAD_Fortran)
ENDIF (CMAKE_Fortran_COMPILER_WORKS)

# MPI3 defines some args constant
check_mpi_const_correctness(HAVE_MPI3_CONST_ARGS)

check_mpi_function_exists(MPI_Type_create_indexed_block
  HAVE_PMPI_TYPE_CREATE_INDEXED_BLOCK)

# handle convert macros may be present
##@TODO we also have feature tests for the other handles, we just didn't have the time to
##handle them correctly in the existing stuff, so right now just what we use
##@NOTE These tests are CPP as for MPICH2(?) the Request_c2f/f2c stuff is broken for
##CPP (but nor for C); with that we see the issue there and do not use them for this MPI,
##which is a good choice
SET (HandleConvertMacros
    MPI_Status
    CACHE INTERNAL "All the handle convert macros"
)

FOREACH (type ${HandleConvertMacros})
  string(TOUPPER "HAVE_${type}" variable)

  check_mpi_function_exists(${type}_f2c "${variable}_F2C")
  if (${${variable}_F2C})
    check_mpi_function_exists(${type}_c2f "${variable}_C2F")
  endif ()
ENDFOREACH (type)

if (BFD_FOUND)
  featureTest("ft_bfd_old_api.c" PNMPI_OLD_BFD_API)
  featureTest("ft_bfd_new_api.c" PNMPI_NEW_BFD_API)
endif ()
