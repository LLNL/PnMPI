# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Add include dir 
INCLUDE_DIRECTORIES(BEFORE "${CMAKE_BINARY_DIR}/include")
INCLUDE_DIRECTORIES(BEFORE "${CMAKE_SOURCE_DIR}")

#Create library targets
ADD_LIBRARY(${TARGET_PNMPILIB} STATIC 
    debug.c services.c core.c wrapper.c
    )
    
ADD_LIBRARY(${TARGET_PNMPILIB}_shared SHARED 
    debug.c services.c core.c wrapper.c
    )

ADD_LIBRARY(forcempi STATIC forcempi.c)

#Set properties for shared libraries
SET_TARGET_PROPERTIES (${TARGET_PNMPILIB}_shared
    PROPERTIES 
    VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION}
    )
    
#Add library dependency to MPI lib
TARGET_LINK_LIBRARIES(${TARGET_PNMPILIB}_shared ${MPI_LIBRARIES})

#Install pnmpi core with reasonable file permissions
INSTALL(
    TARGETS 
        ${TARGET_PNMPILIB} 
        ${TARGET_PNMPILIB}_shared 
        forcempi
    PERMISSIONS 
        OWNER_READ OWNER_WRITE OWNER_EXECUTE 
        GROUP_EXECUTE GROUP_READ 
        WORLD_EXECUTE
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )  
    
#Add dependencies
ADD_DEPENDENCIES(${TARGET_PNMPILIB}  
    ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
ADD_DEPENDENCIES(${TARGET_PNMPILIB}_shared  
    ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
ADD_DEPENDENCIES(forcempi            
    ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})

#copy header files to include directory in binary dir
CONFIGURE_FILE(pnmpi.h "${INCLUDE_OUTPUT_PATH}/pnmpi.h" COPYONLY)
CONFIGURE_FILE(pnmpimod.h "${INCLUDE_OUTPUT_PATH}/pnmpimod.h" COPYONLY) 

#Fortran PnMPI lib (If Fortran available)    
IF (PNMPI_ENABLE_FORTRAN)
    ADD_LIBRARY(${TARGET_PNMPILIB}f STATIC 
        debug.c services.c core.c wrapper.c         
        )
    
    ADD_LIBRARY(${TARGET_PNMPILIB}f_shared SHARED 
        debug.c services.c core.c wrapper.c         
        )
        
    #Add library dependency to MPI lib
    TARGET_LINK_LIBRARIES(${TARGET_PNMPILIB}f_shared ${MPI_LIBRARIES})

    #Add the fortran define for the fortran libs!
    SET_TARGET_PROPERTIES (
	    ${TARGET_PNMPILIB}f ${TARGET_PNMPILIB}f_shared
	    PROPERTIES COMPILE_FLAGS "-DCOMPILE_FOR_FORTRAN"
	    )
	    
	#Set properties for shared libraries
    SET_TARGET_PROPERTIES (${TARGET_PNMPILIB}f_shared
        PROPERTIES 
        VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
        SOVERSION ${PNMPI_LIB_INTERFACE_VERSION}
        )
        
    #Install pnmpi core for Fortran with reasonable file permissions
    INSTALL(
        TARGETS 
            ${TARGET_PNMPILIB}f 
            ${TARGET_PNMPILIB}f_shared
        PERMISSIONS 
            OWNER_READ OWNER_WRITE OWNER_EXECUTE 
            GROUP_EXECUTE GROUP_READ 
            WORLD_EXECUTE
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )      
        
    #Add dependencies
    ADD_DEPENDENCIES(${TARGET_PNMPILIB}f 
        ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
    ADD_DEPENDENCIES(${TARGET_PNMPILIB}f_shared 
        ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
ENDIF (PNMPI_ENABLE_FORTRAN)
    
