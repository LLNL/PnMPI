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

/* Declaration of all destructors. They are not in a seperate header file, as
 * they will be used in this file only. If the normal destructors are enabled,
 * no other files call them.
 *
 * The declarations are outside of the ifndef __GNUC__, because otherwise this
 * file will be empty if __GNUC__ is defined, which is forbidden in ISO C. */
void pnmpi_app_shutdown();


/* Only enable the fallback constructor for builds without individual
 * constructors enabled. */
#ifndef __GNUC__


/** \brief Fallback destructor.
 *
 * \details If the compiler does not support '__attribute__((destructor))', the
 *  following fallback destructor will be used to call the independend
 *  destructors of PnMPI.
 */
void _fini()
{
  pnmpi_app_shutdown();
}


#endif
