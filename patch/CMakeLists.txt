# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011


#Add the target for the patcher
set(CMAKE_C_COMPILER gcc)

add_executable(pnmpi-patch patch.c)
if (BFD_FOUND)
  target_link_libraries(pnmpi-patch ${BFD_LIBRARIES})
  target_link_libraries(pnmpi-patch ${IBERTY_LIBRARIES})
endif()
install(TARGETS pnmpi-patch EXPORT pnmpi-patch RUNTIME DESTINATION bin)

# Allow patch target to be used by subprojects.
install(EXPORT pnmpi-patch DESTINATION share/cmake/PnMPI)

include_directories(${CMAKE_BINARY_DIR})
