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

#ifndef PNMPI_PRIVATE_MODULES_H
#define PNMPI_PRIVATE_MODULES_H


#include <pnmpi/private/attributes.h>
#include <pnmpi/service.h>

#include "core.h"


/** \brief Enum to control different modes of hook functions.
 */
enum pnmpi_call_hook_mode
{
  /** \brief Call the hook in the next module of the current stack, only. */
  PNMPI_CALL_HOOK_NEXT_MODULE,

  /** \brief Call the hook in all modules of all stacks. */
  PNMPI_CALL_HOOK_ALL_MODULES,

  /** \brief Call the hook in all modules of the current stack. */
  PNMPI_CALL_HOOK_CURRENT_STACK
};


/** \brief Check if \p handle is a valid module handle.
 *
 *
 * \param handle Module handle to be checked.
 *
 * \return If \p handle is a valid module handle, a positive integer will be
 *  returned, otherwise zero.
 *
 *
 * \private
 */
PNMPI_UNUSED
static int pnmpi_valid_modhandle(const PNMPI_modHandle_t handle)
{
  return (handle >= 0 && handle < modules.num &&
          !modules.module[handle]->stack_delimiter);
}


void pnmpi_modules_unload(void);

int pnmpi_hook_activated(const char *hook,
                         enum pnmpi_call_hook_mode all_modules);
void pnmpi_call_hook(const char *hook, enum pnmpi_call_hook_mode all_modules,
                     PNMPI_modHandle_t start_level);


#endif
