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

include(CheckCSourceCompiles)


# Check if the compiler supports getting the return address, required by
# pnmpi_return_address_set().
check_c_source_compiles("
  int main ()
  {
    void *p = __builtin_return_address(0);
    return 0;
  }
  " HAVE_BUILTIN_RETURN_ADDRESS)


# Install and manage the public and private header files of the PnMPI sources.
# This has to be done to use the same headers for the PnMPI sources and the API
# without getting conflicts about the used paths. Otherwise you'd have to take
# care about file locations after installation, especially if you use the
# headers over multiple directories.
pnmpi_add_private_header(function_address.h)
pnmpi_add_private_header(mpi_interface.h)
pnmpi_add_private_header(mpi_reentry.h)
pnmpi_add_private_header(return_address.h.in return_address.h)

easy_add_library(pnmpi_wrapper OBJECT
  mpi_interface.c
  mpi_reentry.c
  return_address.c
  function_address.c)

add_coverage(pnmpi_wrapper_pic)
add_sanitizers(pnmpi_wrapper_pic)
