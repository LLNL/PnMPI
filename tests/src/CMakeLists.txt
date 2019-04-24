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

# Generate a new header for MPI functions. This header may be used as shortcut
# for calling the function and printing an error if the error code does not
# match MPI_SUCCESS.
add_wrapped_file(mpi_errors.h mpi_errors.h.w -s)
add_custom_target(testbin-mpi-wrapper
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/mpi_errors.h)

# Set an LIBPNMPI variable for the following test cases, so they may use it for
# preloading PnMPI. If Fortran is enabled, LIBPNMPIF will also be set.
set(LIBPNMPI "$<TARGET_FILE:pnmpi>")
if (ENABLE_FORTRAN)
  set(LIBPNMPIF "$<TARGET_FILE:pnmpif>")
endif ()


# The following executable may be used to check the general functionality of a
# test case. It does not call any MPI functions, so it is useful for checking
# non-MPI functionality like the PnMPI startup. Some memory leaks may be tracked
# with this executable, because every application calling MPI will definitely
# have memory leaks (caused by some MPI implementations).
easy_add_test(PREFIX binary_nompi SOURCES test-nompi.c)
set(TESTBIN_NOMPI "$<TARGET_FILE:testbin-binary_nompi>" PARENT_SCOPE)

# The following target may be used to check general PnMPI functionality. It uses
# the common MPI functions MPI_Init and MPI_Finalize. As all PnMPI core routines
# will be called in MPI_Init, this should be enough to check anything except the
# wrapper functions.
easy_add_test(PREFIX binary_mpi_c SOURCES test-mpi.c)
set(TESTBIN_MPI_C "$<TARGET_FILE:testbin-binary_mpi_c-dynamic>" PARENT_SCOPE)
set(TESTBIN_MPI_C_STATIC "$<TARGET_FILE:testbin-binary_mpi_c-static>"
    PARENT_SCOPE)

# The following target may be used to check general PnMPI functionality for
# threaded MPI applications. It uses the common MPI functions MPI_Init_thread
# and MPI_Finalize. As all PnMPI core routines (with threading support) will be
# called in MPI_Init_thread, this should be enough to check the threading
# functionality of PnMPI.
easy_add_test(PREFIX binary_mpi_threaded SOURCES test-mpi-threaded.c)
set(TESTBIN_MPI_C_THREADED "$<TARGET_FILE:testbin-binary_mpi_threaded>"
    PARENT_SCOPE)

# The following target may be used to check general PnMPI wrapper functionality.
# It uses the common MPI functions MPI_Init, MPI_Finalize, MPI_Send and MPI_Recv
# and may be used with at least 2 ranks.
easy_add_test(PREFIX binary_mpi_sendrecv SOURCES test-mpi-sendrecv.c)
set(TESTBIN_MPI_C_SENDRECV "$<TARGET_FILE:testbin-binary_mpi_sendrecv>"
    PARENT_SCOPE)


if (ENABLE_FORTRAN)
  # The following target may be used to check general PnMPI Fortran
  # functionality. It uses the common MPI functions MPI_Init and MPI_Finalize.
  # As all PnMPI core routines will be called in MPI_Init, this should be enough
  # to check anything except the wrapper functions.
  easy_add_test(PREFIX binary_mpi_fortran SOURCES test-mpi.f)
  set(TESTBIN_MPI_FORTRAN "$<TARGET_FILE:testbin-binary_mpi_fortran-dynamic>"
      PARENT_SCOPE)

  # The following target may be used to check general PnMPI functionality for
  # threaded Fortran MPI applications. It uses the common MPI functions
  # MPI_Init_thread and MPI_Finalize. As all PnMPI core routines (with threading
  # support) will be called in MPI_Init_thread, this should be enough to check
  # the threading functionality of PnMPI.
  easy_add_test(PREFIX binary_mpi_fortran_threaded SOURCES test-mpi-threaded.f)
  set(TESTBIN_MPI_FORTRAN_THREADED
      "$<TARGET_FILE:testbin-binary_mpi_fortran_threaded>" PARENT_SCOPE)

  # The following target may be used to check general PnMPI Fortran wrapper
  # functionality. It uses the common MPI functions MPI_Init, MPI_Finalize,
  # MPI_Send and MPI_Recv and may be used with at least 2 ranks.
  easy_add_test(PREFIX binary_mpi_fortran_sendrecv SOURCES test-mpi-sendrecv.f)
  set(TESTBIN_MPI_FORTRAN_SENDRECV
      "$<TARGET_FILE:testbin-binary_mpi_fortran_sendrecv>" PARENT_SCOPE)
endif ()
