# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 2011-05-01

include(CheckSymbolExists)

find_package(Sanitizers)


# Add linkage to adept utils if we have it
if (PnMPI_USE_adept_utils)
  include_directories(BEFORE ${adept_utils_INCLUDE_PATH} ${adept-utils_INCLUDE_DIRS})
  set(TIMING_LIB adept_timing adept_cutils)
endif()

# Add wrapped files to the build.
add_wrapped_file(wrapper.h   wrapper.h.w)
add_wrapped_file(newstack.h  newstack.h.w)
add_wrapped_file(xmpi.h      xmpi.h.w)

if (ENABLE_FORTRAN)
  add_wrapped_file(wrapper_c.c wrapper_c.c.w -f)
else ()
  add_wrapped_file(wrapper_c.c wrapper_c.c.w)
endif ()

# Sources for the PnMPI libraries used by all libraries.
#
# Note: The apple linker does not support reordering of constructors, so files
#       containing a constructor must be listed in the order they should be
#       executed.
set(PNMPI_LIB_SOURCES
  debug.c
  services.c
  core.c
  wrapper.c
  app_startup.c
  app_shutdown.c
  get_mpi_interface.c
  call_hook.c
  hook_activated.c
)

if (ENABLE_FORTRAN)
  list(APPEND PNMPI_LIB_SOURCES ${Wrap_EXTRA_SOURCE})
endif ()

check_symbol_exists("__GNUC__" "" HAVE_GNUC)
if (NOT HAVE_GNUC)
  list(APPEND PNMPI_LIB_SOURCES
    "fallback_constructor.c" "fallback_destructor.c")
endif ()


add_custom_target(generate DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper_c.c
  ${CMAKE_CURRENT_BINARY_DIR}/newstack.h
  ${CMAKE_CURRENT_BINARY_DIR}/xmpi.h
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper.h)

# PnMPI shared library
add_library(pnmpi SHARED ${PNMPI_LIB_SOURCES})
add_dependencies(pnmpi generate)
add_coverage(pnmpi)
add_sanitizers(pnmpi)
target_link_libraries(pnmpi
  ${TIMING_LIB}
  ${MPI_C_LIBRARIES}
  -ldl)

set_target_properties(pnmpi PROPERTIES VERSION
  ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
  SOVERSION ${PNMPI_MAJOR_VERSION})

if(NOT HAVE_GNUC)
  set_property(TARGET pnmpi APPEND PROPERTY LINK_FLAGS " -nostartfiles")
endif()

 if(APPLE)
    get_target_property(TEMP_FLAG pnmpi LINK_FLAGS)
    string(TOUPPER "${TEMP_FLAG}" TEMP_CAPITAL_FLAG)
    if(${TEMP_CAPITAL_FLAG} STREQUAL "TEMP_FLAG-NOTFOUND")
      set(TEMP_FLAG "")
    endif()
    set (TEMP_FLAG "${TEMP_FLAG} -undefined dynamic_lookup")
    set_target_properties(pnmpi PROPERTIES LINK_FLAGS ${TEMP_FLAG})
  endif()

# PnMPI static library (same output name as shared lib)
add_library(pnmpi_static STATIC ${PNMPI_LIB_SOURCES})
add_dependencies(pnmpi_static generate)
set_target_properties(pnmpi_static PROPERTIES OUTPUT_NAME pnmpi)

#copy header files to include directory in binary dir
install(
  FILES   pnmpi.h pnmpimod.h ${CMAKE_CURRENT_BINARY_DIR}/newstack.h
    ${CMAKE_CURRENT_BINARY_DIR}/xmpi.h
  DESTINATION include)
install(TARGETS pnmpi_static pnmpi EXPORT pnmpi-libs DESTINATION lib)

#
# Fortran support (If Fortran was enabled)
#
if (CMAKE_Fortran_COMPILER_WORKS)
  # PnMPI fortran shared library
  add_library(pnmpif SHARED ${PNMPI_LIB_SOURCES})
  add_coverage(pnmpif)
  add_sanitizers(pnmpif)
  add_dependencies(pnmpif generate)
  target_link_libraries(pnmpif
    ${TIMING_LIB}
    ${MPI_Fortran_LIBRARIES}
    -ldl)

  set_target_properties(pnmpif PROPERTIES VERSION
    ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_MAJOR_VERSION})

  if(NOT HAVE_GNUC)
    set_property(TARGET pnmpif APPEND PROPERTY LINK_FLAGS " -nostartfiles")
  endif()

  if(CMAKE_Fortran_COMPILER_WORKS)
    get_target_property(TEMP_FLAG pnmpif COMPILE_FLAGS)
    string(TOUPPER "${TEMP_FLAG}" TEMP_CAPITAL_FLAG)
    if(${TEMP_CAPITAL_FLAG} STREQUAL "TEMP_FLAG-NOTFOUND")
      set(TEMP_FLAG "")
    endif()
    set(TEMP_FLAG "${TEMP_FLAG} -DCOMPILE_FOR_FORTRAN")
    set_target_properties(pnmpif PROPERTIES COMPILE_FLAGS ${TEMP_FLAG})
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

  # PnMPI fortran static lib
  add_library(pnmpif_static STATIC ${PNMPI_LIB_SOURCES})
  add_dependencies(pnmpif_static generate)
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
