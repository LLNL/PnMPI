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

#ifndef PNMPI_SERVICE_H
#define PNMPI_SERVICE_H


#include <stdint.h>

#include <pnmpi/attributes.h>


/// \brief Max length of service name (including terminating null-character).
#define PNMPI_SERVICE_NAMELEN 30

/** \brief Max length of service signature (including terminating
 *   null-character).
 */
#define PNMPI_SERVICE_SIGLEN 20



/// \brief Handle for a module.
typedef int PNMPI_modHandle_t;

/** \addtogroup PNMPI_status_t
 * \header{pnmpi/service.h}
 * \{
 */
/// \brief PnMPI error codes.
typedef enum pnmpi_status {
  PNMPI_SUCCESS = 0,    ///< Everything ok.
  PNMPI_FAILURE = -1,   ///< An error occurred.
  PNMPI_NOMEM = -2,     ///< Not enough memory to perform action.
  PNMPI_NOMODULE = -3,  ///< Requested module not found.
  PNMPI_NOSERVICE = -4, ///< Requested service not found.
  PNMPI_NOGLOBAL = -5,  ///< Requested global not found.
  PNMPI_SIGNATURE = -6, /**< Signature is invalid or no global or service with a
                             matching signature can be found. */
  PNMPI_NOARG = -7,     ///< Requested argument not found.
  PNMPI_NOSTACK = -8,   ///< Requested stack not found.
  PNMPI_NOT_IMPLEMENTED = INT8_MIN ///< Service not implemented.
} PNMPI_status_t;
/// \}

/** \addtogroup PNMPI_Service_GetGlobalByName
 * \{
 */
/// \brief Struct to describe a module global.
typedef struct PNMPI_Global_descriptor_d
{
  char name[PNMPI_SERVICE_NAMELEN]; ///< Name of the global.
  char sig;
  /**< \brief Signature of the global.
   *
   * \note The signature has no specific format, but keep the users knowing how
   *  to search for your global.
   */
  union
  {
    char *c;
    short *s;
    int *i;
    unsigned int *u;
    long *l;
    double *d;
    float *f;
    void **p;
  } addr; ///< Pointer to the data.
} PNMPI_Global_descriptor_t;
/// \}

/** \addtogroup PNMPI_Service_GetServiceByName
 * \{
 */
/// \brief Type for a service function.
typedef int (*PNMPI_Service_Fct_t)();

/// \brief Struct to describe a module service.
typedef struct PNMPI_Service_descriptor_d
{
  char name[PNMPI_SERVICE_NAMELEN]; ///< Name of the service.
  char sig[PNMPI_SERVICE_SIGLEN];
  /**< \brief Signature of the service.
   *
   * \note The signature has no specific format, but keep the users knowing how
   *  to search for your service.
   */
  PNMPI_Service_Fct_t fct; ///< Pointer to the service function.
} PNMPI_Service_descriptor_t;
/// \}

/** \addtogroup PNMPI_Service_CallHook
 * \{
 */
#define PNMPI_HOOK_SAME __FUNCTION__
/// \}


/* The PnMPI API should be C++ compatible, too. We have to add the extern "C"
 * stanza to avoid name mangling. Otherwise PnMPI modules would not find PnMPI
 * API functions. */
#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup PNMPI_Service_GetModuleByName PNMPI_Service_GetModuleByName
 *
 * \details These functions may be used for getting module handles. Each module
 *  may receive its own module handle or lookup handles for other modules by a
 *  unique module name. Each module is able to manipulate its module name.
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(1)
PNMPI_status_t PNMPI_Service_RegisterModule(const char *name);
PNMPI_FUNCTION_ARG_NONNULL(1, 2)
PNMPI_status_t PNMPI_Service_GetModuleByName(const char *name,
                                             PNMPI_modHandle_t *handle);
PNMPI_status_t PNMPI_Service_GetModuleSelf(PNMPI_modHandle_t *handle);

/** \defgroup PNMPI_Service_GetStackByName PNMPI_Service_GetStackByName
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(1, 2)
PNMPI_status_t PNMPI_Service_GetStackByName(const char *name,
                                            PNMPI_modHandle_t *handle);
PNMPI_status_t PNMPI_Service_ChangeStack(PNMPI_modHandle_t stack);


/** \defgroup PNMPI_Service_GetGlobalByName PNMPI_Service_GetGlobalByName
 *
 * \details Modules may provide storage that is accessible by other modules.
 *  These functions will be used to register and access this storage.
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(1)
PNMPI_status_t
PNMPI_Service_RegisterGlobal(const PNMPI_Global_descriptor_t *global);
PNMPI_FUNCTION_ARG_NONNULL(2, 4)
PNMPI_status_t PNMPI_Service_GetGlobalByName(PNMPI_modHandle_t handle,
                                             const char *name, const char sig,
                                             PNMPI_Global_descriptor_t *dest);

/** \defgroup PNMPI_Service_GetServiceByName PNMPI_Service_GetServiceByName
 *
 * \details Modules may provide service functions that are accessible by other
 *  modules. These functions will be used to register and access these
 *  functions.
 */
PNMPI_FUNCTION_ARG_NONNULL(1)
PNMPI_status_t
PNMPI_Service_RegisterService(const PNMPI_Service_descriptor_t *service);
PNMPI_FUNCTION_ARG_NONNULL(2, 4)
PNMPI_status_t PNMPI_Service_GetServiceByName(PNMPI_modHandle_t handle,
                                              const char *name, const char *sig,
                                              PNMPI_Service_descriptor_t *dest);

/** \defgroup PNMPI_Service_GetArgument PNMPI_Service_GetArgument
 *
 * \details These functions may be used to query the configuration for module
 *  arguments.
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(2, 3)
PNMPI_status_t PNMPI_Service_GetArgument(PNMPI_modHandle_t handle,
                                         const char *name, const char **dest);
PNMPI_FUNCTION_ARG_NONNULL(1)
const char *PNMPI_Service_GetArgumentSelf(const char *name);

/** \defgroup PNMPI_Service_GetPcontrol PNMPI_Service_GetPcontrol
 *
 * \details These functions may be used to query the modules Pcontrol settings.
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(2)
PNMPI_status_t PNMPI_Service_GetPcontrol(const PNMPI_modHandle_t handle,
                                         int *flag);
int PNMPI_Service_GetPcontrolSelf(void);

/** \defgroup PNMPI_Service_GetReturnAddress PNMPI_Service_GetReturnAddress
 *
 * \details These functions may be used to query debug information of the
 *  application's MPI call.
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(1)
PNMPI_status_t PNMPI_Service_GetReturnAddress(void **ptr);

PNMPI_FUNCTION_ARG_NONNULL(1)
PNMPI_status_t PNMPI_Service_GetFunctionAddress(void **ptr);

/** \defgroup PNMPI_Service_CallHook PNMPI_Service_CallHook
 *
 * \details These functions may be used to call hooks recursively in other
 *  modules.
 *
 * \header{pnmpi/service.h}
 */
PNMPI_FUNCTION_ARG_NONNULL(1)
void PNMPI_Service_CallHook(const char *hook);
PNMPI_FUNCTION_ARG_NONNULL(1)
void PNMPI_Service_CallHook_NewStack(const char *hook, PNMPI_modHandle_t stack);


/** \defgroup PNMPI_Service_strerror PNMPI_Service_strerror
 *
 * \header{pnmpi/service.h}
 */
const char *PNMPI_Service_strerror(PNMPI_status_t err);


#ifdef __cplusplus
}
#endif


#endif
