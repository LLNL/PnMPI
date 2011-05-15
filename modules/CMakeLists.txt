# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

# Add include directories for the dependent modules -- this applies to all subdirectories
include_directories(BEFORE datatype requests status ${INCLUDE_OUTPUT_PATH} ${MPI_C_INCLUDE_PATH})

#Recurse to sub-directories
add_subdirectory(comm)
#add_subdirectory(datatype)
#add_subdirectory(empty)
#add_subdirectory(requests)
#add_subdirectory(sample)
#add_subdirectory(status)
#add_subdirectory(switch-matrix)
#add_subdirectory(timelapse)
#add_subdirectory(virtual)
