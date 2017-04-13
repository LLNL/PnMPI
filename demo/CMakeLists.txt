# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2017 Lawrence Livermore National Laboratories, United States of America
#  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2017 RWTH Aachen University, Federal Republic of Germany
#
#
# P^nMPI is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation version 2.1 dated February 1999.
#
# P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with P^nMPI; if not, write to the
#
#   Free Software Foundation, Inc.
#   51 Franklin St, Fifth Floor
#   Boston, MA 02110, USA
#
#
# Written by Martin Schulz, schulzm@llnl.gov.
#
# LLNL-CODE-402774

##Add targets
#C
add_executable(simple    simple.c)
add_executable(simple-pn simple.c)
add_executable(simple-s1 simple.c)

add_coverage(simple)
add_coverage(simple-pn)
add_coverage(simple-s1)

add_sanitizers(simple)
add_sanitizers(simple-pn)
add_sanitizers(simple-s1)

target_link_libraries(simple                        ${MPI_C_LIBRARIES})
target_link_libraries(simple-pn pnmpi               ${MPI_C_LIBRARIES})
target_link_libraries(simple-s1 sample1-non-patched ${MPI_C_LIBRARIES})

configure_file(.pnmpi-conf ${CMAKE_CURRENT_BINARY_DIR}/.pnmpi-conf)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# Fortran (if we have it)
if (CMAKE_Fortran_COMPILER_WORKS)
  add_executable(fdemo    fdemo.f)
  add_executable(fdemo-pn fdemo.f)
  add_executable(fdemo-s1 fdemo.f)
  add_executable(f90demo    fdemo.f90)
  add_executable(f90demo-pn fdemo.f90)
  add_executable(f90demo-s1 fdemo.f90)

  target_link_libraries(fdemo-pn pnmpif)
  target_link_libraries(fdemo-s1 sample1-non-patched)
  target_link_libraries(f90demo-pn pnmpif)
  target_link_libraries(f90demo-s1 sample1-non-patched)

  foreach (TARGET fdemo fdemo-pn fdemo-s1 f90demo f90demo-pn f90demo-s1)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
      set_target_properties(${TARGET} PROPERTIES
        LINK_FLAGS "-Wl,--no-as-needed")
    endif()

    add_coverage(${TARGET})
    add_sanitizers(${TARGET})
    target_link_libraries(${TARGET} ${MPI_Fortran_LIBRARIES})
  endforeach ()
endif()

if(NOT MPIEXEC_NUMPROC_FLAG)
  set(MPIEXEC_NUMPROC_FLAG "-np")
endif()


function(set_sanitizer_env testname)
  if (SANITIZE_ADDRESS)
    set_tests_properties(${testname} PROPERTIES
      ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
  endif ()
endfunction()

function(set_pnmpi_env testname)
  get_test_property(${testname} ENVIRONMENT TEMP)
  if (TEMP STREQUAL "TEMP-NOTFOUND")
    set(TEMP "")
  endif ()

  list(APPEND TEMP
    "PNMPI_LIB_PATH=${PROJECT_BINARY_DIR}/modules/sample"
    "PNMPI_CONF=${PROJECT_SOURCE_DIR}/demo/.pnmpi-conf-cmake"
  )

  set_tests_properties(${testname} PROPERTIES ENVIRONMENT "${TEMP}")
endfunction()


# Add C tests
add_test(simple ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
  ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/simple ${MPIEXEC_POSTFLAGS})
add_test(simple-s1 ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
  ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/simple-s1 ${MPIEXEC_POSTFLAGS})
add_test(simple-pn ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
  ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/simple-pn ${MPIEXEC_POSTFLAGS})

foreach (TARGET simple simple-s1 simple-pn)
  set_sanitizer_env(${TARGET})
endforeach ()
set_pnmpi_env(simple-pn)


#Add Fortran tests
if (CMAKE_Fortran_COMPILER_WORKS)
  add_test(fdemo ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/fdemo ${MPIEXEC_POSTFLAGS})
  add_test(fdemo-s1 ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/fdemo-s1 ${MPIEXEC_POSTFLAGS})
  add_test(fdemo-pn ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/fdemo-pn ${MPIEXEC_POSTFLAGS})

  add_test(f90demo ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/f90demo ${MPIEXEC_POSTFLAGS})
  add_test(f90demo-s1 ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/f90demo-s1
    ${MPIEXEC_POSTFLAGS})
  add_test(f90demo-pn ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS} ${EXECUTABLE_OUTPUT_PATH}/f90demo-pn
    ${MPIEXEC_POSTFLAGS})


  foreach (TARGET fdemo fdemo-s1 fdemo-pn f90demo f90demo-s1 f90demo-pn)
    set_sanitizer_env(${TARGET})
  endforeach ()
  set_pnmpi_env(fdemo-pn)
  set_pnmpi_env(f90demo-pn)
endif()

include_directories(
  ${MPI_C_INCLUDE_PATH}
  ${MPI_Fortran_INCLUDE_PATH})

#
# TODO: do something with .pnmpi-conf-democonf
#
