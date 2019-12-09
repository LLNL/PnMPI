# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2019 Lawrence Livermore National Laboratories, United States of America
#  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2019 RWTH Aachen University, Federal Republic of Germany
#
#
# P^nMPI is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation version 2.1 dated February 1999.
#
# P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with P^nMPI; if not, write to the
#
#   Free Software Foundation, Inc.
#   51 Franklin St, Fifth Floor
#   Boston, MA 02110, USA
#
#
# Written by Martin Schulz, schulzm@llnl.gov.
#
# LLNL-CODE-402774

include(FindPackageMessage)


#===================================
#Check for adept_utils
#===================================
option(ENABLE_ADEPT_UTILS "Enable adept-utils support." false)
if (ENABLE_ADEPT_UTILS)
  find_package(adept_utils QUIET)
  if(adept_utils_FOUND)
    find_package_message(adept_utils "Found adept_utils: ${adept_utils_DIR}"
                         "${adept_utils_DIR}")
    add_definitions(-DHAVE_ADEPT_UTILS)
    set(PnMPI_USE_adept_utils TRUE)
  else()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/adept-utils/CMakeLists.txt)
        ADD_SUBDIRECTORY (adept-utils)
        find_package_message(adept_utils "Found adept_utils as submodule."
                             "submodule")
        set(adept-utils_INCLUDE_DIRS
                    ${adept-utils_SOURCE_DIR}/utils
                    ${adept-utils_SOURCE_DIR}/cutils
                    ${adept-utils_SOURCE_DIR}/timing
                    ${adept-utils_BINARY_DIR}
                    ${adept-utils_INSTALL_DIR}/include
                    CACHE INTERNAL "adept-utils: Include Directories" FORCE)
        add_definitions(-DHAVE_ADEPT_UTILS)
        set(PnMPI_USE_adept_utils TRUE)
        set(adept_utils_DIR ${adept-utils_BINARY_DIR})
    else()
        find_package_message(adept_utils "adept_utils package not found."
                             "not found")
        # Set these for our PnMPIConfig.cmake file.
        set(adept_utils_INCLUDE_PATH adept_utils_INCLUDE_PATH-NOTFOUND)
        set(adept_utils_LIBRARIES    adept_utils_LIBRARIES-NOTFOUND)
    endif()
  endif()
endif()

#======================================================
# Set up wrapper generator so this project can use it.
#======================================================
find_package(wrap QUIET)
if(wrap_FOUND)
    find_package_message(wrap "Found wrap: ${wrap_DIR}" "${wrap_DIR}")
else()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/wrap/CMakeLists.txt)
        ADD_SUBDIRECTORY (wrap)
        find_package_message(wrap "Found wrap as submodule." "submodule")
        set(WRAP ${CMAKE_CURRENT_SOURCE_DIR}/wrap/wrap.py)
        set(GETSTATUSSIZE ${CMAKE_CURRENT_SOURCE_DIR}/wrap/getStatusSize.f)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wrap/wrap-config.cmake.in
                       ${CMAKE_CURRENT_BINARY_DIR}/wrap-config.cmake
                       @ONLY)
        include(${CMAKE_CURRENT_BINARY_DIR}/wrap-config.cmake)
    else()
        message(FATAL_ERROR "Subproject wrap not available, CMake will exit.\n"
                            "Try:\n"
                            "   git submodule update --init --recursive")
    endif()
endif()
