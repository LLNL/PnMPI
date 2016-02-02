# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 2011-05-01

# Add linkage to adept utils if we have it
if (PnMPI_USE_adept_utils)
  include_directories(BEFORE ${adept_utils_INCLUDE_PATH} ${adept-utils_INCLUDE_DIRS})
  set(TIMING_LIB adept_timing adept_cutils)
endif()

# Add wrapped files to the build.
add_wrapped_file(wrapper.h   wrapper.h.w)
add_wrapped_file(newstack.h  newstack.h.w)
add_wrapped_file(wrapper_c.c wrapper_c.c.w -f)

# Sources for the PnMPI libraries used by all libraries.
set(PNMPI_LIB_SOURCES
  # -- Regular sources --
  debug.c
  services.c
  core.c
  wrapper.c
  ${Wrap_EXTRA_SOURCE}
  # -- Generated sources --
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper_c.c
#   wrapper_f77.c
  # Note: Generated headers must be added to enforce dependency.
  ${CMAKE_CURRENT_BINARY_DIR}/newstack.h
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper.h)

# PnMPI shared library
add_library(pnmpi SHARED ${PNMPI_LIB_SOURCES})
add_coverage(pnmpi)
target_link_libraries(pnmpi
  ${TIMING_LIB}
  ${MPI_C_LIBRARIES}
  -ldl)
set_target_properties(pnmpi PROPERTIES
    VERSION   ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION})
 if(APPLE)
    get_target_property(TEMP_FLAG pnmpi LINK_FLAGS)
    string(TOUPPER "${TEMP_FLAG}" TEMP_CAPITAL_FLAG)
    if(${TEMP_CAPITAL_FLAG} STREQUAL "TEMP_FLAG-NOTFOUND")
      set(TEMP_FLAG "")
    endif()
    set (TEMP_FLAG "${TEMP_FLAG} -undefined dynamic_lookup")
    set_target_properties(pnmpi PROPERTIES LINK_FLAGS ${TEMP_FLAG})
  endif()
  if(PNMPI_HAVE_PTHREADS)
    get_target_property(TEMP_FLAG pnmpi LINK_FLAGS)
    string(TOUPPER "${TEMP_FLAG}" TEMP_CAPITAL_FLAG)
    if(${TEMP_CAPITAL_FLAG} STREQUAL "TEMP_FLAG-NOTFOUND")
      set(TEMP_FLAG "")
    endif()
    if(NOT PNMPI_HAVE_GNUC)
      message(STATUS "BUILDING WITHOUT GNUC EXTENSIONS")
      set(TEMP_FLAG "${TEMP_FLAG} -nostartfiles")
      set_target_properties(pnmpi PROPERTIES LINK_FLAGS ${TEMP_FLAG})
    else()
      message(STATUS "BUILDING WITH GNUC EXTENSIONS")
    endif()
  endif()

# PnMPI static library (same output name as shared lib)
add_library(pnmpi_static STATIC ${PNMPI_LIB_SOURCES})
set_target_properties(pnmpi_static PROPERTIES OUTPUT_NAME pnmpi)

#copy header files to include directory in binary dir
install(
  FILES   pnmpi.h pnmpimod.h ${CMAKE_CURRENT_BINARY_DIR}/newstack.h
  DESTINATION include)
install(TARGETS pnmpi_static pnmpi EXPORT pnmpi-libs DESTINATION lib)

#
# Fortran support (If Fortran was enabled)
#
if (CMAKE_Fortran_COMPILER_WORKS)
  # PnMPI fortran shared library
  add_library(pnmpif SHARED ${PNMPI_LIB_SOURCES})
  target_link_libraries(pnmpif
    ${TIMING_LIB}
    ${MPI_Fortran_LIBRARIES}
    -ldl)
#  set_target_properties(pnmpif PROPERTIES
#    VERSION       ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
#    SOVERSION     ${PNMPI_LIB_INTERFACE_VERSION})

  if(CMAKE_Fortran_COMPILER_WORKS)
    set_target_properties(pnmpif PROPERTIES COMPILE_FLAGS "-DCOMPILE_FOR_FORTRAN")
  endif()
  if(APPLE)
    get_target_property(TEMP_FLAG pnmpif LINK_FLAGS)
    string(TOUPPER "${TEMP_FLAG}" TEMP_CAPITAL_FLAG)
    if(${TEMP_CAPITAL_FLAG} STREQUAL "TEMP_FLAG-NOTFOUND")
      set(TEMP_FLAG "")
    endif()
    set(TEMP_FLAG "${TEMP_FLAG} -undefined dynamic_lookup")
    set_target_properties(pnmpif PROPERTIES LINK_FLAGS ${TEMP_FLAG})
  endif()
  if(PNMPI_HAVE_PTHREADS)
    get_target_property(TEMP_FLAG pnmpif LINK_FLAGS)
    string(TOUPPER "${TEMP_FLAG}" TEMP_CAPITAL_FLAG)
    if(${TEMP_CAPITAL_FLAG} STREQUAL "TEMP_FLAG-NOTFOUND")
      set(TEMP_FLAG "")
    endif()
    if(NOT PNMPI_HAVE_GNUC)
      message(STATUS "BUILDING WITHOUT GNUC EXTENSIONS")
      set(TEMP_FLAG "${TEMP_FLAG} -nostartfiles")
      set_target_properties(pnmpif PROPERTIES LINK_FLAGS ${TEMP_FLAG})
    endif()
  endif()

  # PnMPI fortran static lib
  add_library(pnmpif_static STATIC ${PNMPI_LIB_SOURCES})
  set_target_properties(pnmpif_static PROPERTIES OUTPUT_NAME   pnmpif)
  set_target_properties(pnmpif_static PROPERTIES COMPILE_FLAGS "-DCOMPILE_FOR_FORTRAN")

  #Install pnmpi core for Fortran with reasonable file permissions
  install(TARGETS pnmpif pnmpif_static EXPORT pnmpi-libs DESTINATION lib)
endif()

# file to export pnmpi-libs
install(EXPORT pnmpi-libs DESTINATION share/cmake/PnMPI)

# Extra directories to include from when building.
include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${MPI_C_INCLUDE_PATH})
