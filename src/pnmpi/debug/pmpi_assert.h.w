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
 * \brief Assertion wrappers for PMPI functions.
 *
 * \details Macros in this header wrap the PMPI function calls to call the PMPI
 *  function and checking the return code. If MPI_SUCCESS is not found,
 *  \ref PNMPI_Error will be called.
 *
 * \note Macros will be used instead of inline functions, so \ref PNMPI_Error
 *  will print the right line numbers.
 */


#ifndef PNMPI_PRIVATE_PMPI_ASSERT_H
#define PNMPI_PRIVATE_PMPI_ASSERT_H


#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/private/config.h>


{{forallfn fn_name}}
#define P{{fn_name}}_assert({{args}})          \
  {                                            \
    if (P{{fn_name}}({{args}}) != MPI_SUCCESS) \
      PNMPI_Error("P{{fn_name}} failed.\n");   \
  }
{{endforallfn}}


/* If PnMPI will be compiled for the Fortran MPI interface, wrappers for the
 * used Fortran PMPI functions are also required. */
#ifdef ENABLE_FORTRAN

#define pmpi_init_assert_(ierror)          \
  {                                        \
    pmpi_init_(ierror);                    \
    if (*ierror != MPI_SUCCESS)            \
      PNMPI_Error("pmpi_init_ failed.\n"); \
  }

#ifdef HAVE_MPI_INIT_THREAD_Fortran
#define pmpi_init_thread_assert_(required, provided, ierror) \
  {                                                          \
    pmpi_init_thread_(required, provided, ierror);           \
    if (*ierror != MPI_SUCCESS)                              \
      PNMPI_Error("pmpi_init_thread_ failed.\n");            \
  }
#endif

#define pmpi_finalize_assert_(ierror)          \
  {                                            \
    pmpi_finalize_(ierror);                    \
    if (*ierror != MPI_SUCCESS)                \
      PNMPI_Error("pmpi_finalize_ failed.\n"); \
  }

#endif


#endif
