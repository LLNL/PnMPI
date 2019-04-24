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


/** \brief Call \p hook recursively at the current stack.
 *
 * \details This function should be called by modules implementing a hook to
 *  recurse into the hooks of other modules at the same stack, so one can
 *  control which code to execute before and after the hooks of other modules.
 *
 * \warning Do not use this function in
 *  \ref pnmpi_module_hooks::PNMPI_RegistrationPoint, but only in recursive
 *  hooks like custom ones.
 *
 *
 * \param hook Name of the hook to call. Use \ref PNMPI_HOOK_SAME for recursing
 *  in the current hook.
 *
 *
 * \ingroup PNMPI_Service_CallHook
 */
void PNMPI_Service_CallHook(const char *hook)
{
  assert(hook);


  pnmpi_call_hook(hook, PNMPI_CALL_HOOK_NEXT_MODULE, pnmpi_level + 1);
}


/** \brief Call \p hook recursively at \p stack.
 *
 * \details This function has the same functionality as
 *  \ref PNMPI_Service_CallHook, except a \p stack to switch on can be defined.
 *
 *
 * \param hook Name of the hook to call. Use \ref PNMPI_HOOK_SAME for recursing
 *  in the current hook.
 * \param stack At which stack the hook should be called.
 *
 *
 * \ingroup PNMPI_Service_CallHook
 */
void PNMPI_Service_CallHook_NewStack(const char *hook,
                                     const PNMPI_modHandle_t stack)
{
  assert(hook);


  pnmpi_call_hook(hook, PNMPI_CALL_HOOK_NEXT_MODULE, stack);
}
