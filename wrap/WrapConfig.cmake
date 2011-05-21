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
# @author Todd Gamblin
# @date 19 May 2011


# add_wrapped_file(file_name wrapper_name)
# 
# This macro adds a command to generate <file_name> from <wrapper_name> to the
# build.  Properties on <file_name> are also set so that CMake knows that it
# is generated.
#
function(add_wrapped_file file_name wrapper_name)
  set(file_path    ${CMAKE_CURRENT_BINARY_DIR}/${file_name})
  set(wrapper_path ${CMAKE_CURRENT_SOURCE_DIR}/${wrapper_name})
  add_custom_command(
    OUTPUT  ${file_path}
    COMMAND ${CMAKE_SOURCE_DIR}/wrap/wrap.py ARGS ${wrapper_path} -o ${file_path}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS ${wrapper_path}
    COMMENT "Generating ${file_name} from ${wrapper_name}"
    VERBATIM)
  set_source_files_properties(${file_path} PROPERTIES GENERATED TRUE)
endfunction()

