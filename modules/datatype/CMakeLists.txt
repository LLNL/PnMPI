# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

add_pnmpi_module(datatype datatype.cpp)
add_pnmpi_module(dtprint dtprint.c)

#Add header to intermediate include dir
install(FILES datatype.h DESTINATION include)
install(TARGETS datatype dtprint DESTINATION lib/pnmpi-modules)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}/src
  ${MPI_C_INCLUDE_PATH})
