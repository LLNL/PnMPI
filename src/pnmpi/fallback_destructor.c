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
