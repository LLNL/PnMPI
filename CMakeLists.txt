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

cmake_minimum_required(VERSION 2.8.11.2)


#===================================
# Policies
#===================================
foreach (POL CMP0017 CMP0042 CMP0051 CMP0074)
  if (POLICY ${POL})
    cmake_policy(SET ${POL} NEW)
  endif ()
endforeach ()


#===================================
# Basic initialization
#===================================
project(pnmpi C CXX)

# Set the module search path to include local modules.
set(CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmakemodules"
  "${PROJECT_SOURCE_DIR}/externals/CMake-argp/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-CChelper/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-codecov/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-easylib/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-easytest/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-gitinfo/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-gitpack/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-MPIhelper/cmake"
  "${PROJECT_SOURCE_DIR}/externals/CMake-sanitizers/cmake"
  ${CMAKE_MODULE_PATH})

# Get the version info from git.
find_package(GitInfo REQUIRED)
git_version_info(PNMPI)

# Optional Fortran support.
#
# By default PnMPI will be build with the Fortran interface. If PnMPI should not
# be build with the Fortran interface or no Fortran compiler could be found, the
# Fortran interface will be disabled.
option(ENABLE_FORTRAN "Enable PnMPI Fortran interface." ON)
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

  # Workaround for this CMake issues:
  # https://cmake.org/pipermail/cmake/2010-November/040949.html
  if(CMAKE_Fortran_COMPILER_ID MATCHES "PGI")
    set(CMAKE_SHARED_LIBRARY_LINK_Fortran_FLAGS "")
  endif()
endif ()


#===================================
# Compiler options
#===================================

# RPATH setup.
#
# By default rpath will be used for all targets, so the binary's dependent
# libraries will be found without adjusting the loader's environment. This
# feature may be disabled to use the linker's search paths instead.
option(CMAKE_INSTALL_RPATH_USE_LINK_PATH "Add rpath for all dependencies." ON)

# Set MPI skip flags for C++ code
#
# Some MPI implementations use a legacy C++ header for mapping MPI C calls to
# C++ methods. Sometimes, code compiled with this "automatic feature" fails when
# executed due unresolvable symbols. The following flags will disable this
# "feature" entirely for all C++ code compiled in this project.
set(MPI_CXX_SKIP_FLAGS "-DMPI_NO_CPPBIND"      # SGI
                       "-DOMPI_SKIP_MPICXX"    # OpenMPI
                       "-D_MPICC_H"            # HP-MPI
                       "-DMPICH_SKIP_MPICXX"   # MPICH
                       "-DMPIBULL_SKIP_MPICXX" # BUll-MPI
    CACHE STRING "Flags to skip C++ MPI")
string(REPLACE ";" " " CMAKE_CXX_FLAGS
       "${CMAKE_CXX_FLAGS} ${MPI_CXX_SKIP_FLAGS}")


#===================================
# Performance optimizations
#===================================
option(ENABLE_DEBUG "Enable PnMPI debug logging and assertions." ON)
if (NOT ENABLE_DEBUG)
  # For performance, assertions and debug logging may be disabled in PnMPI to
  # reduce the overhead in the PnMPI stack. This will not change the PnMPI API,
  # but will compile much less comparisons into the code so PnMPI won't print
  # any debug information nor check any parameters.
  add_definitions("-DNDEBUG" "-DPNMPI_NO_DEBUG")
endif ()


#===================================
# System inspection
#===================================

# Find general packages required by most of the targets.
#
# NOTE: Packages used by a specific target only should be searched in the
#       CMakeLists.txt file defining the target.
find_package(codecov)
find_package(MPI REQUIRED)
find_package(Sanitizers)

# Search for C11 compiler support.
#
# Some of the modules and the PnMPI library try to use C11 features. Instead of
# searching for them in each module this will be done here, so the message gets
# printed only once.
find_package(C11 OPTIONAL_COMPONENTS ATOMICS THREAD_LOCAL)


#===================================
# Recurse to sub-directories
#===================================
add_subdirectory(doc)
add_subdirectory(externals)
add_subdirectory(src)

# Add PnMPI test cases.
#
# PnMPI test cases will be managed in the 'tests/' subdirectory. As
# 'enable_testing()' needs to be called in the project's root directory,
# including the directory and related options will be managed here instead of
# inside the directory.
#
# NOTE: If PnMPI is included into other projects via 'add_subdirectory()',
#       testing will be disabled by CMake. Therefore, this directory doesn't
#       need to be included to avoid test binaries to be built.
option(ENABLE_TESTS "Selects whether tests are built." OFF)
if (ENABLE_TESTS AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
  enable_testing()
  add_subdirectory(tests)
endif ()


# Evaluate coverage data.
#
# Coverage data needs to be evaluated after all binary targets have been added,
# as coverage_evaluate() can't handle targets defined in the future.
coverage_evaluate()


# Install CMake project files
#
# Additional files will be installed, so other CMake projects are able to find
# (and use) PnMPI by simply calling 'find_package(PnMPI)'.
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(PNMPI_VERSION_CONCAT "${PNMPI_VERSION_MAJOR}" "${PNMPI_VERSION_MINOR}"
                         "${PNMPI_VERSION_PATCH}" "${PNMPI_VERSION_TWEAK}")
string(REPLACE ";" "." PNMPI_VERSION_CONCAT "${PNMPI_VERSION_CONCAT}")
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


# Populate cache with PnMPI configuration.
#
# If PnMPI is included into other projects via add_subdirectory(), additional
# variables need to be populated in the cache, so PnMPI is able to be used from
# the parent directories.
if (NOT PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(PnMPI_FOUND         true PARENT_SCOPE)
  set(PnMPI_IS_SUBPROJECT true)
  include("${CMAKE_CURRENT_BINARY_DIR}/PnMPIConfig.cmake")
endif ()


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


# CPack configuration for packaging sources.
#
# If PnMPI is NOT included into other projects (via add_subdirectory()),
# configure CPack to pack the sources. The package may be generated by building
# the 'package_deploy' target.
if (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(CPACK_PACKAGE_VERSION          "${PNMPI_VERSION}")
  set(CPACK_PACKAGE_VERSION_MAJOR    ${PNMPI_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR    ${PNMPI_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH    ${PNMPI_VERSION_PATCH})
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "PnMPI-${PNMPI_VERSION}")

  include(GitPack)
  include(CPackDeploy)
endif ()
