# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011

find_package(Sanitizers)

#Add the target for the patcher
SET (PATCHER_FRONTEND_COMPILER "" CACHE FILEPATH "If this is a frontend/backend build use the backend compiler as CMake compilers and specify a frontend compiler here to create a frontend-operating version of the PnMPI patcher.")

SET (targetName pnmpi-patch)
if (PATCHER_FRONTEND_COMPILER)
   SET (targetName backend-pnmpi-patch)
endif()

add_executable(${targetName} patch.c)
add_coverage(${targetName})
add_sanitizers(${targetName})
if (BFD_FOUND)
  target_link_libraries(${targetName} ${BFD_LIBRARIES})
  target_link_libraries(${targetName} ${IBERTY_LIBRARIES})
endif()
install(TARGETS ${targetName} EXPORT ${targetName} RUNTIME DESTINATION bin)

# Allow patch target to be used by subprojects.
install(EXPORT ${targetName} DESTINATION share/cmake/PnMPI)

include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(frontend)
