# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

#modules to build:
set(MODS sample1 sample2 sample3 sample4 sample1f)

#loop over modules and build them
foreach(mod ${MODS})
    #Use macro to: add target for module, version it, install it, patch it
    add_pnmpi_module(${mod} ${mod}.c)
    install(TARGETS ${mod} DESTINATION lib/pnmpi-modules)
endforeach()

# Create special non-patched version of sample1 (as SHARED and not as MODULE)
# Used as part of the demo to show simple preloading of PMPI libraries.
add_library(sample1-non-patched SHARED sample1.c)
target_link_libraries(sample1-non-patched ${MPI_C_LIBRARIES})

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}/src
  ${MPI_C_INCLUDE_PATH})
