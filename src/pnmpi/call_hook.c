/*
  Copyright (c) 2008
  Lawrence Livermore National Security, LLC.

  Produced at the Lawrence Livermore National Laboratory.
  Written by Martin Schulz, schulzm@llnl.gov.
  LLNL-CODE-402774,
  All rights reserved.

  This file is part of P^nMPI.

  Please also read the file "LICENSE" included in this package for
  Our Notice and GNU Lesser General Public License.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  (as published by the Free Software Foundation) version 2.1
  dated February 1999.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY
  OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  terms and conditions of the GNU General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, write to the

  Free Software Foundation, Inc.,
  59 Temple Place, Suite 330,
  Boston, MA 02111-1307 USA
*/

#include "core.h"


/** \brief Call \p hook in all modules.
 *
 * \details This function iterates over all modules and calls the \p hook in all
 *  of them.
 */
void pnmpi_call_hook(const char *hook)
{
  size_t i;
  for (i = 0; i < modules.num; i++)
    {
      /* Stack delimiter may be ignored: they are no real modules, so they can't
       * have any hook functions defined. */
      if (modules.module[i]->stack_delimiter)
        continue;

      int (*sym)() = (int (*)())find_symbol(modules.module[i], hook);
      if (sym == NULL)
        {
          DBGPRINT2("Module %s has no '%s' hook", modules.module[i]->name,
                    hook);
          continue;
        }

      /* Call the hook's symbol and get the return value. If the hook does not
       * return PNMPI_SUCCESS, the hook execution failed and a error message
       * will be printed. Before calling the hook, the PnMPI level will be
       * adjusted. */
      set_pnmpi_level(i);

      int err = sym();
      if (err != PNMPI_SUCCESS)
        WARNPRINT("Error in '%s' hook in module %s: %i", hook,
                  modules.module[i]->name, err);
    }

  /* Restore PnMPI level. */
  set_pnmpi_level(0);
}
