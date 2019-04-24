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
#include <pnmpi/debug_io.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/modules.h>


/** \brief Call \p hook in loaded modules.
 *
 * \details This function iterates over the modules in the current stack and
 *  calls the \p hook in all of them.
 *
 *
 * \param hook Name of the hook to be called.
 * \param all_modules If set to non-zero, \p hook will be called in all modules,
 *  not just those in the current stack.
 * \param start_level At which level to start looking for \p hook.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_call_hook(const char *hook,
                     const enum pnmpi_call_hook_mode all_modules,
                     const PNMPI_modHandle_t start_level)
{
  assert(hook);

  /* NOTE: start_level will NOT be checked to be a valid modhandle, as a valid
   *       usecase is to call this function at the bottom of a stack with
   *       'current level + 1' as start_level. The for-loop below will take care
   *       of using just valid handles. */


  PNMPI_Debug(PNMPI_DEBUG_MODULE, "Checking for %s hook in %s.\n", hook,
              all_modules ? "all modules" : "the next available module");


  /* Save the current PnMPI level to restore it later. */
  int pnmpi_level_restore = pnmpi_level;

  /* Iterate over the array of modules. */
  for (pnmpi_level = start_level; pnmpi_level < modules.num; pnmpi_level++)
    {
      /* If the current level is a stack delimiter, this level should be
       * ignored. If the hook should be called in all modules, we'll go to the
       * next level, otherwise the loop will be exited. */
      if (modules.module[pnmpi_level]->stack_delimiter)
        {
          if (all_modules == PNMPI_CALL_HOOK_ALL_MODULES)
            continue;
          else
            break;
        };

      /* Check if the module provides the required hook and call it if it's
       * available. */
      void (*sym)() =
        (void (*)())find_symbol(modules.module[pnmpi_level], hook);
      if (sym != NULL)
        {
          PNMPI_Debug(PNMPI_DEBUG_MODULE, "Calling %s hook in module '%s'.\n",
                      hook, modules.module[pnmpi_level]->name);
          sym();

          /* If the hook should be called in all modules, don't break after the
           * first invocation. Otherwise the hook has to call a service function
           * for recursive hook invocations.  */
          if (all_modules == PNMPI_CALL_HOOK_NEXT_MODULE)
            break;
        }
    }

  /* Restore PnMPI level. */
  pnmpi_level = pnmpi_level_restore;
}
