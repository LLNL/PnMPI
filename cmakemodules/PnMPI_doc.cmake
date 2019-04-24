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

include(GNUInstallDirs)


## \brief Add a man page for Doxygen \p MODULE with \p SYMBOL as symlink.
#
# \note This function has no effect if BUILD_DOC and BUILD_DOC_INTERNAL are
#  disabled.
#
#
# \param MODULE The Doxygen module name.
# \param SYMBOL The name of a symbol in the module.
#
function (pnmpi_add_man MODULE SYMBOL)
  if (NOT BUILD_DOC)
    return()
  endif ()


  # Add a new target dependent on the doc target for the required man page.
  if (NOT TARGET ${MODULE}.3)
    set(MANFILE "${PROJECT_BINARY_DIR}/doc/man/man3/${MODULE}.3")
    add_custom_command(OUTPUT ${MANFILE} DEPENDS doc)
    add_custom_target(${MODULE}.3 ALL DEPENDS ${MANFILE})
  endif ()

  # Install the man page and a link for SYMBOL.
  install(FILES ${MANFILE}
          DESTINATION ${CMAKE_INSTALL_MANDIR}/man3)
  if(CMAKE_HOST_UNIX)
    if (SYMBOL)
      set(INSTALL_FILE ${CMAKE_INSTALL_FULL_MANDIR}/man3/${SYMBOL}.3)
      set(LINK_TARGET ${MODULE}.3)
      install(CODE "
        set(install_link true)
        if (EXISTS ${INSTALL_FILE})
          execute_process(COMMAND readlink ${INSTALL_FILE}
                          OUTPUT_VARIABLE link_value
                          OUTPUT_STRIP_TRAILING_WHITESPACE)
          if (\${link_value} STREQUAL ${LINK_TARGET})
            set(install_link false)
          endif ()
        endif ()

        if (install_link)
          execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${LINK_TARGET} ${INSTALL_FILE})
          message(STATUS \"Installing: ${INSTALL_FILE}\")
        else ()
          message(STATUS \"Up-to-date: ${INSTALL_FILE}\")
        endif()
      ")
    endif ()
  else()
    message(WARNING "Can't install symlinks for man pages.")
  endif()
endfunction ()


## \brief Install a module man page.
#
# \note This function has no effect if BUILD_DOC and BUILD_DOC_INTERNAL are
#  disabled.
#
#
# \param MODNAME Name of the module. Will be appended to `pnmpi-mod-`
# \param CLASS The class name used in Doxygen.
#
function (pnmpi_add_module_man MODNAME CLASS)
  if (NOT BUILD_DOC)
    return()
  endif ()

  # Add a new target dependent on the doc target for the required class and
  # install it.
  set(TARGET_FILE "pnmpi-module-${MODNAME}.1")
  if (NOT TARGET ${MODULE}.3)
    set(MANFILE "${PROJECT_BINARY_DIR}/doc/man/man3/${CLASS}.3")
    add_custom_command(OUTPUT ${MANFILE} DEPENDS doc)
    add_custom_target(${TARGET_FILE} ALL DEPENDS ${MANFILE})
  endif ()
  install(FILES "${PROJECT_BINARY_DIR}/doc/man/man3/${CLASS}.3"
          DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
          RENAME ${TARGET_FILE})
endfunction ()
