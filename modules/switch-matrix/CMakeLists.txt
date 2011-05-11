# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Definitions
set(WRAPDIR "../../wrapper")
set(MPISPEC "mpi_pnmpi")
set(PROTOFILE "${WRAPDIR}/${MPISPEC}_proto")
set(FCTFILE   "${WRAPDIR}/${MPISPEC}_fct")

#Use macro to: add target for module, version it, install it, patch it
pnmpi_mac_add_module(switch-matrix "switch-matrix.c" "C")
add_custom_target(switch-matrix-prepare ALL)
add_dependencies(switch-matrix switch-matrix-prepare)
add_dependencies(switch-matrix-prepare ${TARGET_PATCHER})

#Invoke generation of the empty.h header 
## - For details on that see the "wrapper" cmake build script
add_custom_command(
    TARGET switch-matrix-prepare PRE_BUILD 
    COMMAND ${WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_CURRENT_SOURCE_DIR}/switch-matrix.w" "-o" "switch-matrix.h"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating module sources for the \"switch-matrix\" module" 
    VERBATIM
    )
    
#Add the generated header into the include search path
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}) 
