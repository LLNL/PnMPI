#===================================
#Check for adept_utils timers
#===================================

find_package(adept_utils QUIET)
if(adept_utils_FOUND)
    message(STATUS "Found adept_utils in ${adept_utils_DIR}.")
    add_definitions(-DHAVE_ADEPT_UTILS)
    set(PnMPI_USE_adept_utils TRUE)
else()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/adept-utils/CMakeLists.txt)
        ADD_SUBDIRECTORY (adept-utils)
        message(STATUS "Found adept_utils as submodule.")
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
        message(STATUS "adept_utils package not found.  Building without timing.")
        # Set these for our PnMPIConfig.cmake file.
        set(adept_utils_INCLUDE_PATH adept_utils_INCLUDE_PATH-NOTFOUND)
        set(adept_utils_LIBRARIES    adept_utils_LIBRARIES-NOTFOUND)
    endif()
endif()

#======================================================
# Set up wrapper generator so this project can use it.
#======================================================
find_package(wrap QUIET)
if(wrap_FOUND)
    message(STATUS "Found warp in ${wrap_DIR}.")
else()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/wrap/CMakeLists.txt)
        ADD_SUBDIRECTORY (wrap)

        set(WRAP ${PROJECT_SOURCE_DIR}/externals/wrap/wrap.py)
	set(GETSTATUSSIZE ${PROJECT_SOURCE_DIR}/externals/wrap/getStatusSize.f)
        configure_file(
        #  ${PROJECT_SOURCE_DIR}/wrap/WrapConfig.cmake.in
        #  ${PROJECT_BINARY_DIR}/wrap/WrapBuildConfig.cmake @ONLY)
        #include(${PROJECT_BINARY_DIR}/wrap/WrapBuildConfig.cmake)
        ${PROJECT_SOURCE_DIR}/externals/wrap/wrap-config.cmake.in
        ${PROJECT_BINARY_DIR}/wrap-config.cmake @ONLY)
        include(${PROJECT_BINARY_DIR}/wrap-config.cmake)
    else()
        message( FATAL_ERROR "Subproject wrap not available, CMake will exit.\n Try:\n   git submodule update --init --recursive" )
    endif()
endif()
