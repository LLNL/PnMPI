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

/*
 *   This code is an extension of the MPICH wrapper generator
 *   included in the MPICH 1.2.7p1 release. This code has been
 *   released with the following notice: 
 *
 *   Copyright (C) 1997 University of Chicago.
 *   See COPYRIGHT notice in the wrappergen directory.
 */

/* args.h - argument handling stuff */

#ifndef _ARGS_H_
#define _ARGS_H_

#ifdef __STDC__

int GetIntArg( int *argc, char **argv, char *switchName, int *val );
int GetDoubleArg( int *argc, char **argv, char *switchName, double *val );
int GetStringArg( int *argc, char **argv, char *switchName, char **val );
int IsArgPresent( int *argc, char **argv, char *switchName );
int GetArgAdjacentString( int *argc, char **argv, char *switchName,
			  char **value );
int GetIntListArg( int *argc, char **argv, char *switchName,
	       int **intList, int *listLen );
int GetStringListArg( int *argc, char **argv, char *switchName,
		  char ***strList, int *listLen );
#else
int GetIntArg( );
int GetDoubleArg( );
int IsArgPresent( );
int GetArgAdjacentString( );
int GetIntListArg();
int GetStringListArg( );
#endif


#endif
/* _ARGS_H_ */
