# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 2011-05-01

# Add linkage to adept utils if we have it
if (adept_utils_FOUND)
  include_directories(BEFORE ${adept_utils_INCLUDE_PATH})
  set(TIMING_LIB adept_timing adept_cutils)
endif()

# Add wrapped files to the build.
add_wrapped_file(wrapper.h   wrapper.h.w)
add_wrapped_file(newstack.h  newstack.h.w)
add_wrapped_file(wrapper_c.c wrapper_c.c.w)

# Sources for the PnMPI libraries used by all libraries.
set(PNMPI_LIB_SOURCES
  # -- Regular sources --
  debug.c
  services.c
  core.c
  wrapper.c
  # -- Generated sources --
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper_c.c
  wrapper_f77.c
  # Note: Generated headers must be added to enforce dependency.
  ${CMAKE_CURRENT_BINARY_DIR}/newstack.h
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper.h)

# Build fortran wrappers here
add_custom_command(
  OUTPUT wrapper_f77.c f77symbols.h
  COMMAND eval ARGS "${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/wrap/makefortran.py --xlate --f77symbol ${F77SYMBOL} ${CMAKE_SOURCE_DIR}/wrap/mpi-f77.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating Fortran Wrappers"
  VERBATIM)
set_source_files_properties(wrapper_f77.c f77symbols.h PROPERTIES GENERATED TRUE)
get_directory_property(cleanfiles ADDITIONAL_MAKE_CLEAN_FILES)
list(APPEND cleanfiles wrapper_f77.c f77symbols.h)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${cleanfiles}")

# PnMPI shared library
add_library(pnmpi SHARED ${PNMPI_LIB_SOURCES})
target_link_libraries(pnmpi
  ${TIMING_LIB}
  ${MPI_C_LIBRARIES}
  -ldl)
set_target_properties(pnmpi PROPERTIES
    VERSION   ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION})
 if(APPLE)
    get_target_property(TEMP_FLAG pnmpi LINK_FLAGS)
    set (TEMP_FLAG "${TEMP_FLAG} -undefined dynamic_lookup")
    set_target_properties(pnmpi PROPERTIES LINK_FLAGS ${TEMP_FLAG})
  endif()
  if(PNMPI_HAVE_PTHREADS)
    get_target_property(TEMP_FLAG pnmpi LINK_FLAGS)
    set (TEMP_FLAG "${TEMP_FLAG} -nostartfiles")
    set_target_properties(pnmpi PROPERTIES LINK_FLAGS ${TEMP_FLAG})
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
    set (TEMP_FLAG "${TEMP_FLAG} -undefined dynamic_lookup")
    set_target_properties(pnmpif PROPERTIES LINK_FLAGS ${TEMP_FLAG})
  endif()
  if(PNMPI_HAVE_PTHREADS)
    get_target_property(TEMP_FLAG pnmpif LINK_FLAGS)
    set (TEMP_FLAG "${TEMP_FLAG} -nostartfiles")
    set_target_properties(pnmpif PROPERTIES LINK_FLAGS ${TEMP_FLAG})
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
