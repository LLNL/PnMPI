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

#include <pnmpi/private/app_hooks.h>
#include <pnmpi/private/attributes.h>
#include <pnmpi/private/fallback_init.h>
#include <pnmpi/private/modules.h>


#ifdef PNMPI_HAVE_NO_DESTRUCTOR
/** \brief Fallback destructor.
 *
 * \details If the compiler does not support an implementation for \ref
 *  PNMPI_DESTRUCTOR, the following fallback destructor will be used to call the
 *  independend destructors of PnMPI.
 *
 * \note Using \ref _fini is obsolute, dangerous and is only a fallback for
 *  legacy systems. If PnMPI has no compiler specific implementation of
 *  PNMPI_DESTRUCTOR for your compiler yet, please file an issue.
 */
void _fini()
{
  pnmpi_app_shutdown();
}
#endif


/** \brief Fallback destructor.
 *
 * \details If the compiler does not support an implementation for \ref
 *  PNMPI_DESTRUCTOR nor does \ref _fini get called by the dynamic loader, this
 *  function will be called by \ref MPI_Finalize as a backup.
 *
 * \note This function can't call all of the destructor functions, as some of
 *  them, especially \ref pnmpi_app_shutdown, must be called before the
 *  execution of `main`.
 *
 *
 * \return If the regular destructors will be called, zero will be returned.
 *  Otherwise the return value will be non-zero, indicating the MPI environment
 *  must be shutdown in \ref MPI_Finalize.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_fallback_fini()
{
  /* If any constructor (either the compiler specific or fallback one) has been
   * called before, execution of this function should be skipped. */
  if (pnmpi_constructor_called)
    return;


  /* Call the optional version of PNMPI_AppShutdown, that may be called at
   * initialization time. */
  pnmpi_call_hook("PNMPI_AppShutdownOptional");
}
