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
#include <pnmpi/private/initialization.h>
#include <pnmpi/private/modules.h>


/** \brief Finalize PnMPI.
 *
 * \note This function must be called as many times as \ref pnmpi_initialize.
 *
 * \warning PnMPI API functions **MUST NOT** be used after calling this
 * function.
 *
 *
 * \private
 */
void pnmpi_finalize(void)
{
  /* Ignore any call to this function except the last call. This will be
   * achieved by decreasing the initialization counter. If it's zero after
   * decreasing, this is the last call to the finalization function and PnMPI
   * needs to be finalized now. */
  if (--pnmpi_initialized)
    return;

  /* Call the PnMPI finalization functions. These will unload the PnMPI modules
   * and free allocated memory. */
  pnmpi_modules_unload();
}


#ifdef __GNUC__
/** \brief The PnMPI destructor.
 *
 * \details If the compiler supports destructors, finalize PnMPI in the
 *  destructor, just before the application finishes.
 *
 *
 * \private
 */
PNMPI_INTERNAL
__attribute__((destructor)) void pnmpi_destructor(void)
{
  pnmpi_finalize();
}
#endif
