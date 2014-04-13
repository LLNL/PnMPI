# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

add_pnmpi_module(empty empty.c)
add_wrapped_file(empty.c empty.w)

install(TARGETS empty DESTINATION lib/pnmpi-modules)

include_directories(${MPI_C_INCLUDE_PATH})
