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
# @author Todd Gamblin, Tobias Hilbrich
# @date 22.05.2011


# add_pnmpi_module(targetname source1 source2 ... sourceN)
#
#=============================================================================
# This function adds a PnMPI module to the build and sets flags so that it
# is built and patched properly.
#
function(add_pnmpi_module targetname)
  list(LENGTH ARGN num_sources)
  if (num_sources LESS 1)
    message(FATAL_ERROR "add_pnmpi_module() called with no source files!")
  endif()

  # Add a library for the module, and set it up to omit the 'lib' prefix
  add_library(${targetname} MODULE ${ARGN})
  set_target_properties(${targetname} PROPERTIES PREFIX "")

  #For Apple set that undefined symbols should be looked up dynamically
  #(On linux this is already the default)
  if(APPLE)
    set_target_properties(${targetname} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  endif()

  # Patch the library in place once it's built so that it can be installed normally,
  # like any other library.  Also keeps build dir libraries consistent with installed libs.
  set(tmplib ${targetname}-unpatched.so)

  add_custom_command(TARGET ${targetname} POST_BUILD
    COMMAND mv                         ARGS $<TARGET_FILE:${targetname}> ${tmplib}
    COMMAND $<TARGET_FILE:pnmpi-patch> ARGS ${tmplib} $<TARGET_FILE:${targetname}>
    COMMAND rm                         ARGS -f ${tmplib}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Patching ${targetname}"
    VERBATIM)

  # Make sure that PnMPI lib and patch tool are built before this module.
  add_dependencies(${targetname} pnmpi-patch pnmpi)
endfunction()

####################################################
## Macro featureTestMpi
##
## Creates a feature test with an MPI program.
####################################################

MACRO (
    featureTestMpi
        source # The source file name of the MPI program to test, must be in the folder cmakemodules/FeatureTests
        language # one of: C, CXX, Fortran
        successVar # Name of a variable to set to true iff the test was successful, will be set as a CACHE variable
        )
    IF (NOT DEFINED ${successVar})
        #Organize the temporary src directory, copy source there
        SET (binDir "${CMAKE_CURRENT_BINARY_DIR}/${source}/BUILD")
        SET (srcDir "${CMAKE_CURRENT_BINARY_DIR}/${source}")
        CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmakemodules/FeatureTests/${source}" "${srcDir}/${source}" COPYONLY)

	set(HALT_ON_WARN "-Werror")
	IF (CMAKE_${language}_COMPILER MATCHES "xl")
		set(HALT_ON_WARN "-qhalt=w")
	ENDIF()
        IF (MPI_${language}_COMPILER)
           FILE(WRITE "${srcDir}/CMakeLists.txt"
               "PROJECT (test ${language})\n"
	       "set(CMAKE_${language}_FLAGS \"${HALT_ON_WARN}\")\n"
               "cmake_minimum_required(VERSION 2.6)\n"
               "set(CMAKE_${language}_COMPILER \"${MPI_${language}_COMPILER}\")\n"
               "add_definitions(${skipPrepare})\n"
               "add_executable(test \"${source}\")\n"
            )
        ELSE ()
           SET (TEMP_INCS "")
           FOREACH (INC_PATH ${MPI_${language}_INCLUDE_PATH})
               SET (TEMP_INCS "${TEMP_INCS} \"${INC_PATH}\"")
           ENDFOREACH ()

           #Create CMakeLists.txt
           FILE(WRITE "${srcDir}/CMakeLists.txt"
               "PROJECT (test ${language})\n"
               "cmake_minimum_required(VERSION 2.6)\n"
	       "set(CMAKE_${language}_FLAGS \"${HALT_ON_WARN}\")\n"
               "set(CMAKE_C_COMPILER \"${C_compiler_to_use}\")\n"
               "set(CMAKE_CXX_COMPILER \"${CXX_compiler_to_use}\")\n"
               "set(CMAKE_Fortran_COMPILER \"${Fortran_compiler_to_use}\")\n"
               "add_definitions(${skipPrepare})\n"
               "if (NOT \"${MPI_${language}_INCLUDE_PATH}\" STREQUAL \"\")\n"
               "    include_directories(${TEMP_INCS})\n"
               "endif (NOT \"${MPI_${language}_INCLUDE_PATH}\" STREQUAL \"\")\n"
               "add_executable(test \"${source}\")\n"
               "target_link_libraries(test ${MPI_${language}_LIBRARIES})\n"
            )
        ENDIF ()

        #If MPI include directories are a list, we need to concatenate it and add quotes to handle potential space characters correctly

        #Try compile and preserve the result in a cached variable
        try_compile(${successVar} "${binDir}" "${srcDir}"
                        test
	#  CMAKE_MODULE_PATH is needed to find the provided Toolchain/Platform files
			CMAKE_FLAGS "-DCMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'"
                        OUTPUT_VARIABLE output)

        SET (${successVar} ${${successVar}} CACHE INTERNAL "Result of feature testing ${source}")

        IF (GTI_VERBOSE)
            MESSAGE ("${output}")
        ENDIF (GTI_VERBOSE)

        #Add status
        SET (successStatus "success")
        IF (NOT ${successVar})
            SET (successStatus "failed")
        ENDIF (NOT ${successVar})

        MESSAGE (STATUS "Checking for ${source} ... ${successStatus}")
    ENDIF (NOT DEFINED ${successVar})
ENDMACRO (featureTestMpi)

# Macro featureTest
#
#=============================================================================
# Creates a feature test with an non-MPI program.
MACRO (
    featureTest
        source # The source file name of the MPI program to test, must be in the folder cmakemodules/FeatureTests
        language # one of: C, CXX, Fortran
        successVar # Name of a variable to set to true iff the test was successful, will be set as a CACHE variable
        )
    IF (NOT DEFINED ${successVar})
        #Organize the temporary src directory, copy source there
        SET (binDir "${CMAKE_CURRENT_BINARY_DIR}/${source}/BUILD")
        SET (srcDir "${CMAKE_CURRENT_BINARY_DIR}/${source}")
        CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmakemodules/FeatureTests/${source}" "${srcDir}/${source}" COPYONLY)

        #Create CMakeLists.txt
        FILE(WRITE "${srcDir}/CMakeLists.txt"
            "PROJECT (test ${language})\n"
            "cmake_minimum_required(VERSION 2.6)\n"
            "add_executable(test \"${source}\")\n"
            )

        #Try compile and preserve the result in a cached variable
        try_compile(${successVar} "${binDir}" "${srcDir}"
                        test
	#  CMAKE_MODULE_PATH is needed to find the provided Toolchain/Platform files
			CMAKE_FLAGS "-DCMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'"
                        OUTPUT_VARIABLE output)

        SET (${successVar} ${${successVar}} CACHE INTERNAL "Result of feature testing ${source}")

        #Add status
        SET (successStatus "success")
        IF (NOT ${successVar})
            SET (successStatus "failed")
        ENDIF (NOT ${successVar})

        MESSAGE (STATUS "Checking for ${source} ... ${successStatus}")
    ENDIF (NOT DEFINED ${successVar})
ENDMACRO (featureTest)

# Macro liblistToDependList
#
#=============================================================================
# Creates a list of dependent libraries for a given list of libraries; This dependency list should be used
# as dependency for shared libraries. This macro removes all static libraries from the input list.
MACRO (
    liblistToDependList
        libListVar
        dependListVar
        )
    set(${dependListVar} "")
    foreach(lib ${${libListVar}})
        if (NOT lib MATCHES "[.]a$$$")
            set(${dependListVar} ${${dependListVar}} "${lib}")
        endif ()
    endforeach(lib)
ENDMACRO (liblistToDependList)
