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

#ifndef PNMPI_PRIVATE_MPI_INTERFACE_H
#define PNMPI_PRIVATE_MPI_INTERFACE_H


/** \brief Language of the MPI interface used by the application.
 */
typedef enum pnmpi_mpi_interface {
  PNMPI_INTERFACE_UNKNOWN = 0, /**< MPI interface could not be detected or was
                                  *   not detected yet. */
  PNMPI_INTERFACE_C,           ///< Application uses the C MPI interface.
  PNMPI_INTERFACE_FORTRAN      ///< Application uses the Fortran MPI interface.
} pnmpi_mpi_interface;


pnmpi_mpi_interface pnmpi_get_mpi_interface(const char *cmd);


#endif
