# - BFD Library module.
#=============================================================================
# This module finds libbfd and associated headers.
#
#=== Variables ===============================================================
# This module will set the following variables in your project:
#
#   BFD_INCLUDE_PATH     Location of bfd.h and libiberty.h
#   BFD_LIBRARIES        Location of libbfd.so or libbfd.a
#
#   BFD_FOUND            Whether libbfd was successfully found.
#
#=============================================================================
include(FindPackageHandleStandardArgs)

find_path(BFD_INCLUDE_PATH bfd.h)
find_library(BFD_LIBRARIES bfd)

find_path(IBERTY_INCLUDE_PATH libiberty.h)
find_library(IBERTY_LIBRARIES iberty)

function(append_unique l elt)
  list(FIND ${l} ${elt} INDEX)
  if (INDEX GREATER -1)
    list(APPEND ${l} ${elt})
  endif()
endfunction()

append_unique(${BFD_INCLUDE_PATH} ${IBERTY_INCLUDE_PATH})
append_unique(${BFD_LIBRARIES} ${IBERTY_LIBRARIES})

find_package_handle_standard_args(BFD
  DEFAULT_MSG
  BFD_LIBRARIES BFD_INCLUDE_PATH IBERTY_LIBRARIES IBERTY_INCLUDE_PATH)
