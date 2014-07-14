# CMake file
# Copyright (C) ZIH, Technische Universitaet Dresden, Federal Republic of Germany, 2011-2011
# Copyright (C) Lawrence Livermore National Laboratories, United States of America, 2011-2011
#
# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @file FeatureTests.cmake
#       CMake file for running feature tests.
#
# @author Mathias Korepkat, Tobias Hilbrich
# @date 25.11.2011

###Run all feature tests that influence config.h
#MPI-2 constants
featureTestMpi ("ft_mpi_status_ignore.c" C HAVE_MPI_STATUS_IGNORE)
featureTestMpi ("ft_mpi_statuses_ignore.c" C HAVE_MPI_STATUSES_IGNORE)
featureTestMpi ("ft_mpi_f_status_ignore.c" C HAVE_MPI_F_STATUS_IGNORE)
featureTestMpi ("ft_mpi_f_statuses_ignore.c" C HAVE_MPI_F_STATUSES_IGNORE)

#MPI 2 Functionality
featureTestMpi ("ft_mpi_init_thread.c" C HAVE_MPI_INIT_THREAD_C)

#MPI Fortran things
IF (CMAKE_Fortran_COMPILER_WORKS)
  featureTestMpi ( "ft_mpi_init_thread.f" Fortran HAVE_MPI_INIT_THREAD_Fortran)
ENDIF (CMAKE_Fortran_COMPILER_WORKS)

# MPI3 defines some args constant
featureTestMpi ("ft_mpi3_const_args.c" C HAVE_MPI3_CONST_ARGS)

featureTestMpi ("ft_pmpi_type_create_indexed_block.c" C HAVE_PMPI_TYPE_CREATE_INDEXED_BLOCK)

# handle convert macros may be present
##@TODO we also have feature tests for the other handles, we just didn't have the time to
##handle them correctly in the existing stuff, so right now just what we use
##@NOTE These tests are CPP as for MPICH2(?) the Request_c2f/f2c stuff is broken for
##CPP (but nor for C); with that we see the issue there and do not use them for this MPI,
##which is a good choice
SET (HandleConvertMacros
    MPI_STATUS_C2F
    CACHE INTERNAL "All the handle convert macros"
)

FOREACH (type ${HandleConvertMacros})
    STRING (TOLOWER ${type} type_lower)
    featureTestMpi ( "ft_${type_lower}2c.cpp" CXX HAVE_${type})
ENDFOREACH (type)

if (BFD_FOUND)
    featureTest("ft_bfd_old_api.c" "C" "PNMPI_OLD_BFD_API")
    featureTest("ft_bfd_new_api.c" "C" "PNMPI_NEW_BFD_API")
endif ()

SET(PNMPI_ENABLE_THREAD_SAFETY OFF CACHE BOOL "Pthreads found and thread safety selected")

option(ENABLE_THREAD_SAFETY "Selects whether pnmpi is built threadsafe." FALSE)
if(ENABLE_THREAD_SAFETY)
    featureTest("ft_pthreads.c" C PNMPI_HAVE_PTHREADS)
    IF(PNMPI_HAVE_PTHREADS)
        SET(PNMPI_ENABLE_THREAD_SAFETY ON)
    ELSE(PNMPI_HAVE_PTHREADS)
        MESSAGE(FATAL_ERROR "Enabling thread-safety failed, no pthread support found on this system")
    ENDIF(PNMPI_HAVE_PTHREADS)
endif(ENABLE_THREAD_SAFETY)
