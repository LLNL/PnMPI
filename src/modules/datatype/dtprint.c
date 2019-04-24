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

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/const.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>

#include "datatype.h"


#define PNMPI_MODULE_DATATYPEPRINT "datatype-print"

PNMPIMOD_Datatype_getReference_t r_get;
PNMPIMOD_Datatype_delReference_t r_del;


/*------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

void PNMPI_RegistrationPoint()
{
  PNMPI_Service_RegisterModule(PNMPI_MODULE_DATATYPEPRINT);
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

int MPI_Isend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm, MPI_Request *req)
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

int MPI_Send(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
             int tag, MPI_Comm comm)
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
