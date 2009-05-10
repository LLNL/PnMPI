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
