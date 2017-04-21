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

#include "core.h"
#include <pnmpi/debug_io.h>
#include <pnmpi/private/attributes.h>


/** \brief Call \p hook in all modules.
 *
 * \details This function iterates over all modules and calls the \p hook in all
 *  of them.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_call_hook(const char *hook)
{
  /* Save the current PnMPI level to restore it later. */
  int pnmpi_level_restore = pnmpi_level;


  /* Iterate over all modules in all stacks. */
  size_t i;
  for (i = 0; i < modules.num; i++)
    {
      /* Stack delimiter may be ignored: they are no real modules, so they can't
       * have any hook functions defined. */
      if (modules.module[i]->stack_delimiter)
        continue;

      void (*sym)() = (void (*)())find_symbol(modules.module[i], hook);
      if (sym == NULL)
        {
          PNMPI_Debug(PNMPI_DEBUG_MODULE, "Module %s has no '%s' hook.\n",
                      modules.module[i]->name, hook);
          continue;
        }

      /* Call the hook's symbol. Before calling the hook, the PnMPI level will
       * be adjusted, so the hooks function may use service functions. */
      PNMPI_Debug(PNMPI_DEBUG_MODULE, "Calling %s hook in module '%s'.\n", hook,
                  modules.module[i]->name);
      pnmpi_level = i;
      sym();
    }


  /* Restore PnMPI level. */
  pnmpi_level = pnmpi_level_restore;
}
