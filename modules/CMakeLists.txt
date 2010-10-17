# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Add include directories for the dependent modules
INCLUDE_DIRECTORIES(BEFORE datatype requests status ${INCLUDE_OUTPUT_PATH})

#Recurse to sub-directories
ADD_SUBDIRECTORY(comm)
ADD_SUBDIRECTORY(datatype)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(requests)
ADD_SUBDIRECTORY(sample)
ADD_SUBDIRECTORY(status)
ADD_SUBDIRECTORY(switch-matrix)
ADD_SUBDIRECTORY(timelapse)
ADD_SUBDIRECTORY(virtual)