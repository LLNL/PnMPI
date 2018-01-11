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

cmake_minimum_required(VERSION 2.8.11.2)
project(pnmpi C CXX)


# Otional Fortran support.
#
# By default PnMPI will be build with the Fortran interface. If PnMPI should not
# be build with the Fortran interface or no Fortran compiler could be found, the
# Fortran interface will be disabled.
option(ENABLE_FORTRAN "Enable PnMPI Fortran interface." on)
if (ENABLE_FORTRAN)
  # Workaround for this CMake issue:
  #  http://public.kitware.com/Bug/view.php?id=9220
  # Ensures that enable_language(Fortran OPTIONAL) works correctly.  Annoying.
  if (DEFINED CMAKE_Fortran_COMPILER AND CMAKE_Fortran_COMPILER MATCHES "^$")
    set(CMAKE_Fortran_COMPILER CMAKE_Fortran_COMPILER-NOTFOUND)
  endif()

  enable_language(Fortran OPTIONAL)

  if (NOT CMAKE_Fortran_COMPILER_WORKS)
    set(ENABLE_FORTRAN false)
    message(STATUS "No working Fortran compiler found: PnMPI will be build "
      "without Fortran interface.")
  endif ()
endif ()


#===================================
#Policies
#===================================
foreach (POL CMP0017 CMP0051)
  if (POLICY ${POL})
    cmake_policy(SET ${POL} NEW)
  endif ()
endforeach ()

foreach (POL CMP0042)
  if (POLICY ${POL})
    cmake_policy(SET ${POL} OLD)
  endif ()
endforeach ()


# Workaournd for this CMake issues:
# https://cmake.org/pipermail/cmake/2010-November/040949.html
if(CMAKE_Fortran_COMPILER_ID MATCHES "PGI")
  set(CMAKE_SHARED_LIBRARY_LINK_Fortran_FLAGS "")
endif()


#======================================================================
# RPATH setup.  Default is to rpath everything.  Set the option to
# false if you don't want this behavior.
#======================================================================
option(CMAKE_INSTALL_RPATH_USE_LINK_PATH "Add rpath for all dependencies." TRUE)

#===================================
# Basic initialization
#===================================

# Set the module search path to include local modules.
set(CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmakemodules"
  "${PROJECT_SOURCE_DIR}/externals/CMake-argp/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-CChelper/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-codecov/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-easylib/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-easytest/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-gitinfo/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-MPIhelper/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-sanitizers/cmake"
  ${CMAKE_MODULE_PATH}
  "${PROJECT_SOURCE_DIR}/cmakemodules/legacy")


# Get the version info from git.
find_package(GitInfo REQUIRED)
git_version_info(PNMPI)


#===================================
#System inspection
#===================================
# TODO
#     currently hardcoded, should be put into find modules:
#     * detect gawk, tcsh, sort, uniq, sed
#     * dl open lib detection
find_package(MPI)
set(MPI_CXX_SKIP_FLAGS
    -DMPI_NO_CPPBIND -DOMPI_SKIP_MPICXX -D_MPICC_H -DMPICH_SKIP_MPICXX -DMPIBULL_SKIP_MPICXX
    CACHE STRING "Default skip-flags used (for SGI, OpenMPI, HP-MPI, MPICH, BULL-MPI)")

# BFD is used by the patch utility.
find_package(BFD)


# Search for C11 compiler support.
#
# Some of the modules and the PnMPI library try to use C11 features. Instead of
# searching for them in each module this will be done here, so the message gets
# only printed once.
find_package(C11 OPTIONAL_COMPONENTS ATOMICS THREAD_LOCAL)


#Run feature tests
INCLUDE (FeatureTests)


#
# Add code coverage for the sourcecode.
#
find_package(codecov)


#===================================
# Performance optimizations
#===================================
option(ENABLE_DEBUG "Enable PnMPI debug logging and assertions." on)
if (NOT ENABLE_DEBUG)
  # For performance, assertions and debug logging may be disabled in PnMPI to
  # reduce the overhead in the PnMPI stack. This will not change the PnMPI API,
  # but will compile much less comparisons into the code so PnMPI won't print
  # any debug information nor check any parameters.
  add_definitions("-DNDEBUG" "-DPNMPI_NO_DEBUG")
endif ()


#===================================
# Recurse to sub-directories
#===================================
add_subdirectory(doc)
add_subdirectory(externals)
add_subdirectory(src)

option(ENABLE_TESTS "Selects whether tests are built." false)
if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif (ENABLE_TESTS)


# Install CMake project files
#
# Additional files will be installed, so other CMake projects are able to find
# (and use) PnMPI by simply calling 'find_package(PnMPI)'.
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

string(CONCAT PNMPI_VERSION_CONCAT "${PNMPI_VERSION_MAJOR}."
                                   "${PNMPI_VERSION_MINOR}."
                                   "${PNMPI_VERSION_PATCH}."
                                   "${PNMPI_VERSION_TWEAK}")
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/PnMPIConfigVersion.cmake
  VERSION ${PNMPI_VERSION_CONCAT}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  PnMPIConfig.cmake.in
  PnMPIConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}
)

install(
  FILES
    ${PROJECT_BINARY_DIR}/PnMPIConfigVersion.cmake
    ${PROJECT_BINARY_DIR}/PnMPIConfig.cmake
    cmakemodules/PnMPI_modules.cmake
  DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/cmake/PnMPI
)


#===================================
#Other install stuff (which is not handled in the sub-dirs)
#===================================

# Configure a header file with the full build configuration.  Include the CMake file
# first so that the header can include build parameters.
# include(${PROJECT_BINARY_DIR}/pnmpi-vars.cmake)
SET(PnMPI_INSTALL_PREFIX )
set(PNMPI_HAVE_FORTRAN ${ENABLE_FORTRAN})
configure_file(${PROJECT_SOURCE_DIR}/pnmpi-config.h.in ${PROJECT_BINARY_DIR}/pnmpi-config.h @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/pnmpi-config.h DESTINATION include)


#
# Evaluate coverage data.
#
coverage_evaluate()
