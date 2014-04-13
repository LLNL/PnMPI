# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

#Set names for targets
set(PREFIX commsub)
set(SUBMODULES print empty)
# TODO: checksum removed due to incompatibility with OpenMPI

#Loop over all targets (submods)
foreach(mod ${SUBMODULES})
    # Use macro to: add target for module, version it, install it, patch it
    set(modulename ${PREFIX}-${mod})
    add_pnmpi_module(${modulename} ${modulename}.cpp comm.cpp)

    #  Add the submodname define as a flag
    ## TODO: Martin, please have a look at the "SUBMODNAME" define, is that what you want ???
    set_source_files_properties(${modulename}.cpp COMPILE_FLAGS "-DSUBMODNAME='\"${mod}\"'")

    install(TARGETS ${modulename} DESTINATION lib/pnmpi-modules)
endforeach()

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/modules/datatype
  ${PROJECT_SOURCE_DIR}/modules/requests
  ${PROJECT_SOURCE_DIR}/modules/status
  ${PROJECT_BINARY_DIR}/src
  ${MPI_C_INCLUDE_PATH})
