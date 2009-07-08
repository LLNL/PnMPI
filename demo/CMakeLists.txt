# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Set MPI compiler
SET(CMAKE_C_COMPILER "${MPICC}")
SET(CMAKE_Fortran_COMPILER "${MPIF77}")

#What libraries and flags are needed ?
IF (CMAKE_SYSTEM MATCHES "AIX")
    SET (LIBS ${TARGET_PNMPILIB}_shared forcempi)
    ADD_DEFINITIONS("-brtl")
ELSE(CMAKE_SYSTEM MATCHES "AIX")
    SET (LIBS ${TARGET_PNMPILIB}_shared) 
ENDIF(CMAKE_SYSTEM MATCHES "AIX")


##Add targets
#C
ADD_EXECUTABLE(simple simple.c)
ADD_EXECUTABLE(simple-pn simple.c)
ADD_EXECUTABLE(simple-s1 simple.c)

TARGET_LINK_LIBRARIES(simple-pn ${LIBS})
TARGET_LINK_LIBRARIES(simple-s1 sample1-non-patched dl) 

#Fortran (if enabled)
IF (PNMPI_ENABLE_FORTRAN)
    ADD_EXECUTABLE(fdemo fdemo.f)
    ADD_EXECUTABLE(fdemo-pn fdemo.f)
    ADD_EXECUTABLE(fdemo-s1 fdemo.f)
    
    TARGET_LINK_LIBRARIES(fdemo-pn ${LIBS})
    TARGET_LINK_LIBRARIES(fdemo-s1 sample1-non-patched dl)    
ENDIF (PNMPI_ENABLE_FORTRAN)

##Add tests
#C
ADD_TEST(
    simple ${MPIRUN} -np 2 ${EXECUTABLE_OUTPUT_PATH}/simple
    )
    
ADD_TEST(
    simple-s1 ${MPIRUN} -np 2 ${EXECUTABLE_OUTPUT_PATH}/simple-s1
    )

ADD_TEST(
    simple-pn
    ${MPIRUN} 
        -x PNMPI_LIB_PATH=${CMAKE_INSTALL_PREFIX}/modules
        -x LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/modules:${CMAKE_INSTALL_PREFIX}/lib:$LD_LIBRARY_PATH
        -x PNMPI_CONF=${CMAKE_SOURCE_DIR}/demo/.pnmpi-conf-cmake
        -np 2 ${EXECUTABLE_OUTPUT_PATH}/simple-pn
    )

#Fortran
IF (PNMPI_ENABLE_FORTRAN)
    ADD_TEST(
        fdemo ${MPIRUN} -np 2 ${EXECUTABLE_OUTPUT_PATH}/fdemo
        )
        
    ADD_TEST(
        fdemo-s1 ${MPIRUN} -np 2 ${EXECUTABLE_OUTPUT_PATH}/fdemo-s1
        )
    
    ADD_TEST(
        fdemo-pn
        ${MPIRUN} 
            -x PNMPI_LIB_PATH=${CMAKE_INSTALL_PREFIX}/modules
            -x LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/modules:$LD_LIBRARY_PATH
            -x PNMPI_CONF=${CMAKE_SOURCE_DIR}/demo/.pnmpi-conf-cmake
            -np 2 ${EXECUTABLE_OUTPUT_PATH}/fdemo-pn
        )
ENDIF (PNMPI_ENABLE_FORTRAN)
 