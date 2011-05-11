# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Set names for targets
set(MODS "datatype" "dtprint")

#Loop over all targets (submods)
foreach(mod ${MODS})
    #Use macro to: add target for module, version it, install it, patch it
    pnmpi_mac_add_module(${mod} "${mod}" "CXX")
endforeach()

#Add header to intermediate include dir
install(FILES datatype.h DESTINATION include)
