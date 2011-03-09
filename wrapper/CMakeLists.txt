# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Name definitions
set(MPISPEC "mpi_pnmpi")

set(PROTOFILE "${MPISPEC}_proto")
set(FCTFILE   "${MPISPEC}_fct")

set(WRAPPERC "wrapper_c.w")
set(WRAPPERH "wrapper_h.w")
set(NEWSTACK "newstack_h.w")

#Target for "extrargs" tool from P^nMPI
add_executable(${TARGET_EXTRARGS} extrargs.c)
add_dependencies(${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})

#Set sources as generated
##TODO neccessary still ?? needs to be moved after the target ???
set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/wrapper.c 
    ${CMAKE_CURRENT_BINARY_DIR}/wrapper.h 
    ${CMAKE_CURRENT_BINARY_DIR}/newstack.h 
    ${CMAKE_CURRENT_BINARY_DIR}/wrapper_f77.c 
    ${CMAKE_CURRENT_BINARY_DIR}/f77symbols.h 
    PROPERTIES GENERATED 1)    

#Invoke prepration scripts
configure_file(
    "${CMAKE_SOURCE_DIR}/wrapper/generate_protp_file_cmake.sh.in" 
    "${CMAKE_CURRENT_BINARY_DIR}/generate_protp_file_cmake.sh")

# Make -I includes from MPI include directories
string(REGEX REPLACE "([^ ]+)" "-I\\1" MPI_INCLUDES "${MPI_C_INCLUDE_PATH}")

add_custom_command(
    TARGET ${TARGET_EXTRARGS} POST_BUILD
    COMMAND eval ARGS "${CMAKE_C_COMPILER} ${MPI_INCLUDES} -E ${CMAKE_SOURCE_DIR}/wrapper/dummy.c | sed 's/__attribute__[(][(]visibility[(]\"default\"[)][)][)][[:space:]]//' | ${AWK} -f ${CMAKE_SOURCE_DIR}/wrapper/join_lines | grep -e \"PMPI_\" | grep -v \"_FN\" | grep -v f2c | grep -v c2f | sed 's/(/ (/' | grep -v PMPI_Pcontrol | grep -v PMPI_Type_count > dummy.fct"
    COMMAND eval ARGS "python ${CMAKE_SOURCE_DIR}/wrapper/makefortran.py --xlate --f77symbol ${F77SYMBOL} ${CMAKE_SOURCE_DIR}/wrapper/mpi-f77.txt"
    COMMAND eval ARGS "cat dummy.fct | ${AWK} -f ${CMAKE_SOURCE_DIR}/wrapper/get_fct | sed 's/PMPI/MPI/' | sort | uniq > ${MPISPEC}_fct-nopctrl"
    COMMAND eval ARGS "${AWK} 'BEGIN{print \"MPI_Pcontrol\"}{print}'< ${MPISPEC}_fct-nopctrl > ${MPISPEC}_fct"
    COMMAND eval ARGS "tcsh generate_protp_file_cmake.sh ${MPISPEC}_fct-nopctrl dummy.fct | ${AWK} -f ${CMAKE_SOURCE_DIR}/wrapper/add-pctrl> ${MPISPEC}_proto" 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Invoking preparation for wrapper generation" 
    VERBATIM)

#Invoke generation of wrappers as pre build event of wrapper lib target
add_custom_command(
    TARGET ${TARGET_EXTRARGS} POST_BUILD 
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_SOURCE_DIR}/wrapper/${WRAPPERC}" "-o" "wrapper.c"
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_SOURCE_DIR}/wrapper/${WRAPPERH}" "-o" "wrapper.h"
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_SOURCE_DIR}/wrapper/${NEWSTACK}" "-o" "newstack.h"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating wrapper sources" 
    VERBATIM)

#copy header files to include directory in binary dir
add_custom_command(TARGET ${TARGET_EXTRARGS} POST_BUILD 
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/newstack.h" "${INCLUDE_OUTPUT_PATH}/newstack.h"
    COMMENT "Copying generated header to common header directory"
    VERBATIM)

