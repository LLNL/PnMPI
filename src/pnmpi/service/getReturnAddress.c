/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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

#include <assert.h>

#include <pnmpi/private/return_address.h>
#include <pnmpi/service.h>


/** \brief Get the application's return address.
 *
 * \details When entering a wrapped MPI function, PnMPI will store the return
 *  address. Modules may use this address by calling this function to get the
 *  origin of the error.
 *
 *
 * \param [out] ptr Pointer where to store the return address.
 *
 * \return \ref PNMPI_SUCCESS The return address has been stored into \p ptr.
 * \return \ref PNMPI_NOT_IMPLEMENTED This service is not implemented.
 *
 *
 * \ingroup PNMPI_Service_GetReturnAddress
 */
PNMPI_status_t PNMPI_Service_GetReturnAddress(void **ptr)
{
  assert(ptr);


/* If the compiler does support accessing the return address, it has been saved
 * by accessing the PnMPI wrappers before. Copy the saved return address into
 * the pointer supported by the callee and return success.
 *
 * However, if the compiler doesn't, return an error status code to tell the
 * callee this feature isn't implemented. */
#ifdef HAVE_BUILTIN_RETURN_ADDRESS
  *ptr = pnmpi_get_return_address();
  return PNMPI_SUCCESS;
#else
  return PNMPI_NOT_IMPLEMENTED;
#endif
}
