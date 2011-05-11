# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz, Todd Gamblin
# @date 2011-05-01

# Add linkage to adept utils if we have it
if (AdeptUtils_FOUND)
  include_directories(BEFORE ${AdeptUtils_INCLUDE_PATH})
  set(TIMING_LIB AdeptUtilsTiming)
endif()

# Sources for the PnMPI libraries used by all libraries.
set(PNMPI_LIB_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/newstack.h
  ${CMAKE_CURRENT_BINARY_DIR}/wrapper.h
  debug.c
  services.c
  core.c
  wrapper.c
  wrapper_c.c
  wrapper_f77.c)

# Invoke generation of wrappers as pre build event of wrapper lib target
function(add_wrapped_file file_name wrapper_name)
  set(file_path    ${CMAKE_CURRENT_BINARY_DIR}/${file_name})
  set(wrapper_path ${CMAKE_CURRENT_SOURCE_DIR}/${wrapper_name})
  add_custom_command(
    OUTPUT  ${file_path}
    COMMAND ${CMAKE_SOURCE_DIR}/wrap/wrap.py ARGS ${wrapper_path} -o ${file_path}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS ${wrapper_path}
    COMMENT "Generating ${file_name} from ${wrapper_name}"
    VERBATIM)
  set_source_files_properties(${file_path} PROPERTIES GENERATED TRUE)
endfunction()

add_wrapped_file(wrapper.h   wrapper.h.w)
add_wrapped_file(newstack.h  newstack.h.w)
add_wrapped_file(wrapper_c.c wrapper_c.c.w)

# Build fortran wrappers here
add_custom_command(
  OUTPUT wrapper_f77.c f77symbols.h
  COMMAND eval ARGS "python ${CMAKE_SOURCE_DIR}/wrap/makefortran.py --xlate --f77symbol ${F77SYMBOL} ${CMAKE_SOURCE_DIR}/wrap/mpi-f77.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating Fortran Wrappers"
  VERBATIM)
set_source_files_properties(wrapper_f77.c f77symbols.h PROPERTIES GENERATED TRUE)


# PnMPI shared library
add_library(pnmpi SHARED ${PNMPI_LIB_SOURCES})
target_link_libraries(pnmpi
  ${TIMING_LIB}
  ${MPI_C_LIBRARIES}
  -ldl)
set_target_properties(pnmpi PROPERTIES 
    VERSION   ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION})

# PnMPI static library (same output name as shared lib)
add_library(pnmpi_static STATIC ${PNMPI_LIB_SOURCES})
set_target_properties(pnmpi_static PROPERTIES OUTPUT_NAME pnmpi)

#copy header files to include directory in binary dir
install(FILES   pnmpi.h pnmpimod.h DESTINATION include)
install(TARGETS pnmpi_static pnmpi DESTINATION lib)

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
  set_target_properties(pnmpif PROPERTIES 
    VERSION       ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION     ${PNMPI_LIB_INTERFACE_VERSION})

  # PnMPI fortran static lib
  add_library(pnmpif_static STATIC ${PNMPI_LIB_SOURCES})
  set_target_properties(pnmpif_static PROPERTIES OUTPUT_NAME   pnmpif)
	
  #Install pnmpi core for Fortran with reasonable file permissions
  install(TARGETS pnmpif pnmpif_static DESTINATION lib)
endif()

# Extra directories to include from when building.
include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${MPI_C_INCLUDE_PATH})
