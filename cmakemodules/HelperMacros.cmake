# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# This files is partially from the MUST project and bears the follwing copyright.
# Copyright (c) 2009, ZIH, Technische Universität Dresden, Federal Republic of Germany
# Copyright (c) 2009, LLNL, United States of America
#
# @file HelperMacros.cmake
#       Contains helpful macros.
#
# @author Tobias Hilbrich
# @date 16.03.2009

#=========================================================
# Macro
# Does:
#   Gets the name of a library, Gets the path to the 
#   P^nMPI patcher, Creates a CMake script in the binary
#   directory which executes the patcher for the given 
#   library. The result is placed in the P^nMPI library 
#   directory. The script is added as a install script.
#   (executed at install time)
#=========================================================
MACRO (PNMPI_MAC_PATCH_LIB targetname version patcher)
    #TODO: test whether this works with Windows pathes (spaces and such)
    #SET (lib ${CMAKE_SHARED_MODULE_PREFIX}${targetname}${CMAKE_SHARED_MODULE_SUFFIX}.${version})
    #Modules shouldn't have prefixes
    SET (libin ${CMAKE_SHARED_MODULE_PREFIX}${targetname}${CMAKE_SHARED_MODULE_SUFFIX}.${version})
    SET (libout ${targetname}${CMAKE_SHARED_MODULE_SUFFIX})
    
    FILE (WRITE ${PROJECT_BINARY_DIR}/install-scripts/patch-${libout}.cmake 
        "MESSAGE (\"Patching ${lib}\")\n"
        "EXECUTE_PROCESS (COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/modules/"
        "   RESULT_VARIABLE result"
        "   OUTPUT_VARIABLE output"
        "   ERROR_VARIABLE output)\n"
        "IF (result)\n"
        "   MESSAGE (FATAL_ERROR MKDIR MODULES \${output})\n"
        "ENDIF (result)\n"
        "EXECUTE_PROCESS (COMMAND ${patcher}"
        "   ${LIBRARY_OUTPUT_PATH}/${libin} ${CMAKE_INSTALL_PREFIX}/modules/${libout}"
        "   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/install-scripts"
        "   RESULT_VARIABLE result"
        "   OUTPUT_VARIABLE output"
        "   ERROR_VARIABLE output)\n"
        "IF (result)\n"
        "   MESSAGE (FATAL_ERROR PATCHING \${output})\n"
        "ENDIF (result)\n"
        )
    INSTALL (SCRIPT ${PROJECT_BINARY_DIR}/install-scripts/patch-${libout}.cmake)
ENDMACRO (PNMPI_MAC_PATCH_LIB)

#=========================================================
# Macro
# Does:
#   Takes a targetname and a list of source files.
#   Adds a module with given name and sources, installs 
#   patches and versions it.
#=========================================================
MACRO (PNMPI_MAC_ADD_MODULE targetname sources)
    #Add target and its dependency on the patcher
    ADD_LIBRARY(${targetname} MODULE ${sources})
    ADD_DEPENDENCIES(${targetname} ${TARGET_PATCHER})
    
    #Set lib version
    SET_TARGET_PROPERTIES (${targetname} PROPERTIES 
        VERSION ${PNMPI_VERSION}
        SOVERSION ${PNMPI_LIB_INTERFACE_VERSION}
        )    

    #Install it with reasonable file permissions
    #INSTALL(TARGETS ${targetname}
    #    PERMISSIONS 
    #        OWNER_READ OWNER_WRITE OWNER_EXECUTE 
    #        GROUP_EXECUTE GROUP_READ 
    #        WORLD_EXECUTE
    #    RUNTIME DESTINATION bin
    #    LIBRARY DESTINATION modules
    #    ARCHIVE DESTINATION modules
    #    )
    
    #Patch it during installation
    PNMPI_MAC_PATCH_LIB (
        ${targetname}
        ${PNMPI_VERSION}
        ${PATCHER}
        )
ENDMACRO (PNMPI_MAC_ADD_MODULE)
