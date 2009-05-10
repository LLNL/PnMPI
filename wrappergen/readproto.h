/*
 *   This code is an extension of the MPICH wrapper generator
 *   included in the MPICH 1.2.7p1 release. This code has been
 *   released with the following notice: 
 *
 *   Copyright (C) 1997 University of Chicago.
 *   See COPYRIGHT notice in the wrappergen directory.
 */

int IsNameInList( char *name, char **list, int nitems );
int oldstyle_function( void );
int ReadFnProto( char *fn, char **fn_names, int n_fn_names, fn_def **fn_list, int *n_fn );
int ReadFnProto( char *fn, char **fn_names, int n_fn_names, fn_def **fn_list, int *n_fn );
int ReadFnList( char *fn, char ***fn_names, int *n_fn );

