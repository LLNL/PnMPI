# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Name definitions
SET (MPISPEC "mpi_pnmpi")

SET (PROTOFILE "${MPISPEC}_proto")
SET (FCTFILE   "${MPISPEC}_fct")

SET (WRAPPERC "wrapper_c.w")
SET (WRAPPERH "wrapper_h.w")
SET (NEWSTACK "newstack_h.w")

#Target for "extrargs" tool from P^nMPI
ADD_EXECUTABLE(${TARGET_EXTRARGS} extrargs.c)
ADD_DEPENDENCIES(${TARGET_EXTRARGS} ${TARGET_WRAPPERGEN})

#Set sources as generated
##TODO neccessary still ?? needs to be moved after the target ???
SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_BINARY_DIR}/wrapper.c 
    ${CMAKE_CURRENT_BINARY_DIR}/wrapper.h 
    ${CMAKE_CURRENT_BINARY_DIR}/newstack.h 
    ${CMAKE_CURRENT_BINARY_DIR}/wrapper_f77.c 
    ${CMAKE_CURRENT_BINARY_DIR}/f77symbols.h 
    PROPERTIES GENERATED 1)    

#Invoke prepration scripts
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/wrapper/generate_protp_file_cmake.sh.in" 
    "${CMAKE_CURRENT_BINARY_DIR}/generate_protp_file_cmake.sh"
    )
ADD_CUSTOM_COMMAND(
    TARGET ${TARGET_EXTRARGS} POST_BUILD 
    COMMAND eval ARGS "${CMAKE_C_COMPILER} -I${MPI_H_DIR} -E ${CMAKE_SOURCE_DIR}/wrapper/dummy.c | sed 's/__attribute__[(][(]visibility[(]\"default\"[)][)][)][[:space:]]//' | gawk -f ${CMAKE_SOURCE_DIR}/wrapper/join_lines | grep -e \"PMPI_\" | grep -v \"_FN\" | grep -v f2c | grep -v c2f | sed 's/(/ (/' | grep -v PMPI_Pcontrol | grep -v PMPI_Type_count > dummy.fct"
    COMMAND eval ARGS "python ${CMAKE_SOURCE_DIR}/wrapper/makefortran.py --xlate --f77symbol ${F77SYMBOL} ${CMAKE_SOURCE_DIR}/wrapper/mpi-f77.txt"
    COMMAND eval ARGS "cat dummy.fct | gawk -f ${CMAKE_SOURCE_DIR}/wrapper/get_fct | sed 's/PMPI/MPI/' | sort | uniq > mpi_pnmpi_fct-nopctrl"
    COMMAND eval ARGS "gawk 'BEGIN{print \"MPI_Pcontrol\"}{print}'< mpi_pnmpi_fct-nopctrl > mpi_pnmpi_fct"
    COMMAND eval ARGS "tcsh generate_protp_file_cmake.sh mpi_pnmpi_fct-nopctrl dummy.fct | gawk -f ${CMAKE_SOURCE_DIR}/wrapper/add-pctrl> mpi_pnmpi_proto" 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Invoking preparation for wrapper generation" 
    VERBATIM)

#Invoke generation of wrappers as pre build event of wrapper lib target
ADD_CUSTOM_COMMAND(
    TARGET ${TARGET_EXTRARGS} POST_BUILD 
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_SOURCE_DIR}/wrapper/${WRAPPERC}" "-o" "wrapper.c"
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_SOURCE_DIR}/wrapper/${WRAPPERH}" "-o" "wrapper.h"
    COMMAND ${TARGET_WRAPPERGEN} ARGS "-p" "${PROTOFILE}" "-f" "${FCTFILE}" "-w" "${CMAKE_SOURCE_DIR}/wrapper/${NEWSTACK}" "-o" "newstack.h"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating wrapper sources" 
    VERBATIM)

#copy header files to include directory in binary dir
ADD_CUSTOM_COMMAND(TARGET ${TARGET_EXTRARGS} POST_BUILD 
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/newstack.h" "${INCLUDE_OUTPUT_PATH}/newstack.h"
    COMMENT "Copying generated header to common header directory"
    VERBATIM
    )

