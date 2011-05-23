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

#Enable this line for debugging
#SET (CMAKE_VERBOSE_MAKEFILE 1)

#======================================================================
# RPATH setup.  Default is to rpath everything.  Set the option to 
# false if you don't want this behavior.  
#======================================================================
option(CMAKE_INSTALL_RPATH_USE_LINK_PATH "Add rpath for all dependencies." TRUE)

#===================================
# Basic initialization
#===================================
# Version info
set(PNMPI_MAJOR_VERSION 1)
set(PNMPI_MINOR_VERSION 4)
set(PNMPI_PATCH_VERSION 0)
set(PNMPI_VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION})

# Set version of the P^nMPI interface (used to determine incompatibility with P^nMPI lib)
# TODO: is this just for services?  The basic interface is going to be MPI no matter what.
set(PNMPI_LIB_INTERFACE_VERSION "1")

# Set the module search path to include local modules.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakemodules;${CMAKE_MODULE_PATH}") #

# Sensible defaults for the install prefix on Win32.  CMake defaults to /usr/local on UNIX.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (WIN32)
    set(CMAKE_INSTALL_PREFIX "$ENV{PROGRAMFILES}/pnmpi" CACHE PATH "CMAKE_INSTALL_PREFIX" FORCE)
  endif()
endif()

#Add helper stuff
include(PnMPIModules)

#Find Packages
find_package(AWK REQUIRED)

#===================================
#System inspection
#===================================
# TODO
#     currently hardcoded, should be put into find modules:
#     * detect python
#     * detect gawk, tcsh, sort, uniq, sed
#     * dl open lib detection
find_package(MPI)
set(MPI_CXX_SKIP_FLAGS
    -DMPI_NO_CPPBIND -DOMPI_SKIP_MPICXX -D_MPICC_H -DMPICH_SKIP_MPICXX -DMPIBULL_SKIP_MPICXX
    CACHE STRING "Default skip-flags used (for SGI, OpenMPI, HP-MPI, MPICH, BULL-MPI)")

# Fortran name mangling option, customizable at cmd line
# TODO: detect fortran name mangling, e.g.: F77SYMBOL = symbol_
option(F77SYMBOL "Name demangling scheme." symbol_) 

# BFD is used by the patch utility.
find_package(BFD)

#===================================	
# Ennable PnMPI internal debugging 
#===================================	
if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-DDBGLEVEL=0)
endif()

#===================================	
#Check for adept_utils timers
#===================================	
find_package(AdeptUtils QUIET)
if(AdeptUtils_FOUND)
  message(STATUS "Found AdeptUtils in ${AdeptUtils_DIR}.")
  add_definitions(-DHAVE_ADEPT_UTILS)
else()
  message(STATUS "AdeptUtils package not found.  Building without timing.")
  # Set these for our PnMPIConfig.cmake file.
  set(AdeptUtils_INCLUDE_PATH AdeptUtils_INCLUDE_PATH-NOTFOUND)
  set(AdeptUtils_LIBRARIES    AdeptUtils_LIBRARIES-NOTFOUND)
endif()


#======================================================
# Set up wrapper generator so this project can use it.
#======================================================
set(WRAP ${PROJECT_SOURCE_DIR}/wrap/wrap.py)
configure_file(
  ${PROJECT_SOURCE_DIR}/wrap/WrapConfig.cmake.in
  ${PROJECT_BINARY_DIR}/wrap/WrapBuildConfig.cmake @ONLY)
include(${PROJECT_BINARY_DIR}/wrap/WrapBuildConfig.cmake)

#===================================
# Recurse to sub-directories
#===================================
add_subdirectory(wrap)
add_subdirectory(src)
add_subdirectory(patch)
add_subdirectory(modules)

enable_testing() #activates creation of tests
add_subdirectory(demo)

#===================================
#Other install stuff (which is not handled in the sub-dirs)
#===================================
#Install includes (collected and copied during build process)
install(DIRECTORY ${INCLUDE_OUTPUT_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})

# Configure a header file with the full build configuration.
configure_file(
  ${PROJECT_SOURCE_DIR}/pnmpi-config.h.in
  ${PROJECT_BINARY_DIR}/pnmpi-config.h)
install(FILES ${PROJECT_BINARY_DIR}/pnmpi-config.h DESTINATION include)

# Install configuration file so that other projects can use PnMPI
configure_file(
  ${PROJECT_SOURCE_DIR}/PnMPIConfig.cmake.in
  ${PROJECT_BINARY_DIR}/PnMPIConfig.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/PnMPIConfig.cmake DESTINATION lib/cmake/PnMPI)
install(FILES ${PROJECT_SOURCE_DIR}/cmakemodules/PnMPIModules.cmake DESTINATION lib/cmake/PnMPI)
