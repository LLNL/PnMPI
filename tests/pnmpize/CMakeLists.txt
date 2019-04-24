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

# If the PnMPIze tool will not be built, the following test cases can't be
# executed.
if (NOT ENABLE_PNMPIZE)
  return()
endif ()


# Custom post-test hook for the PnMPIze tests.
#
macro(pnmpi_post_test TARGET CONFIG MAIN_SOURCE)
  # Set the library path, so PnMPIze can find PnMPI even if it's not yet
  # installed.
  if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_property(TEST ${TARGET} APPEND PROPERTY ENVIRONMENT
      "LD_LIBRARY_PATH=${PROJECT_BINARY_DIR}/src/pnmpi:$ENV{LD_LIBRARY_PATH}")
  else ()
    # As mac OS security features may delete the DYLD_LIBRARY_PATH and
    # LD_LIBRARY_PATH variables under some circumstances, the alternate
    # PNMPI_LIBRARY_PATH variable will be used in this case.
    set_property(TEST ${TARGET} APPEND PROPERTY ENVIRONMENT
                 "PNMPI_PATH=${PROJECT_BINARY_DIR}/src/pnmpi")
  endif ()
endmacro()


# Set an PNMPIZE variable for all test cases, so they may use it for calling
# PnMPIze. If AddressSanitizer is enabled, an extra wrapper will be used, so
# AddressSanitizer will be initialized.
set(PNMPIZE "$<TARGET_FILE:pnmpize>")
if (SANITIZE_ADDRESS)
  set(PNMPIZE "${ASan_WRAPPER} ${PNMPIZE}")
endif ()


easy_add_test(PREFIX pnmpize_exec SOURCES exec.conf NOBINARY)
if (ENABLE_FORTRAN)
  easy_add_test(PREFIX pnmpize_exec_fortran SOURCES exec-fortran.conf NOBINARY)
endif ()

easy_add_test(PREFIX pnmpize_config SOURCES config.c)
easy_add_test(PREFIX pnmpize_debug SOURCES debug.conf NOBINARY)
