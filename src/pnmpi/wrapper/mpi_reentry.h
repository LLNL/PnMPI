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

#ifndef PNMPI_PRIVATE_MPI_REENTRY_H
#define PNMPI_PRIVATE_MPI_REENTRY_H


#include <pnmpi/private/attributes.h>
#include <pnmpi/private/tls.h>


extern pnmpi_compiler_tls_keyword int pnmpi_mpi_reentry;


/** \brief Try to enter the reentry-guarded section.
 *
 * \details This function tries to enter the reentry-guarded section. It may be
 *  used inside an if-expression to execute code depending on the returned
 *  status. The default would be to call the PMPI function, if entering the
 *  section fails.
 *
 *
 * \return If the section could be entered, this expression will be zero,
 *  otherwise one.
 *
 *
 * \private
 */
PNMPI_UNUSED
static int pnmpi_reentry_enter(void)
{
  return (pnmpi_mpi_reentry == 1 || pnmpi_mpi_reentry++);
}


/** \brief Leave the reentry-guarded section.
 *
 *
 * \private
 */
PNMPI_UNUSED
static void pnmpi_reentry_exit(void)
{
  pnmpi_mpi_reentry = 0;
}


#endif
