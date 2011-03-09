# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009
cmake_minimum_required(VERSION 2.6)
project(pnmpi C CXX)

#Enable this line for debugging
#SET (CMAKE_VERBOSE_MAKEFILE 1)

#======================================================================
# RPATH setup.  Default is to rpath everything.  Set the option to 
# false if you don't want this behavior.  
#======================================================================
option(CMAKE_INSTALL_RPATH_USE_LINK_PATH "Add rpath for all dependencies." TRUE)

#===================================
# Fortran or not
#===================================
# TODO:
#       This should be extended to have a test being executed here,
#       which requires a Fortran compiler. Enable Fortran if the test
#       succeeds and disable it otherwise.
#
if (WIN32)
  set(PNMPI_ENABLE_FORTRAN OFF CACHE BOOL "Enables P^nMPI support for Fortran, requires a Fortran compiler.")
else()
  set(PNMPI_ENABLE_FORTRAN ON CACHE BOOL "Enables P^nMPI support for Fortran, requires a Fortran compiler.")
endif()

if (PNMPI_ENABLE_FORTRAN)
  enable_language(Fortran)
endif()

#===================================
# Basic initialization
#===================================
#Set version info
#TODO Martin set a reasonable version number
set(PNMPI_MAJOR_VERSION 1)
set(PNMPI_MINOR_VERSION 4)
set(PNMPI_PATCH_VERSION 0)
set(PNMPI_VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION})

#Set version of the P^nMPI interface (used to determine incompatibility with P^nMPI lib)
#TODO Martin set a reasonable version number
set(PNMPI_LIB_INTERFACE_VERSION "1")

#Set the module search path
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakemodules;${CMAKE_MODULE_PATH}")

#Set prefix
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (WIN32)
    set(CMAKE_INSTALL_PREFIX "$ENV{PROGRAMFILES}/pnmpi" CACHE PATH "CMAKE_INSTALL_PREFIX" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "CMAKE_INSTALL_PREFIX" FORCE)
  endif()
endif()

#Binary dir stuff
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Directory for built libraries.")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for built executables.")
set(INCLUDE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/include CACHE PATH "Intermediate directory for includes of P^nMPI.")
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH INCLUDE_OUTPUT_PATH)

#Add helper stuff
include(PnMPIModules)

#Find Packages
find_package(AWK)

#===================================
#System inspection
#===================================
# TODO
#     currently hardcoded, should be put into find modules:
#     * detect python
#     * detect gawk, tcsh, sort, uniq, sed
#     * detect BFD precense 
#     * dl open lib detection
find_package(MPI)

# Fortran name mangling option, customizable at cmd line
# TODO: detect fortran name mangling, e.g.: F77SYMBOL = symbol_
option(F77SYMBOL "Name demangling scheme." symbol_) 
set(MPI_CXX_SKIP_FLAGS
    "-DMPI_NO_CPPBIND" "-DOMPI_SKIP_MPICXX" "-D_MPICC_H" "-DMPICH_SKIP_MPICXX" "-DMPIBULL_SKIP_MPICXX"
    CACHE STRING "Default skip-flags used (for SGI, OpenMPI, HP-MPI, MPICH, BULL-MPI)")

#===================================
#Set global flags, include dirs, ...
#===================================
include_directories(BEFORE ${MPI_H_DIR})
set(PATCHER "${EXECUTABLE_OUTPUT_PATH}/patch")


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
endif()


#===================================
#Recurse to sub-directories
#===================================

#defines all the targets that exist, used to specify dependencies
set(TARGET_WRAPPERGEN   wrappergen)
set(TARGET_EXTRARGS     extrargs)
set(TARGET_PNMPILIB     pnmpi)
set(TARGET_PATCHER      patch)

add_subdirectory(wrappergen)
add_subdirectory(wrapper)
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

#TODO licence, documentation

#===================================
#Build a Package
#===================================
#TODO
