# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

##Add targets
#C
add_executable(simple    simple.c)
add_executable(simple-pn simple.c)
add_executable(simple-s1 simple.c)

target_link_libraries(simple                        ${MPI_C_LIBRARIES})
target_link_libraries(simple-pn pnmpi               ${MPI_C_LIBRARIES})
target_link_libraries(simple-s1 sample1-non-patched ${MPI_C_LIBRARIES})

configure_file(.pnmpi-conf ${CMAKE_CURRENT_BINARY_DIR}/.pnmpi-conf COPY_ONLY)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# Fortran (if we have it)
if (CMAKE_Fortran_COMPILER_WORKS)
  add_executable(fdemo    fdemo.f)
  add_executable(fdemo-pn fdemo.f)
  add_executable(fdemo-s1 fdemo.f)
  add_executable(f90demo    fdemo.f90)
  add_executable(f90demo-pn fdemo.f90)
  add_executable(f90demo-s1 fdemo.f90)

  target_link_libraries(fdemo                        ${MPI_Fortran_LIBRARIES})
  target_link_libraries(fdemo-pn pnmpif              ${MPI_Fortran_LIBRARIES})
  target_link_libraries(fdemo-s1 sample1-non-patched ${MPI_Fortran_LIBRARIES})
  target_link_libraries(f90demo                        ${MPI_Fortran_LIBRARIES})
  target_link_libraries(f90demo-pn pnmpif              ${MPI_Fortran_LIBRARIES})
  target_link_libraries(f90demo-s1 sample1-non-patched ${MPI_Fortran_LIBRARIES})
endif()

set(npflag "-np")
if(MPIEXEC_NUMPROC_FLAG)
  set(npflag ${MPIEXEC_NUMPROC_FLAG})
endif(MPIEXEC_NUMPROC_FLAG)
if(NOT DEFINED MPIEXEC_EXTRA_FLAG)
  set(MPIEXEC_NUMPROC_FLAG "")
endif()

# Add C tests
add_test(simple ${MPIEXEC} ${npflag} 2 ${MPIEXEC_EXTRA_FLAG} ${EXECUTABLE_OUTPUT_PATH}/simple)
add_test(simple-s1 ${MPIEXEC} ${npflag} 2 ${MPIEXEC_EXTRA_FLAG} ${EXECUTABLE_OUTPUT_PATH}/simple-s1)
add_test(
  simple-pn
  ${MPIEXEC}
  -x PNMPI_LIB_PATH=${CMAKE_INSTALL_PREFIX}/lib/pnmpi-modules
  -x LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$LD_LIBRARY_PATH
  -x DYLD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$DYLD_LIBRARY_PATH
  -x PNMPI_CONF=${CMAKE_SOURCE_DIR}/demo/.pnmpi-conf-cmake
  ${npflag} 2 ${EXECUTABLE_OUTPUT_PATH}/simple-pn)

#Add Fortran tests
if (CMAKE_Fortran_COMPILER_WORKS)
  add_test(fdemo    ${MPIEXEC} ${npflag} 2 ${MPIEXEC_EXTRA_FLAG} ${EXECUTABLE_OUTPUT_PATH}/fdemo)
  add_test(fdemo-s1 ${MPIEXEC} ${npflag} 2 ${MPIEXEC_EXTRA_FLAG} ${EXECUTABLE_OUTPUT_PATH}/fdemo-s1)
  add_test(
    fdemo-pn
    ${MPIEXEC}
    -x PNMPI_LIB_PATH=${CMAKE_INSTALL_PREFIX}/lib/pnmpi-modules
    -x LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$LD_LIBRARY_PATH
    -x DYLD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$DYLD_LIBRARY_PATH
    -x PNMPI_CONF=${CMAKE_SOURCE_DIR}/demo/.pnmpi-conf-cmake
    ${npflag} 2 ${EXECUTABLE_OUTPUT_PATH}/fdemo-pn)

  add_test(f90demo    ${MPIEXEC} ${npflag} 2 ${MPIEXEC_EXTRA_FLAG} ${EXECUTABLE_OUTPUT_PATH}/f90demo)
  add_test(f90demo-s1 ${MPIEXEC} ${npflag} 2 ${MPIEXEC_EXTRA_FLAG} ${EXECUTABLE_OUTPUT_PATH}/f90demo-s1)
  add_test(
    f90demo-pn
    ${MPIEXEC}
    -x PNMPI_LIB_PATH=${CMAKE_INSTALL_PREFIX}/lib/pnmpi-modules
    -x LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$LD_LIBRARY_PATH
    -x DYLD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$DYLD_LIBRARY_PATH
    -x PNMPI_CONF=${CMAKE_SOURCE_DIR}/demo/.pnmpi-conf-cmake
    ${npflag} 2 ${EXECUTABLE_OUTPUT_PATH}/f90demo-pn)
endif()

include_directories(
  ${MPI_C_INCLUDE_PATH}
  ${MPI_Fortran_INCLUDE_PATH})

#
# TODO: do something with .pnmpi-conf-democonf
#
