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

#include "core.h"
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/modules.h>


/** \brief Check if \p hook is activated in loaded modules.
 *
 * \details This function iterates the modules in the current stack and checks,
 *  if \p hook is defined in any of them.
 *
 *
 * \param hook Name of the hook.
 * \param all_modules If set to non-zero, \p hook will be searched in all
 *  modules, not just those in the current stack.
 *
 * \return If any module implements the \p hook, a non-zero value will be
 *  returned, otherwise zero.
 *
 *
 * \private
 */
PNMPI_INTERNAL
int pnmpi_hook_activated(const char *hook,
                         const enum pnmpi_call_hook_mode all_modules)
{
  int i;
  for (i = (all_modules == PNMPI_CALL_HOOK_ALL_MODULES) ? 0 : pnmpi_level;
       i < modules.num; i++)
    {
      /* If the current level is a stack delimiter, this level should be
       * ignored. If the hook should be searched in all modules, we'll go to the
       * next level, otherwise the loop will be exited. */
      if (modules.module[pnmpi_level]->stack_delimiter)
        {
          if (all_modules == PNMPI_CALL_HOOK_ALL_MODULES)
            continue;
          else
            break;
        };

      if (find_symbol(modules.module[i], hook) != NULL)
        return 1;
    }

  return 0;
}
