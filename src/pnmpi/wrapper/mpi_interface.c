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
#include <pnmpi/private/mpi_interface.h>


/** \brief MPI interface used for an MPI call.
 *
 * \details This variable stores the MPI interface used by the application for
 *  an MPI call. By default the interface is \ref PNMPI_INTERFACE_C, but by
 *  entering a Fortran MPI wrapper function, this will be set to \ref
 *  PNMPI_INTERFACE_FORTRAN.
 *
 * \note This value should not be changed directly by any function, but by using
 *  the \ref pnmpi_reentry_enter and \ref pnmpi_reentry_exit functions.
 *
 *
 * \private
 */
PNMPI_INTERNAL
pnmpi_mpi_interface pnmpi_used_mpi_interface = PNMPI_INTERFACE_C;
