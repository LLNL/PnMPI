# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2016 Lawrence Livermore National Laboratories, United States of America
#  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

include(CheckSymbolExists)
include(easylib)
include(PnMPI_headers)

find_package(Sanitizers)


# Recurse into subdirectories.
#
# This has to be done BEFORE adding the PnMPI libraries, because for adding
# PnMPI with its object libraries, the easylib functions need to know about the
# available object libraries. Otherwise the compilation of PnMPI will result in
# errors, as easylib would select the static object library for the shared PnMPI
# library.
add_subdirectory(attributes)
add_subdirectory(debug)


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
  max_module_threading_level.c

  $<TARGET_OBJECTS:pnmpi_debug>
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
easy_add_library(pnmpi SHARED ${PNMPI_LIB_SOURCES})
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
easy_add_library(pnmpi_static STATIC ${PNMPI_LIB_SOURCES})
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
  easy_add_library(pnmpif SHARED ${PNMPI_LIB_SOURCES})
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
  easy_add_library(pnmpif_static STATIC ${PNMPI_LIB_SOURCES})
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
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PNMPI_HEADER_DIR}
  ${MPI_C_INCLUDE_PATH})
