/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2019 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2019 RWTH Aachen University, Federal Republic of Germany
 *
 *
 * P^nMPI is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation version 2.1 dated February 1999.
 *
 * P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with P^nMPI; if not, write to the
 *
 *   Free Software Foundation, Inc.
 *   51 Franklin St, Fifth Floor
 *   Boston, MA 02110, USA
 *
 *
 * Written by Martin Schulz, schulzm@llnl.gov.
 *
 * LLNL-CODE-402774
 */

#ifndef PNMPI_PRIVATE_FUNCTION_ADDRESS_H
#define PNMPI_PRIVATE_FUNCTION_ADDRESS_H


#include <stddef.h>

#include <pnmpi/private/attributes.h>
#include <pnmpi/private/tls.h>


extern pnmpi_compiler_tls_keyword void *pnmpi_function_address;


/** \brief Store the address of the application's MPI function call.
 *
 * \note If the function address has already been set, a new value will NOT be
 *  stored. This is required for the Fortran wrapper redirections, as these call
 *  the C wrappers internal. Remember to reset the function address before
 *  exiting the wrapper.
 *
 *
 * \private
 */
PNMPI_UNUSED
static void pnmpi_set_function_address(void *addr)
{
  if (pnmpi_function_address == NULL)
    pnmpi_function_address = (char *)addr;
}



/** \brief Get the function address.
 *
 *
 * \return The address of the original function call currently wrapped.
 *
 *
 * \private
 */
PNMPI_UNUSED
static void *pnmpi_get_function_address(void)
{
  return pnmpi_function_address;
}


/** \brief Reset the function address.
 *
 *
 * \private
 */
PNMPI_UNUSED
static void pnmpi_reset_function_address(void)
{
  pnmpi_function_address = NULL;
}


#endif
