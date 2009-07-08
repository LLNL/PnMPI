# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Definitions
SET (WRAPDIR "../../wrapper")
SET (MPISPEC "mpi_pnmpi")
SET (PROTOFILE "${WRAPDIR}/${MPISPEC}_proto")
SET (FCTFILE   "${WRAPDIR}/${MPISPEC}_fct")

#Use macro to: add target for module, version it, install it, patch it
PNMPI_MAC_ADD_MODULE (switch-matrix "switch-matrix.c")
ADD_CUSTOM_TARGET(switch-matrix-prepare ALL)
ADD_DEPENDENCIES(switch-matrix switch-matrix-prepare)
ADD_DEPENDENCIES(switch-matrix-prepare ${TARGET_PATCHER})

#Invoke generation of the empty.h header 
## - For details on that see the "wrapper" cmake build script
ADD_CUSTOM_COMMAND(
    TARGET switch-matrix-prepare PRE_BUILD 
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_CURRENT_SOURCE_DIR}/switch-matrix.w" "-o" "switch-matrix.h"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating module sources for the \"switch-matrix\" module" 
    VERBATIM
    )
    
#Add the generated header into the include search path
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR}) 
