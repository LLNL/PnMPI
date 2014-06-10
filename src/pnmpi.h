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

/* Public interface for PNMPI */
/* To be used by applications that are aware of PNMPI */

#include <mpi.h>

#include "pnmpimod.h"

#define PNMPI_PCONTROL_LEVEL 3

#define PNMPI_PCONTROL_SINGLE 0x0
#define PNMPI_PCONTROL_MULTIPLE 0x1
#define PNMPI_PCONTROL_VARG 0x0
#define PNMPI_PCONTROL_PTR 0x2

#define PNMPI_PCONTROL_PMPI 0
#define PNMPI_PCONTROL_OFF 1
#define PNMPI_PCONTROL_ON 2
#define PNMPI_PCONTROL_PNMPI 3
#define PNMPI_PCONTROL_MIXED 4
#define PNMPI_PCONTROL_INT 5
#define PNMPI_PCONTROL_TYPED 6

#define PNMPI_PCONTROL_DEFAULT PNMPI_PCONTROL_INT

#define PNMPI_PCONTROL_TYPE_INT 0
#define PNMPI_PCONTROL_TYPE_LONG 1
#define PNMPI_PCONTROL_TYPE_PTR 2
#define PNMPI_PCONTROL_TYPE_DOUBLE 3

#define PNMPI_ERROR -1

#define PNMPI_MAX_THREADED MPI_THREAD_SINGLE
