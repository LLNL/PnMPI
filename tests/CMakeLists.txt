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

include(easytest)
include(PnMPI_headers)

find_package(codecov)
find_package(MPI REQUIRED)
find_package(Sanitizers)


# easytest setup hook.
#
# This hook will be used to set variables for PnMPI tests depending on the test
# case and extracting the PnMPI configuration file if available.
#
macro (easytest_hook_setup TEST_TARGET BINARY_TARGET CONFIG MAIN_SOURCE)
  # If this test is a PnMPI module, set the module name variable (the binary
  # target name).
  easytest_get_key(MODTYPE EASYTEST_MODTYPE ${MAIN_SOURCE})
  if (EASYTEST_MODTYPE)
    set(MODNAME "${BINARY_TARGET}")
  endif ()

  # If a PnMPI configuration file is embedded in the test file, extract it to
  # store it in an external file, which may be loaded by the test case.
  easytest_get_key("PNMPICONF" EASYTEST_PNMPICONF ${MAIN_SOURCE})
  easytest_get_key("PNMPICONF-${CONFIG}" EASYTEST_PNMPICONF ${MAIN_SOURCE})
  if (EASYTEST_PNMPICONF)
    set(PNMPICONF "${CMAKE_CURRENT_BINARY_DIR}/${TEST_TARGET}.conf")
    file(WRITE "${PNMPICONF}" "${EASYTEST_PNMPICONF}")
  endif ()
endmacro ()


# easytest compile hook.
#
# If the test file is a PnMPI module, it must be build with the PMPI or XMPI
# module build functions. This hook will handle to call these functions, if the
# test file is a module.
#
macro (easytest_hook_compile TARGET CONFIG MAIN_SOURCE)
  # If test case is a PnMPI module, build it with the PnMPI helper functions
  # instead of the default easytest hook.
  if (EASYTEST_MODTYPE)
    if (EASYTEST_MODTYPE STREQUAL "XMPI")
      pnmpi_add_xmpi_module(${TARGET} ${MAIN_SOURCE} ${ARGN})
    elseif (EASYTEST_MODTYPE STREQUAL "PMPI")
      pnmpi_add_xmpi_module(${TARGET} ${MAIN_SOURCE} ${ARGN})
    else ()
      message(FATAL_ERROR "Unknown module type '${EASYTEST_MODTYPE}'.")
    endif ()

  # If the module is not an PnMPI module, call the original hook to build the
  # binary.
  else ()
    _easytest_hook_compile(${TARGET} "${CONFIG}" ${MAIN_SOURCE} ${ARGN})
  endif()
endmacro()


# easytest post-compile hook
#
# This hook will be used to add coverage and sanitizers to all test binaries.
# The functions (provided by their CMake modules) will add them, if the user
# enabled these features.
#
macro (easytest_hook_post_compile TARGET CONFIG MAIN_SOURCE)
  _easytest_hook_post_compile(${TARGET} "${CONFIG}" ${MAIN_SOURCE})

  add_coverage(${TARGET})
  add_sanitizers(${TARGET})
endmacro ()


# easytest post-test hook
#
# This hook will be used to set the environment for all test binaries, if
# necessary.
#
macro(easytest_hook_post_test TARGET CONFIG MAIN_SOURCE)
  _easytest_hook_post_test(${TARGET} "${CONFIG}" ${MAIN_SOURCE})

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

  # Set the PnMPI module path for the build and install directories, so the
  # tests may be executed before installation.
  set(PNMPI_MODULE_PATHS ${PROJECT_BINARY_DIR}/modules/comm
                         ${PROJECT_BINARY_DIR}/modules/datatype
                         ${PROJECT_BINARY_DIR}/modules/empty
                         ${PROJECT_BINARY_DIR}/modules/limit-threading
                         ${PROJECT_BINARY_DIR}/modules/metrics
                         ${PROJECT_BINARY_DIR}/modules/requests
                         ${PROJECT_BINARY_DIR}/modules/sample
                         ${PROJECT_BINARY_DIR}/modules/status
                         ${PROJECT_BINARY_DIR}/modules/switch-matrix
                         ${PROJECT_BINARY_DIR}/modules/timelapse
                         ${PROJECT_BINARY_DIR}/modules/virtual
                         ${PROJECT_BINARY_DIR}/modules/wait-for-debugger)
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

  if (SANITIZE_ADDRESS)
    # OpenMPI does have memory leaks, which can't be suppressed by
    # LeakSanitizer, as they occur in an unknown module. If OpenMPI is used and
    # the test case does not explictly request the LeakSanitizer, it will be
    # disabled.
    if (MPI_C_INCLUDE_PATH MATCHES "openmpi")
      easytest_get_key(LeakSanitizer lsan_enabled ${MAIN_SOURCE})
      if (NOT lsan_enabled)
        set_property(TEST ${TARGET} APPEND PROPERTY ENVIRONMENT
                     "ASAN_OPTIONS=detect_leaks=0")
      endif ()
    endif ()

    # Add fail regular expressions for AddressSanitizer and LeakSanitizer. This
    # will make tests fail, if one of them fails, even if the rest of the test
    # succeeds. The expression will be appended to extend expressions set by the
    # test file.
    set_property(TEST ${TARGET} APPEND PROPERTY FAIL_REGULAR_EXPRESSION
                 "LeakSanitizer" "AddressSanitizer")
  endif ()
endmacro ()


# The PnMPI header directory will be included always, so that it doesn't need to
# be included by every single test file.
include_directories(${PNMPI_HEADER_DIR})


# Some variables used in the testcases, e.g. MPIEXEC_PREFLAGS, need to be
# converted from list to string to be used with easytest, as the whole command
# will be wrapped in sh -c ''.
#
# The following solution is based on http://stackoverflow.com/a/7216542.
foreach (VAR MPIEXEC_PREFLAGS MPIEXEC_POSTFLAGS)
  string (REGEX REPLACE "([^\\]|^);" "\\1 " ${VAR} "${${VAR}}")
  string (REGEX REPLACE "[\\](.)" "\\1" ${VAR} "${${VAR}}")
endforeach ()


# Add src directory first. Binaries used by other tests will be build in this
# directory and common variables set (in the scope of this file).
add_subdirectory(src)

# Set an PNMPIZE variable for all test cases, so they may use it for calling
# PnMPIze. If AddressSanitizer is enabled, an extra wrapper will be used, so
# AddressSanitizer will be initialized.
set(PNMPIZE "$<TARGET_FILE:pnmpize>")
if (SANITIZE_ADDRESS)
  set(PNMPIZE "${ASan_WRAPPER} ${PNMPIZE}")
endif ()



# Recurse into subdirectories with test cases. The order of the test cases
# should be to test components used by other tests first to find bugs early.
add_subdirectory(pnmpize)
add_subdirectory(pnmpi)

if (ENABLE_MODULES)
  add_subdirectory(modules)
endif ()
