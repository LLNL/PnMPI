# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

#Use macro to: add target for module, version it, install it, patch it
add_pnmpi_module(switch-matrix switch-matrix.c)
add_wrapped_file(switch-matrix.c switch-matrix.w)

install(TARGETS switch-matrix DESTINATION lib/pnmpi-modules)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}/src
  ${MPI_C_INCLUDE_PATH})
