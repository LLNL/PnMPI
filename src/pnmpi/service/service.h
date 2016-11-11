/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

#ifndef PNMPI_SERVICE_H
#define PNMPI_SERVICE_H


/** \brief Handle for a module.
 */
typedef int pnmpi_module_handle;


/* The PnMPI API should be C++ compatible, too. We have to add the extern "C"
 * stanza to avoid name mangeling. Otherwise PnMPI modules would not find PnMPI
 * API functions. */
#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup pnmpi_service Service functions for module interaction.
 */
int PNMPI_Service_GetPcontrol(pnmpi_module_handle handle, int *flag);


/** \defgroup pnmpi_service_self Service functions for module interaction.
 *
 * \note These functions are the same as the \ref pnmpi_service interface, but
 *  allow the modules to interact with PnMPI for the own module in one call
 *  instead of getting the handle first.
 */
int PNMPI_Service_GetPcontrolSelf();


#ifdef __cplusplus
}
#endif


#endif
