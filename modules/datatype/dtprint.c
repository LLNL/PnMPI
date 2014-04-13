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

#include <stdio.h>
#include <stdlib.h>

#include "datatype.h"
#include <mpi.h>
#include <pnmpimod.h>

#define PNMPI_MODULE_DATATYPEPRINT "datatype-print"

PNMPIMOD_Datatype_getReference_t r_get;
PNMPIMOD_Datatype_delReference_t r_del;


/*------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;

  /* register this module and its services */

  err = PNMPI_Service_RegisterModule(PNMPI_MODULE_DATATYPEPRINT);
  if (err != PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  return err;
}

/*.......................................................*/
/* Init */

int MPI_Init(int *argc, char ***argv)
{
  int err;
  PNMPI_modHandle_t handle;
  PNMPI_Service_descriptor_t s_del, s_get;


  /* call the init routines */

  err = PMPI_Init(argc, argv);
  if (err != MPI_SUCCESS)
    return err;

  /* query the other modules */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_DATATYPE, &handle);
  if (err != MPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetServiceByName(handle, "getDatatypeReference", "pimp",
                                       &s_get);
  if (err != MPI_SUCCESS)
    return err;
  r_get = (PNMPIMOD_Datatype_getReference_t)s_get.fct;

  err =
    PNMPI_Service_GetServiceByName(handle, "delDatatypeReference", "p", &s_del);
  if (err != MPI_SUCCESS)
    return err;
  r_del = (PNMPIMOD_Datatype_delReference_t)s_del.fct;

  return err;
}


/*------------------------------------------------------------*/
/* Isend */

int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *req)
{
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s;
  MPI_Datatype t;

  r_get(buf, count, datatype, &ref);

  printf("Sending to %i :\n", dest);
  do
    {
      PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
        ;
#endif
      printf("\t%i ", l);
      if (t == MPI_INT)
        printf("INT   ");
      else if (t == MPI_SHORT)
        printf("SHORT ");
      else if (t == MPI_LONG)
        printf("LONG  ");
      else if (t == MPI_CHAR)
        printf("CHAR  ");
      else if (t == MPI_DOUBLE)
        printf("DOUBLE");
      else if (t == MPI_FLOAT)
        printf("FLOAT ");
      else
        printf("Other");

      printf(" of size %i at buf %16p / %li\n", s, b, ((long)b) - ((long)buf));
      fflush(stdout);
    }
  while (!done);

  r_del(&ref);

  return PMPI_Isend(buf, count, datatype, dest, tag, comm, req);
}


/*------------------------------------------------------------*/
/* Send */

int MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
             MPI_Comm comm)
{
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s;
  MPI_Datatype t;

  r_get(buf, count, datatype, &ref);

  printf("Sending to %i :\n", dest);
  do
    {
      PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
        ;
#endif
      printf("\t%i ", l);
      if (t == MPI_INT)
        printf("INT   ");
      else if (t == MPI_SHORT)
        printf("SHORT ");
      else if (t == MPI_LONG)
        printf("LONG  ");
      else if (t == MPI_CHAR)
        printf("CHAR  ");
      else if (t == MPI_DOUBLE)
        printf("DOUBLE");
      else if (t == MPI_FLOAT)
        printf("FLOAT ");
      else
        printf("Other");

      printf(" of size %i at buf %16p / %li\n", s, b, ((long)b) - ((long)buf));
      fflush(stdout);
    }
  while (!done);

  r_del(&ref);

  return PMPI_Send(buf, count, datatype, dest, tag, comm);
}
