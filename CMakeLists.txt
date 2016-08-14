# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011
cmake_minimum_required(VERSION 2.6)
project(pnmpi C CXX)

# Workaround for this CMake issue: http://public.kitware.com/Bug/view.php?id=9220
# Ensures that enable_language(Fortran OPTIONAL) works correctly.  Annoying.
if (DEFINED CMAKE_Fortran_COMPILER AND CMAKE_Fortran_COMPILER MATCHES "^$")
  set(CMAKE_Fortran_COMPILER CMAKE_Fortran_COMPILER-NOTFOUND)
endif()
enable_language(Fortran OPTIONAL)

#===================================
#Policies
#===================================
foreach (POL CMP0017)
  if (POLICY ${POL})
    cmake_policy(SET ${POL} NEW)
  endif ()
endforeach ()

foreach (POL CMP0026 CMP0042)
  if (POLICY ${POL})
    cmake_policy(SET ${POL} OLD)
  endif ()
endforeach ()


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
  "${CMAKE_SOURCE_DIR}/cmakemodules"
  "${CMAKE_SOURCE_DIR}/externals/CMake-codecov/cmake"
  "${CMAKE_SOURCE_DIR}/externals/CMake-sanitizers/cmake"
  "${CMAKE_SOURCE_DIR}/externals/CMake-MPIhelper/cmake"
  "${CMAKE_SOURCE_DIR}/externals/CMake-argp/cmake"
  "${CMAKE_SOURCE_DIR}/externals/CMake-gitinfo/cmake"
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmakemodules/legacy")


# Get the version info from git.
if (EXISTS "${PROJECT_SOURCE_DIR}/.git")
  find_package(GitInfo REQUIRED)
  git_wc_info(${PROJECT_SOURCE_DIR} GIT)

# If git is not available (e.g. this git was packed as .tar.gz), try to read the
# version-info from a hidden file in the root directory. This file should not be
# versioned, but added at packaging time.
elseif (EXISTS "${PROJECT_SOURCE_DIR}/.version")
  file(READ "${PROJECT_SOURCE_DIR}/.version" GIT_WC_LATEST_TAG_LONG)

# If no version could be gathered by git or the version file, print a warning,
# so the user has to define a version in the backup version file.
else ()
  message(FATAL_ERROR "No PnMPI version provided by .version file")
endif ()

set(PNMPI_VERSION ${GIT_WC_LATEST_TAG_LONG} CACHE STRING "PnMPI version")
if (GIT_WC_LATEST_TAG_LONG MATCHES "^([^0-9]*)([0-9]+)[.]([0-9]+)-([0-9]+)")
  set(PNMPI_MAJOR_VERSION ${CMAKE_MATCH_2} CACHE STRING "")
  set(PNMPI_MINOR_VERSION ${CMAKE_MATCH_3} CACHE STRING "")
  set(PNMPI_PATCH_VERSION ${CMAKE_MATCH_4} CACHE STRING "")
else ()
  message(FATAL_ERROR "Invalid version info: '${GIT_WC_LATEST_TAG_LONG}'.")
endif ()


#Include our macros
INCLUDE (PnMPIModules)
find_package(codecov)

#===================================
#System inspection
#===================================
# TODO
#     currently hardcoded, should be put into find modules:
#     * detect gawk, tcsh, sort, uniq, sed
#     * dl open lib detection
find_package(PythonInterp 2.6 REQUIRED)
if (NOT PYTHONINTERP_FOUND)
  message(FATAL_ERROR "PnMPI requires a Python interpreter to build.")
endif()

find_package(MPI)
set(MPI_CXX_SKIP_FLAGS
    -DMPI_NO_CPPBIND -DOMPI_SKIP_MPICXX -D_MPICC_H -DMPICH_SKIP_MPICXX -DMPIBULL_SKIP_MPICXX
    CACHE STRING "Default skip-flags used (for SGI, OpenMPI, HP-MPI, MPICH, BULL-MPI)")

# BFD is used by the patch utility.
find_package(BFD)

#Run feature tests
INCLUDE (FeatureTests)
add_subdirectory(externals)

#===================================
# Ennable PnMPI internal debugging
#===================================
if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-DDBGLEVEL=0)
endif()

#===================================
# Recurse to sub-directories
#===================================
add_subdirectory(doc)
add_subdirectory(src)

option(ENABLE_MODULES "Selects whether PnMPI modules are built." TRUE)
if(ENABLE_MODULES)
    add_subdirectory(modules)
endif(ENABLE_MODULES)

option(ENABLE_DEMO "Selects whether demo biraries are built." TRUE)
if(ENABLE_DEMO)
    if(ENABLE_MODULES)
	# only if demo and modules get built
        enable_testing() #activates creation of tests
    endif(ENABLE_MODULES)
    add_subdirectory(demo)
endif(ENABLE_DEMO)

#===================================
#Other install stuff (which is not handled in the sub-dirs)
#===================================
#Install includes (collected and copied during build process)
install(DIRECTORY ${INCLUDE_OUTPUT_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install configuration file so that other projects can use PnMPI
configure_file(${PROJECT_SOURCE_DIR}/PnMPIConfig.cmake.in ${PROJECT_BINARY_DIR}/PnMPIConfig.cmake @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/pnmpi-vars.cmake.in  ${PROJECT_BINARY_DIR}/pnmpi-vars.cmake  @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/PnMPIConfig.cmake
              ${PROJECT_BINARY_DIR}/pnmpi-vars.cmake
              ${PROJECT_SOURCE_DIR}/cmakemodules/PnMPIModules.cmake
              DESTINATION share/cmake/PnMPI)

# Configure a header file with the full build configuration.  Include the CMake file
# first so that the header can include build parameters.
# include(${PROJECT_BINARY_DIR}/pnmpi-vars.cmake)
SET(PnMPI_INSTALL_PREFIX )
configure_file(${PROJECT_SOURCE_DIR}/pnmpi-config.h.in ${PROJECT_BINARY_DIR}/pnmpi-config.h @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/pnmpi-config.h DESTINATION include)


option(ENABLE_TESTS "Selects whether tests are built." false)
if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif (ENABLE_TESTS)


# evaluate coverage data
coverage_evaluate()
