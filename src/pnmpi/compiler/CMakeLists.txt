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

# NOTE: Compiler features checked inside this directory should be general
#       purpose, i.e. reusable at several places inside of PnMPI's source code.
#       Compiler features related to a specific function should be tested in the
#       relating CMakeLists file instead!

# Check if thread local storage is available for thread safe variables in PnMPI
# and modules. First we try to use C11 _Thread_local, next the compiler-specific
# builtin __thread keyword.
if (NOT C11_THREAD_LOCAL_FOUND)
  find_package(ThreadKeyword)
endif ()

# PnMPI will be built thread safe by default. For performance improvements
# thread local storage may be disabled.
option(ENABLE_THREAD_SAFETY "Selects whether PnMPI is built thread safe." TRUE)
if (NOT ENABLE_THREAD_SAFETY)
  set(C11_THREAD_LOCAL_FOUND false)
  set(THREADKEYWORD_FOUND false)
endif ()


# Install and manage the public and private header files of the PnMPI sources.
# This has to be done to use the same headers for the PnMPI sources and the API
# without getting conflicts about the used paths. Otherwise you'd have to take
# care about file locations after installation, especially if you use the
# headers over multiple directories.
pnmpi_add_header(attr_public.h attributes.h)
pnmpi_add_private_header(attr_private.h attributes.h)
pnmpi_add_private_header(features.h.in compiler_features.h)
pnmpi_add_private_header(tls.h tls.h)
