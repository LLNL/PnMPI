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
PNMPI_MAC_ADD_MODULE (virtual "virtual.c")
ADD_CUSTOM_TARGET(virtual-prepare ALL)
ADD_DEPENDENCIES(virtual virtual-prepare)
ADD_DEPENDENCIES(virtual-prepare ${TARGET_PATCHER})

#Invoke generation of the empty.h header 
## - For details on that see the "wrapper" cmake build script
ADD_CUSTOM_COMMAND(
    TARGET virtual-prepare PRE_BUILD 
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_CURRENT_SOURCE_DIR}/virtual.w" "-o" "virtual.h"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating module sources for the \"virtual\" module" 
    VERBATIM
    )
    
#Add the generated header into the include search path
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR}) 
