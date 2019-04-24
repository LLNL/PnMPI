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

/** \file
 *
 * \details This file contains compiler-dependent macros, which set function or
 *  variable attributes for the compiler, if they are supported.
 *
 *
 * \privatesection
 */


#ifndef PNMPI_PRIVATE_ATTRIBUTES_H
#define PNMPI_PRIVATE_ATTRIBUTES_H


/** \brief Mark function as internal.
 *
 * \details This macro will be used to mark functions as internal functions,
 *  which will be hidden for global exports.
 */
#ifdef __GNUC__
#define PNMPI_INTERNAL __attribute__((visibility("hidden")))
#else
#define PNMPI_INTERNAL
#endif


/** \brief Mark function as possibly unused.
 *
 * \details When defining static functions inside a header, possibly not all
 *  functions will be used by all source files including this header. By using
 *  this macro, the compiler knows about this and will not print a warning about
 *  an unused function.
 */
#ifdef __GNUC__
#define PNMPI_UNUSED __attribute__((unused))
#else
#define PNMPI_UNUSED
#endif


#endif
