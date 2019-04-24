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

#ifndef PNMPI_PRIVATE_MPI_INTERFACE_H
#define PNMPI_PRIVATE_MPI_INTERFACE_H


#include <pnmpi/private/attributes.h>
#include <pnmpi/private/config.h>


/** \brief Language of the MPI interface used by the application.
 */
typedef enum pnmpi_mpi_interface {
  PNMPI_INTERFACE_C ///< Application uses the C MPI interface.

#ifdef ENABLE_FORTRAN
  ,
  PNMPI_INTERFACE_FORTRAN ///< Application uses the Fortran MPI interface.
#endif
} pnmpi_mpi_interface;


extern pnmpi_mpi_interface pnmpi_used_mpi_interface;


#ifdef ENABLE_FORTRAN
/** \brief Set the MPI interface used for the current MPI call to Fortran.
 *
 * \details Some parts of PnMPI (and maybe some modules, too) depend on the
 *  interface used to call a MPI function. E.g. when initializing MPI, the
 *  wrapper has to call the PMPI function for the right interface.
 *
 *  By default interface to use is \ref PNMPI_INTERFACE_C, but it may be changed
 *  to \ref PNMPI_INTERFACE_FORTRAN by calling this function when entering a
 *  Fortran MPI wrapper.
 *
 *
 * \private
 */
PNMPI_UNUSED
static void pnmpi_set_mpi_interface_fortran(void)
{
  pnmpi_used_mpi_interface = PNMPI_INTERFACE_FORTRAN;
}


/** \brief Reset the MPI interface used for the current MPI call.
 *
 * \details This function will reset the MPI interface for the current MPI call
 *  to its initial value \ref PNMPI_INTERFACE_C. It should be called when
 *  leaving a Fortran MPI wrapper function.
 *
 *
 * \private
 */
PNMPI_UNUSED
static void pnmpi_reset_mpi_interface(void)
{
  pnmpi_used_mpi_interface = PNMPI_INTERFACE_C;
}
#endif


/** \brief Get the MPI interface for the current MPI call.
 *
 *
 * \return The MPI interface used by the application for calling the current MPI
 *  call.
 *
 *
 * \private
 */
PNMPI_UNUSED
static pnmpi_mpi_interface pnmpi_get_mpi_interface(void)
{
  return pnmpi_used_mpi_interface;
}


#endif
