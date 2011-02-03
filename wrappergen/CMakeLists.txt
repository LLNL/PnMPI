# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# Based upon a Makefile with the following copyright statment:
## Generated automatically from Makefile.in by configure.
## (C) 1993 by Argonne National Laboratory and Mississipi State University.
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

set(CMAKE_C_COMPILER gcc)
add_definitions("-DPROTO_FILE=\"../wrapper/mpi_proto_def\"")
add_executable(${TARGET_WRAPPERGEN} readproto.c wrappergen.c args.c)

 
