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

#include <pnmpi/service.h>


/** \brief Look up the error message for error \p err.
 *
 * \details As the libc function strerror, this function returns the
 *  corresponding error message for error code \p err.
 *
 * \note The returned pointer MUST NOT be freed.
 *
 *
 * \param err The error code to convert.
 *
 * \return Pointer to the error message string.
 *
 *
 * \ingroup PNMPI_Service_strerror
 */
const char *PNMPI_Service_strerror(PNMPI_status_t err)
{
  switch (err)
    {
    case PNMPI_SUCCESS: return "Success";
    case PNMPI_FAILURE: return "Failure";
    case PNMPI_NOMEM: return "Can't allocate memory";
    case PNMPI_NOMODULE: return "Module not found";
    case PNMPI_NOSERVICE: return "Service not found";
    case PNMPI_NOGLOBAL: return "Global not found";
    case PNMPI_SIGNATURE: return "Signatures don't match";
    case PNMPI_NOARG: return "Argument not found";
    case PNMPI_NOSTACK: return "Stack not found";
    case PNMPI_NOT_IMPLEMENTED: return "Service not implemented";

    default: return "Unknown";
    }
}
