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
 */


#ifndef PNMPI_ATTRIBUTES_H
#define PNMPI_ATTRIBUTES_H


/** \brief Mark function arguments as non-null.
 *
 * \note This macro should be attached to the declaration in the header file.
 *
 *
 * \param ... List of arguments that should be non-null.
 */
#if defined(__GNUC__) || defined(__clang__)
#define PNMPI_FUNCTION_ARG_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define PNMPI_FUNCTION_ARG_NONNULL(...)
#endif


/** \brief Mark function as printf-like function.
 *
 * \note This macro should be attached to the declaration in the header file.
 *
 *
 * \param format Argument number of format string (starting with 1).
 * \param arguments Argument number of first format argument (starting with 1).
 */
#if defined(__GNUC__) || defined(__clang__)
#define PNMPI_FUNCTION_PRINTF(format, arguments) \
  __attribute__((__format__(__printf__, format, arguments)))
#else
#define PNMPI_FUNCTION_PRINTF(format, arguments)
#endif


/** \brief Mark function as non-return function like exit and abort.
 *
 * \note This macro should be attached to the declaration in the header file.
 */
#if defined(__GNUC__) || defined(__clang__)
#define PNMPI_FUNCTION_NORETURN __attribute__((noreturn))
#else
#define PNMPI_FUNCTION_NORETURN
#endif


#endif
