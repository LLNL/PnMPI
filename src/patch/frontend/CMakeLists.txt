# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

# TODO Martin please add the copyright statment of your choice, as well as
#      a reference to the license or license file!
#
# @authors Todd Gamblin, Tobias Hilbrich, Martin Schulz
# @date 05-22-2011


#Add the target for the patcher
if (PATCHER_FRONTEND_COMPILER)
   set(CMAKE_C_COMPILER ${PATCHER_FRONTEND_COMPILER})
   set(CMAKE_C_FLAGS "")
   set(CMAKE_C_FLAGS_DEBUG "")
   set(CMAKE_BUILD_TYPE "Debug")
   set(BFD_FOUND False)
   set(DPNMPI_HAVE_BFD False)

   add_executable(pnmpi-patch ../patch.c)
   add_coverage(pnmpi-patch)
   add_sanitizers(pnmpi-patch)
   install(TARGETS pnmpi-patch EXPORT pnmpi-patch RUNTIME DESTINATION bin)

   # Allow patch target to be used by subprojects.
   install(EXPORT pnmpi-patch DESTINATION share/cmake/PnMPI)
endif()

include_directories(${CMAKE_BINARY_DIR})
