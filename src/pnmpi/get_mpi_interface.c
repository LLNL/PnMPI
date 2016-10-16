/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

#include "app_hooks.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include <pnmpi/private/attributes.h>


/** \brief Get language of instrumented application.
 *
 * \details This function will search for indicates of a specific language the
 *  application was written in. This should help to indicate the used MPI
 *  interface, so PnMPI can decide wheter to use C or Fortran for startup.
 *
 *
 * \return \ref PNMPI_INTERFACE_C The application seems to be written in C.
 * \return \ref PNMPI_INTERFACE_Fortran The application seems to be written in
 *  Fortran.
 */
PNMPI_INTERNAL
pnmpi_mpi_interface pnmpi_get_mpi_interface()
{
  /* C applications will have a symbol called 'main', Fortran applications
   * have one, too - but you can't access it via dlsym. If we can access a
   * symbol called 'main', we known that this is a C application.
   *
   * Note: One may override the name of the 'main' symbol and compilers not
   * known at the time of writing this piece of code may call it different, too.
   * Even if we know the language of the binary, that is no completly safe
   * indicator, that the application uses the MPI interface of its language: One
   * may use the Fortran MPI interface in C too. This code checks the *default*
   * known behaviour only. */

  void *handle = dlopen(NULL, RTLD_LAZY | RTLD_GLOBAL);
  if (!handle)
    {
      fprintf(stderr, "PnMPI: Can't detect application language: %s\n",
              dlerror());
      exit(EXIT_FAILURE);
    }

  dlerror();
  pnmpi_mpi_interface interface =
    dlsym(handle, "main") ? PNMPI_INTERFACE_C : PNMPI_INTERFACE_Fortran;
  dlclose(handle);

  return interface;
}
