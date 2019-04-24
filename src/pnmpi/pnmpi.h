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

/* Public interface for PNMPI */
/* To be used by applications that are aware of PNMPI */

#include <mpi.h>

#include "pnmpi-config.h"
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
