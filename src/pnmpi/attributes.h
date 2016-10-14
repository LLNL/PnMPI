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

/** \file
 *
 * \details This file contains compiler-dependent macros, which set function or
 *  variable attributes for the compiler, if they are supported.
 */


#ifndef PNMPI_ATTRIBUTES_H
#define PNMPI_ATTRIBUTES_H


/** \brief Constructor macro.
 *
 * \details Constructors are a GNU extensions. They must be static to avoid
 *  nasty reordering by name instead of priority. If the compiler does not
 *  support constructors, the function won't be static and can be accessed by
 *  the fallback constructor \ref _init.
 *
 *
 * \param priority Priority of the constructor. Priorities 0-100 are reserved by
 *  the implementation.
 */
#ifdef __GNUC__
#ifndef __APPLE__
#define PNMPI_CONSTRUCTOR(priority) \
  __attribute__((constructor(priority))) static
#else
#define PNMPI_CONSTRUCTOR(priority) __attribute__((constructor)) static
#endif
#else
#define PNMPI_CONSTRUCTOR(priority)
#endif


/** \brief Destructor macro.
 *
 * \details Destructors are a GNU extensions. They must be static to avoid
 *  nasty reordering by name instead of priority. If the compiler does not
 *  support destructors, the function won't be static and can be accessed by
 *  the fallback constructor \ref _fini.
 *
 *
 * \param priority Priority of the destructor. Priorities 0-100 are reserved by
 *  the implementation.
 */
#ifdef __GNUC__
#ifndef __APPLE__
#define PNMPI_DESTRUCTOR(priority) __attribute__((destructor(priority))) static
#else
#define PNMPI_DESTRUCTOR(priority) __attribute__((destructor)) static
#endif
#else
#define PNMPI_DESTRUCTOR(priority)
#endif


#endif
