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

#ifndef PNMPI_APP_HOOKS_H
#define PNMPI_APP_HOOKS_H


/** \brief Type of MPI interface to be used by the application.
 */
typedef enum pnmpi_mpi_interface {
  PNMPI_INTERFACE_C,      ///< Application uses the C MPI interface.
  PNMPI_INTERFACE_Fortran ///< Application uses the Fortran MPI interface.
} pnmpi_mpi_interface;


/** \brief Cache for 'provided' return value of PMPI_Init_thread.
 *
 * \details PMPI_Init_thread returns the provided MPI threading level. The value
 *  will be stored in this variable, to pass it to the application in later
 *  calls to \ref MPI_Init_thread.
 */
extern int pnmpi_mpi_thread_level_provided;


void read_cmdline(int *argc, char ***argv);
void pnmpi_app_startup();
void pnmpi_app_shutdown();
pnmpi_mpi_interface pnmpi_get_mpi_interface();


#endif
