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


# Search for LLNL hires-timers library and header.
find_path(HIRES_TIMERS_INCLUDE_DIR hires-timers.h)
find_library(HIRES_TIMERS_LIBRARY NAMES hires-timers)
mark_as_advanced(HIRES_TIMERS_INCLUDE_DIR HIRES_TIMERS_LIBRARY)


# Handle the QUIET and REQUIRED arguments and set HIRES_TIMERS_FOUND to TRUE if
# all listed variables are defined.
find_package_handle_standard_args(
	HIRES_TIMERS
	FOUND_VAR HIRES_TIMERS_FOUND
	REQUIRED_VARS HIRES_TIMERS_LIBRARY HIRES_TIMERS_INCLUDE_DIR)

# If all required variables have been defined, set the general variables for
# using LLNL hires timers.
set(HIRES_TIMERS_INCLUDE_DIRS ${HIRES_TIMERS_INCLUDE_DIR})
set(HIRES_TIMERS_LIBRARIES ${HIRES_TIMERS_LIBRARY})
