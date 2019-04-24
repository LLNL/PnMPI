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

#include <assert.h>

#include "core.h"
#include <pnmpi/private/modules.h>
#include <pnmpi/service.h>


/** \brief Get argument \p name of module.
 *
 * \details Each module may be followed by one or more arguments in the
 *  configuration file. This function queries these arguments and sets \p dest
 *  to a pointer to the arguments value.
 *
 * \note Only the first match of \p name will be used.
 *
 *
 * \param handle The module to be queried.
 * \param name Name of the argument.
 * \param [out] dest Pointer where to store the pointer.
 *
 * \return \ref PNMPI_SUCCESS The argument was found and may be accessed by
 *  pointer \p dest now.
 * \return \ref PNMPI_NOMODULE \p handle is no valid module handle.
 * \return \ref PNMPI_NOARG The argument can't be found.
 *
 *
 * \ingroup PNMPI_Service_GetArgument
 */
PNMPI_status_t PNMPI_Service_GetArgument(const PNMPI_modHandle_t handle,
                                         const char *name, const char **dest)
{
  assert(name);
  assert(dest);

  /* Check, if module is available and return an error code, if it's not
   * available.
   *
   * NOTE: This check will NOT be disabled for PNMPI_NO_DEBUG, as it's essential
   *       for accessing the module by ID below */
  if (!pnmpi_valid_modhandle(handle))
    return PNMPI_NOMODULE;


  /* Iterate over the argument list of the module. Compare the name of each
   * argument with the argument to be searched. If they match copy the pointer
   * to the value into dest and return success. */
  module_arg_p a;
  for (a = modules.module[handle]->args; a != NULL; a = a->next)
    if (strcmp(a->name, name) == 0)
      {
        *dest = a->value;
        return PNMPI_SUCCESS;
      }

  /* An argument with matching name was not found. */
  return PNMPI_NOARG;
}


/** \brief Get argument \p name of own module.
 *
 * \details Each module may be followed by one or more arguments in the
 *  configuration file. This function queries these arguments and returns a
 *  pointer to its value if a matching argument was found.
 *
 * \note Only the first match of \p name will be used.
 *
 *
 * \param name Name of the argument.
 *
 * \return If argument \p name was found, a pointer to its value will be
 *  returned, otherwise the NULL pointer.
 *
 *
 * \ingroup PNMPI_Service_GetArgument
 */
const char *PNMPI_Service_GetArgumentSelf(const char *name)
{
  assert(name);


  /* Iterate over the argument list of the module. Compare the name of each
   * argument with the argument to be searched. If they match return a pointer
   * to the argument value. */
  module_arg_p a;
  for (a = modules.module[pnmpi_level]->args; a != NULL; a = a->next)
    if (strcmp(a->name, name) == 0)
      return a->value;

  /* An argument with matching name was not found. */
  return NULL;
}
