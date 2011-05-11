# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# This files is partially from the MUST project and bears the follwing copyright.
# Copyright (c) 2009, ZIH, Technische Universitaet Dresden, Federal Republic of Germany
# Copyright (c) 2009, LLNL, United States of America
#
# @file PnMPIModules.cmake
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
macro(pnmpi_mac_patch_lib targetname patcher)
    #TODO: test whether this works with Windows pathes (spaces and such)
    #SET (lib ${CMAKE_SHARED_MODULE_PREFIX}${targetname}${CMAKE_SHARED_MODULE_SUFFIX}.${version})
    #Modules shouldn't have prefixes
    set(libin ${CMAKE_SHARED_MODULE_PREFIX}${targetname}${CMAKE_SHARED_MODULE_SUFFIX})
    set(libout ${targetname}${CMAKE_SHARED_MODULE_SUFFIX})
    
    file(WRITE ${PROJECT_BINARY_DIR}/install-scripts/patch-${libout}.cmake
        "message(\"Patching ${libout}\")\n"
        "execute_process(COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/lib/pnmpi-modules\n"
        "   RESULT_VARIABLE result\n"
        "   OUTPUT_VARIABLE output\n"
        "   ERROR_VARIABLE output)\n"
        "if(result)\n"
        "   message(FATAL_ERROR MKDIR MODULES \${output})\n"
        "endif()\n"
        "execute_process(COMMAND ${patcher}\n"
        "   ${LIBRARY_OUTPUT_PATH}/${libin} ${CMAKE_INSTALL_PREFIX}/lib/pnmpi-modules/${libout}\n"
        "   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/install-scripts\n"
        "   RESULT_VARIABLE result\n"
        "   OUTPUT_VARIABLE output\n"
        "   ERROR_VARIABLE output)\n"
        "if(result)\n"
        "   message(FATAL_ERROR PATCHING \${output})\n"
        "endif()\n"
        )
    install(SCRIPT ${PROJECT_BINARY_DIR}/install-scripts/patch-${libout}.cmake)
endmacro()

#=========================================================
# Macro
# Does:
#   Takes a targetname and a list of source files.
#   Adds a module with given name and sources, installs 
#   patches and versions it.
# 
# language: in C, CXX, FORTRAN
#=========================================================
macro(pnmpi_mac_add_module targetname sources language)
    #Add target and its dependency on the patcher
    add_library(${targetname} MODULE ${sources})
    add_dependencies(${targetname} ${TARGET_PATCHER})
    
    #Set lib version
    ## Removed versions here, MAC doesn't likes that, actually versions might really not make much sense for a module
    #SET_TARGET_PROPERTIES (${targetname} PROPERTIES
    #    VERSION ${PNMPI_VERSION}
    #    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION}
    #    )  
    
    #For Apple set that undefined symbols should be looked up dynamically
    #(On linux this is already the default)
    if(APPLE)
        set_target_properties(${targetname} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif()
       
     #Add avoid MPICXX header flags for C++                                                                                     
    if("${language}" STREQUAL "CXX")
        set(TEMP "")
        foreach(skip_flag ${MPI_CXX_SKIP_FLAGS})
            set(TEMP "${TEMP} ${skip_flag}")
        endforeach()
        set_target_properties(${targetname} PROPERTIES COMPILE_FLAGS "${TEMP}")
    endif()
    
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
    pnmpi_mac_patch_lib(
        ${targetname}
        ${PROJECT_BINARY_DIR}/patch/patch)
endmacro()
