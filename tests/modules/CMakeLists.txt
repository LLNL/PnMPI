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

# If PnMPI is built without modules, the following tests and subdirectories
# should be skipped.
if (NOT ENABLE_MODULES)
  return()
endif ()


# Custom post-test hook for the module tests.
#
# Set the PnMPI module path for the build and install directories, so the tests
# may be executed before installation.
macro(pnmpi_post_test TARGET CONFIG MAIN_SOURCE)
  set(PNMPI_MODULE_PATHS ${PROJECT_BINARY_DIR}/src/modules/comm
                         ${PROJECT_BINARY_DIR}/src/modules/datatype
                         ${PROJECT_BINARY_DIR}/src/modules/empty
                         ${PROJECT_BINARY_DIR}/src/modules/limit-threading
                         ${PROJECT_BINARY_DIR}/src/modules/metrics
                         ${PROJECT_BINARY_DIR}/src/modules/requests
                         ${PROJECT_BINARY_DIR}/src/modules/sample
                         ${PROJECT_BINARY_DIR}/src/modules/status
                         ${PROJECT_BINARY_DIR}/src/modules/switch-matrix
                         ${PROJECT_BINARY_DIR}/src/modules/timelapse
                         ${PROJECT_BINARY_DIR}/src/modules/virtual
                         ${PROJECT_BINARY_DIR}/src/modules/wait-for-debugger)
  get_property(TMP TEST ${TARGET} PROPERTY ENVIRONMENT)
  if (TMP)
    foreach (VAR ${TMP})
      if (VAR MATCHES "PNMPI_LIB_PATH=.*")
        list(REMOVE_ITEM TMP "${VAR}")
        set_property(TEST ${TARGET} PROPERTY ENVIRONMENT "${TMP}")

        string(REPLACE "PNMPI_LIB_PATH=" "" VAR ${VAR})
        string(REPLACE ":" ";" VAR "${VAR}")
        list(APPEND PNMPI_MODULE_PATHS ${VAR})
      endif ()
    endforeach ()
  endif ()
  string(REPLACE ";" ":" PNMPI_MODULE_PATHS "${PNMPI_MODULE_PATHS}")
  set_property(TEST ${TARGET} APPEND PROPERTY
               ENVIRONMENT "PNMPI_LIB_PATH=${PNMPI_MODULE_PATHS}")
endmacro()


# Recurse into subdirectories.
add_subdirectory(limit-threading)
add_subdirectory(metrics)
add_subdirectory(wait-for-debugger)
