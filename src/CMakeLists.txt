# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Add include dir 
include_directories(BEFORE
  "${CMAKE_BINARY_DIR}/include"
  ${MPI_C_INCLUDE_PATH})

#Create library targets
add_library(${TARGET_PNMPILIB} STATIC 
    debug.c services.c core.c wrapper.c)
    
add_library(${TARGET_PNMPILIB}_shared SHARED 
    debug.c services.c core.c wrapper.c)

add_library(forcempi STATIC forcempi.c)

#Set properties for shared libraries
set_target_properties(${TARGET_PNMPILIB}_shared
    PROPERTIES 
    VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION})
    
#Add library dependency to MPI lib
target_link_libraries(${TARGET_PNMPILIB}_shared ${MPI_C_LIBRARIES})

#Install pnmpi core with reasonable file permissions
install(
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
add_dependencies(${TARGET_PNMPILIB}  
    ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
add_dependencies(${TARGET_PNMPILIB}_shared  
    ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
add_dependencies(forcempi            
    ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})

#copy header files to include directory in binary dir
configure_file(pnmpi.h     "${INCLUDE_OUTPUT_PATH}/pnmpi.h"   COPYONLY)
configure_file(pnmpimod.h "${INCLUDE_OUTPUT_PATH}/pnmpimod.h" COPYONLY) 

#Fortran PnMPI lib (If Fortran available)    
if (PNMPI_ENABLE_FORTRAN)
  add_library(${TARGET_PNMPILIB}f STATIC 
    debug.c services.c core.c wrapper.c)
  
  add_library(${TARGET_PNMPILIB}f_shared SHARED 
    debug.c services.c core.c wrapper.c)
  
  #Add library dependency to MPI lib
  target_link_libraries(${TARGET_PNMPILIB}f_shared ${MPI_Fortran_LIBRARIES})
  
  #Add the fortran define for the fortran libs!
  set_target_properties(
	  ${TARGET_PNMPILIB}f ${TARGET_PNMPILIB}f_shared
	  PROPERTIES COMPILE_FLAGS "-DCOMPILE_FOR_FORTRAN")
	
	#Set properties for shared libraries
  set_target_properties (${TARGET_PNMPILIB}f_shared
    PROPERTIES 
    VERSION ${PNMPI_MAJOR_VERSION}.${PNMPI_MINOR_VERSION}.${PNMPI_PATCH_VERSION}
    SOVERSION ${PNMPI_LIB_INTERFACE_VERSION})
  
  #Install pnmpi core for Fortran with reasonable file permissions
  install(
    TARGETS 
      ${TARGET_PNMPILIB}f 
      ${TARGET_PNMPILIB}f_shared
    PERMISSIONS 
      OWNER_READ OWNER_WRITE OWNER_EXECUTE 
      GROUP_EXECUTE GROUP_READ 
      WORLD_EXECUTE
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)      
        
    #Add dependencies
    add_dependencies(${TARGET_PNMPILIB}f ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
    add_dependencies(${TARGET_PNMPILIB}f_shared ${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})
endif()
    
#Add linkage to adept utils if we have them
if (AdeptUtils_FOUND)
  include_directories(${AdeptUtils_INCLUDE_PATH})
  target_link_libraries(${TARGET_PNMPILIB} AdeptUtilsTiming)
  target_link_libraries(${TARGET_PNMPILIB}_shared AdeptUtilsTiming)

  if (PNMPI_ENABLE_FORTRAN)
    target_link_libraries(${TARGET_PNMPILIB}f AdeptUtilsTiming)
    target_link_libraries(${TARGET_PNMPILIB}f_shared AdeptUtilsTiming)
  endif()
endif()
