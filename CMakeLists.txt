# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT (pnmpi C CXX)

#Enable this line for debugging
#SET (CMAKE_VERBOSE_MAKEFILE 1)

#===================================
# Fortran or not
#===================================
# TODO:
#       This should be extended to have a test being executed here,
#       which requires a Fortran compiler. Enable Fortran if the test
#       succeeds and disable it otherwise.
#
IF (WIN32)
  SET (PNMPI_ENABLE_FORTRAN OFF CACHE BOOL "Enables P^nMPI support for Fortran, requires a Fortran compiler.")
ELSE (WIN32)
  SET (PNMPI_ENABLE_FORTRAN ON CACHE BOOL "Enables P^nMPI support for Fortran, requires a Fortran compiler.")
ENDIF (WIN32)

IF (PNMPI_ENABLE_FORTRAN)
  ENABLE_LANGUAGE(Fortran)
ENDIF (PNMPI_ENABLE_FORTRAN)

#===================================
# Basic initialization
#===================================
#Set version info
#TODO Martin set a reasonable version number
SET(PNMPI_MAJOR_VERSION 1)
SET(PNMPI_MINOR_VERSION 0)
SET(PNMPI_PATCH_VERSION 0)
SET(PNMPI_VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION})

#Set version of the P^nMPI interface (used to determine incompatibility with P^nMPI lib)
#TODO Martin set a reasonable version number
SET (PNMPI_LIB_INTERFACE_VERSION "1")

#Set the module search path
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakemodules;${CMAKE_MODULE_PATH}")

#Set prefix
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  IF (WIN32)
    SET(CMAKE_INSTALL_PREFIX "$ENV{PROGRAMFILES}/pnmpi" CACHE PATH "CMAKE_INSTALL_PREFIX" FORCE)
  ELSE (WIN32)
    SET(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "CMAKE_INSTALL_PREFIX" FORCE)
  ENDIF (WIN32)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#Binary dir stuff
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Directory for built libraries.")
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Directory for built executables.")
SET (INCLUDE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/include CACHE PATH "Intermediate directory for includes of P^nMPI.")
MARK_AS_ADVANCED (LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH INCLUDE_OUTPUT_PATH)

#Add helper stuff
INCLUDE(HelperMacros)

#===================================
#System inspection
#===================================
# TODO
#      currently hardcoded, should be put into find modules
#
##    * find MPICC, MPIF77, MPIRUN, MPI_H_DIR
##    * detect python
##    * detect fortran name mangling, e.g.: F77SYMBOL = symbol_
##    * detect gawk, tcsh, sort, uniq, sed
##    * detect BFD precense 
##    * dl open lib detection
SET (MPI_H_DIR "/home/hilbrich/local/openmpi/include" CACHE PATH "Path to MPI header file." ) #path to mpi header
SET (MPICC "mpicc"  CACHE FILEPATH "MPI C compiler." ) #mpi C compiler
SET (MPIF77 "mpif77" CACHE FILEPATH "MPI Fortran77 compiler.") #mpi Fortran compiler
SET (MPIRUN "mpirun" CACHE FILEPATH "MPI mpiexec command.") #mpirun command
SET(F77SYMBOL symbol_ CACHE STRING "Name demangling scheme.") #Fortran name mangling

#===================================
#Set global flags, include dirs, ...
#===================================
INCLUDE_DIRECTORIES(BEFORE ${MPI_H_DIR})
SET (PATCHER "${EXECUTABLE_OUTPUT_PATH}/patch")

#===================================
#Recurse to sub-directories
#===================================

#defines all the targets that exist, used to specify dependencies
SET(TARGET_WRAPPERGEN   wrappergen)
SET(TARGET_EXTRARGS     extrargs)
SET(TARGET_PNMPILIB     pnmpi)
SET(TARGET_PATCHER      patch)

ADD_SUBDIRECTORY(wrappergen)
ADD_SUBDIRECTORY(wrapper)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(patch)
ADD_SUBDIRECTORY(modules)
ENABLE_TESTING() #activates creation of tests
ADD_SUBDIRECTORY(demo)

#===================================
#Other install stuff (which is not handled in the sub-dirs)
#===================================
#Install includes (collected and copied during build process)
INSTALL (DIRECTORY ${INCLUDE_OUTPUT_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})

#TODO licence, documentation

#===================================
#Build a Package
#===================================
#TODO
