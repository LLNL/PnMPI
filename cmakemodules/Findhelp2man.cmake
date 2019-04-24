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

include(FindPackageHandleStandardArgs)
include(GNUInstallDirs)


# Search for help2man binary.
find_program (HELP2MAN_BIN NAMES help2man)
find_package_handle_standard_args(help2man
  FOUND_VAR HELP2MAN_FOUND REQUIRED_VARS HELP2MAN_BIN)

mark_as_advanced(HELP2MAN_BIN)

if (NOT HELP2MAN_BIN)
  return()
endif ()


# Helper function to generate a man page for a binary.
function (help2man TARGET)
  list(REMOVE_AT ARGV 0)

  set(options NOINFO INSTALL)
  set(oneValueArgs SECTION TARGET RENAME)
  cmake_parse_arguments("OPT" "${options}" "${oneValueArgs}" "" ${ARGN})

  if (NOT OPT_TARGET AND OPT_UNPARSED_ARGUMENTS)
    set(OPT_TARGET "${OPT_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT OPT_SECTION)
    set(OPT_SECTION 1)
  endif ()

  if (NOT OPT_TARGET)
    message(FATAL_ERROR "missing required option: TARGET")
  endif ()


  # Generate man page
  set(HELP2MAN_ARGS "")
  if (OPT_NOINFO)
    list(APPEND HELP2MAN_ARGS "-N")
  endif ()

  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
    DEPENDS ${OPT_TARGET}
    COMMAND ${HELP2MAN_BIN} --output=${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
      ${HELP2MAN_ARGS} $<TARGET_FILE:${OPT_TARGET}>)

  add_custom_target(${TARGET} ALL
    DEPENDS ${OPT_TARGET} ${CMAKE_CURRENT_BINARY_DIR}/${TARGET})

  if (OPT_INSTALL)
    set(install_opt "")
    if (OPT_RENAME)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
        DESTINATION ${CMAKE_INSTALL_MANDIR}/man${OPT_SECTION}
        RENAME ${OPT_RENAME})
    else ()
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
        DESTINATION ${CMAKE_INSTALL_MANDIR}/man${OPT_SECTION})
    endif ()
  endif ()
endfunction ()
