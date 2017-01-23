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
 *
 *
 * \ingroup pnmpi_service
 */
typedef int PNMPI_modHandle_t;

/** \addtogroup pnmpi_status
 * \{
 */
/** \brief PnMPI error codes.
 */
typedef enum pnmpi_status {
  PNMPI_SUCCESS = 0,    ///< Everything ok.
  PNMPI_NOT_INIT = -1,  ///< Unused.
  PNMPI_NOMEM = -2,     ///< Not enough memory to perform action.
  PNMPI_NOMODULE = -3,  ///< Requested module not found.
  PNMPI_NOSERVICE = -4, ///< Requested service not found.
  PNMPI_NOGLOBAL = -5,  ///< Requested global not found.
  PNMPI_SIGNATURE = -6, /**< Requested service or global found, but none has a
                             matching signature. */
  PNMPI_NOARG = -7      ///< Requested argument not found.
} pnmpi_status;
/// \}


/* The PnMPI API should be C++ compatible, too. We have to add the extern "C"
 * stanza to avoid name mangeling. Otherwise PnMPI modules would not find PnMPI
 * API functions. */
#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup pnmpi_service Service functions for module interaction.
 */
pnmpi_status PNMPI_Service_GetModuleByName(const char *name,
                                           PNMPI_modHandle_t *handle);
pnmpi_status PNMPI_Service_GetPcontrol(PNMPI_modHandle_t handle, int *flag);


/** \defgroup pnmpi_service_self Service functions for module interaction.
 *
 * \note These functions are the same as the \ref pnmpi_service interface, but
 *  allow the modules to interact with PnMPI for the own module in one call
 *  instead of getting the handle first.
 */
int PNMPI_Service_GetPcontrolSelf();
pnmpi_status PNMPI_Service_GetModuleSelf(PNMPI_modHandle_t *handle);


/** \defgroup pnmpi_service_register Service functions for module registration.
 */
pnmpi_status PNMPI_Service_RegisterModule(const char *name);


#ifdef __cplusplus
}
#endif


#endif
