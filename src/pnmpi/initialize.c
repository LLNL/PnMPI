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
#include <pnmpi/private/initialization.h>


/** \brief Global counter for number of initializations.
 *
 * \details This counter will be increased on any call to the initialization
 *  functions and decreased on finalization. It ensures PnMPI is initialized
 *  only once and finalized in the last finalization call.
 *
 *
 * \private
 */
PNMPI_INTERNAL
int pnmpi_initialized = 0;


/** \brief Initialize PnMPI.
 *
 * \note This function must be called before using any of PnMPI's API functions.
 *
 *
 * \private
 */
PNMPI_INTERNAL
void pnmpi_initialize(void)
{
  /* If PnMPI is already initialize, do not initialize it a second time. A
   * counter will be increased to be save how often the initialization has been
   * called, to be used later in the finalization. */
  if (pnmpi_initialized++)
    return;

  /* Call the PnMPI initialization functions. These will load the PnMPI
   * configuration, parse it and load all the modules defined in the config. */
  pnmpi_PreInit();
}


#ifdef __GNUC__
/** \brief The PnMPI constructor.
 *
 * \details If the compiler supports constructors, initialize PnMPI this early,
 *  so PnMPI is ready to be used, when `main()` gets called.
 *
 *
 * \param argc Count of \p argv.
 * \param argv The argument vector of the executable.
 *
 *
 * \private
 */
PNMPI_INTERNAL
__attribute__((constructor)) void pnmpi_constructor(PNMPI_UNUSED int argc,
                                                    PNMPI_UNUSED char **argv)
{
  pnmpi_initialize();
}
#endif
