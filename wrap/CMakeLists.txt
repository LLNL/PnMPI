# Install wrap.py into the project binary directory
install(PROGRAMS wrap.py DESTINATION bin)

# Install WrapConfig into the project CMake config directory so that
# wrap.py can be used by subprojects.
set(WRAP ${CMAKE_INSTALL_PREFIX}/bin/wrap.py)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/WrapConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/WrapConfig.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/WrapConfig.cmake DESTINATION lib/cmake/PnMPI)
