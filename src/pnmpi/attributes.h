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
 * \param x Priority of the constructor. Priorities 0-100 are reserved by the
 *  implementation.
 */
#ifdef __GNUC__
#ifndef __APPLE__
#define PNMPI_CONSTRUCTOR(x) __attribute__((constructor(x))) static
#else
#define PNMPI_CONSTRUCTOR(x) __attribute__((constructor)) static
#endif
#else
#define PNMPI_CONSTRUCTOR(x)
#endif


/** \brief Destructor macro.
 *
 * \details Destructors are a GNU extensions. They must be static to avoid
 *  nasty reordering by name instead of priority. If the compiler does not
 *  support destructors, the function won't be static and can be accessed by
 *  the fallback constructor \ref _fini.
 *
 *
 * \param x Priority of the destructor. Priorities 0-100 are reserved by the
 *  implementation.
 */
#ifdef __GNUC__
#ifndef __APPLE__
#define PNMPI_DESTRUCTOR(x) __attribute__((destructor(x))) static
#else
#define PNMPI_DESTRUCTOR(x) __attribute__((destructor)) static
#endif
#else
#define PNMPI_DESTRUCTOR(x)
#endif


#endif
