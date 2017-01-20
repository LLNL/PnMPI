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

/* This test module is used to check if PnMPI hits all hooks in modules. */

#include <stdio.h>

#include <pnmpi/hooks.h>
#include <pnmpi/private/attributes.h>


/** \brief Helper macro to generate our check functions.
 *
 *
 * \param name The hooks name.
 */
#define check_hook(name)                                                     \
  void name()                                                                \
  {                                                                          \
    printf("%s hit.\n", __FUNCTION__);                                       \
  }                                                                          \
  /* As C complains about an extra semicolon after the function declaration, \
   * we have to add a variable declaration here which will be never used. */ \
  PNMPI_INTERNAL                                                             \
  int invisible


/* Add check functions for our hooks. */
check_hook(PNMPI_RegistrationPoint);
check_hook(PNMPI_RegistrationComplete);
check_hook(app_startup);
check_hook(app_shutdown);
