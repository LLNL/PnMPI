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

#include <pnmpi/private/attributes.h>
#include <pnmpi/private/modules.h>


/** \brief Unload all modules.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_modules_unload(void)
{
  /* Trigger the PNMPI_Fini hook in all modules, so they may do some
   * finalization steps before they will be unloaded. */
  pnmpi_call_hook("PNMPI_Fini", PNMPI_CALL_HOOK_ALL_MODULES, 0);

  /* Trigger the PNMPI_Unregister hook in all modules, so they may destruct
   * initialized objects and free allocated memory. In contrast to the
   * PNMPI_Fini hook above, the modules are not allowed to talk to each other in
   * this hook, as there is no guaranty, that other hooks still exist. */
  pnmpi_call_hook("PNMPI_UnregistrationPoint", PNMPI_CALL_HOOK_ALL_MODULES, 0);
}
