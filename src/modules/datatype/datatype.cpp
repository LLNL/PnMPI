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

#include "datatype.h"

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/const.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>


using namespace std;

//#define DT_DEBUG

/*------------------------------------------------------------*/
/* globals */

/*.......................................................*/
/* constants */

/*.......................................................*/
/* types */

/*.......................................................*/
/* variables */

map<MPI_Datatype, PNMPIMOD_Datatype_storage_p> datatypetable;
static int check_level;
static int dt_rank;

/*.......................................................*/
/* macros */

#define CHECK_DATATYPE(dt)                                                \
  if (datatypetable.find(dt) == datatypetable.end())                      \
    {                                                                     \
      fprintf(stderr, "ERROR at %s/%i : Can't find datatype\n", __FILE__, \
              __LINE__);                                                  \
      exit(1);                                                            \
    }

#define ERASE_DATATYPE(dt, doit)                          \
  {                                                       \
    CHECK_DATATYPE(dt);                                   \
    PNMPIMOD_Datatype_storage_p ptr2 = datatypetable[dt]; \
    ptr2->usage--;                                        \
    if (ptr2->usage == 0)                                 \
      {                                                   \
        erase_datatype_rec(ptr2, doit);                   \
        if (ptr2->list)                                   \
          {                                               \
            free(ptr2->list);                             \
          }                                               \
        free(ptr2);                                       \
        if (doit)                                         \
          datatypetable.erase(dt);                        \
        else                                              \
          datatypetable[dt] = NULL;                       \
      }                                                   \
  }

#define ERASE_DATATYPE_FROM_REC(dt, ptr2, doit)            \
  {                                                        \
    if (ptr2->usage == 0)                                  \
      {                                                    \
        erase_datatype_rec(ptr2, doit);                    \
        if (ptr2->list)                                    \
          {                                                \
            free(ptr2->list);                              \
          }                                                \
        free(ptr2);                                        \
        if (datatypetable.find(dt) != datatypetable.end()) \
          {                                                \
            if (doit)                                      \
              datatypetable.erase(dt);                     \
            else                                           \
              datatypetable[dt] = NULL;                    \
          }                                                \
      }                                                    \
  }

void erase_datatype_rec(PNMPIMOD_Datatype_storage_p ptr, int doit)
{
  int i;
#ifdef DATATABLEDEBUG
  printf("    Erasing %i / count %i\n", ptr->mpi_dt, ptr->count);
#endif
  if (ptr->list)
    {
      for (i = 0; i < ptr->count; i++)
        {
          ptr->list[i].dt->usage--;
          ERASE_DATATYPE_FROM_REC(ptr->list[i].dt->store_dt, ptr->list[i].dt,
                                  doit);
        }
    }
}


#define ALLOC_DATATYPEBASE(trans, mdt, typ, c, nat)               \
  {                                                               \
    ALLOC_DATATYPEBASESIZE(trans, mdt, mdt, sizeof(typ), c, nat); \
    datatypetable[mdt] = trans;                                   \
  }

#define ALLOC_DATATYPEBASESIZE(trans, mdt, sdt, sz, c, nat) \
  {                                                         \
    trans = (PNMPIMOD_Datatype_storage_p)malloc(            \
      sizeof(PNMPIMOD_Datatype_storage_t));                 \
    if (trans == NULL)                                      \
      {                                                     \
        fprintf(stderr, "ERROR: Can't allocate\n");         \
        exit(1);                                            \
      }                                                     \
    trans->count = c;                                       \
    trans->depth = 1;                                       \
    trans->size = sz * c;                                   \
    trans->realsize = sz * c;                               \
    trans->homogeneous = sz;                                \
    trans->list = NULL;                                     \
    trans->mpi_dt = mdt;                                    \
    trans->store_dt = sdt;                                  \
    trans->usage = 1;                                       \
    trans->native = nat;                                    \
  }

#define ALLOC_DATATYPENOREG(trans, cnt, mdt, nat)      \
  {                                                    \
    int _i;                                            \
    trans = (PNMPIMOD_Datatype_storage_p)malloc(       \
      sizeof(PNMPIMOD_Datatype_storage_t));            \
    if (trans == NULL)                                 \
      {                                                \
        fprintf(stderr, "ERROR: Can't allocate\n");    \
        exit(1);                                       \
      };                                               \
    trans->list = (PNMPIMOD_Datatype_entry_t *)malloc( \
      cnt * sizeof(PNMPIMOD_Datatype_entry_t));        \
    if (trans->list == NULL)                           \
      {                                                \
        fprintf(stderr, "ERROR: Can't allocate\n");    \
        exit(1);                                       \
      };                                               \
    for (_i = 0; _i < cnt; _i++)                       \
      {                                                \
        trans->list[_i].repeat = 1;                    \
        trans->list[_i].step = 0;                      \
        trans->list[_i].block = 1;                     \
        trans->list[_i].start = 0;                     \
      }                                                \
    trans->count = cnt;                                \
    trans->depth = 1;                                  \
    trans->size = 0;                                   \
    trans->realsize = 0;                               \
    trans->homogeneous = 0;                            \
    trans->usage = 1;                                  \
    trans->mpi_dt = mdt;                               \
    trans->store_dt = mdt;                             \
    trans->native = nat;                               \
  }

#define ALLOC_DATATYPE(trans, cnt, mdt, nat)   \
  {                                            \
    ALLOC_DATATYPENOREG(trans, cnt, mdt, nat); \
    datatypetable[mdt] = trans;                \
  }

#define FIX_SUBTYPES(trans)                                                 \
  {                                                                         \
    int i;                                                                  \
    for (i = 0; i < trans->count; i++)                                      \
      {                                                                     \
        if ((trans->list[i].dt->homogeneous) && (trans->list[i].block > 1)) \
          {                                                                 \
            PNMPIMOD_Datatype_storage_p np;                                 \
            trans->list[i].dt->usage--;                                     \
            ALLOC_DATATYPEBASESIZE(                                         \
              np, trans->list[i].dt->mpi_dt, MPI_DATATYPE_NULL,             \
              trans->list[i].dt->homogeneous, trans->list[i].block,         \
              trans->list[i].dt->native);                                   \
            trans->list[i].dt = np;                                         \
            trans->list[i].block = 1;                                       \
          }                                                                 \
      }                                                                     \
  }


#define DEPTH_DATATYPE(trans)               \
  {                                         \
    int i, max = 1;                         \
    for (i = 0; i < trans->count; i++)      \
      {                                     \
        if (trans->list[i].dt->depth > max) \
          max = trans->list[i].dt->depth;   \
      }                                     \
    trans->depth = max + 1;                 \
  }


/*------------------------------------------------------------*/
/* External service routines */

/*.......................................................*/
/* next item */

#ifdef USE_FUNCTIONS

void PNMPIMOD_Datatype_nextItem(PNMPIMOD_Datatype_Parameters_t *ref, int *done)
{
  int cont;
  do
    {
#ifdef DATATABLEDEBUG
      printf("-----------------------------------------------------------------"
             "--------------\n");
      printf("REF l %i / i %i / j %i / b %i\n", ref->level,
             ref->stack[ref->level].i, ref->stack[ref->level].j,
             ref->stack[ref->level].b);
      printf("-----------------------------------------------------------------"
             "--------------\n");
#endif

      cont = 0;
      ref->stack[ref->level].b++;
      if ((ref->stack[ref->level].dt->list == NULL) ||
          (ref->stack[ref->level].b ==
           ref->stack[ref->level].dt->list[ref->stack[ref->level].i].block))
        {
          ref->stack[ref->level].b = 0;
          ref->stack[ref->level].j++;
          if ((ref->stack[ref->level].dt->list == NULL) ||
              (ref->stack[ref->level].j ==
               ref->stack[ref->level]
                 .dt->list[ref->stack[ref->level].i]
                 .repeat))
            {
              ref->stack[ref->level].j = 0;
              ref->stack[ref->level].i++;

              if ((ref->stack[ref->level].i ==
                   ref->stack[ref->level].dt->count) ||
                  (ref->stack[ref->level].dt->homogeneous))
                {
                  ref->level--;
                  cont = 1;
                }
              else
                {
                  ref->stack[ref->level].blockbuf =
                    ref->stack[ref->level].startbuf +
                    ref->stack[ref->level]
                      .dt->list[ref->stack[ref->level].i]
                      .start;
                  ref->stack[ref->level].buf = ref->stack[ref->level].blockbuf;
                }
            }
          else
            {
              ref->stack[ref->level].blockbuf +=
                ref->stack[ref->level].dt->list[ref->stack[ref->level].i].step;
              ref->stack[ref->level].buf = ref->stack[ref->level].blockbuf;
            }
        }
      else
        {
          if (ref->stack[ref->level].b != 0)
            ref->stack[ref->level].buf += ref->stack[ref->level]
                                            .dt->list[ref->stack[ref->level].i]
                                            .dt->size;
        }

      if (cont == 0)
        {
          while (ref->stack[ref->level].dt->homogeneous == 0)
            {
              ref->level++;
              ref->stack[ref->level].i = 0;
              ref->stack[ref->level].j = 0;
              ref->stack[ref->level].b = 0;
              ref->stack[ref->level].startbuf = ref->stack[ref->level - 1].buf;
              ref->stack[ref->level].blockbuf = ref->stack[ref->level - 1].buf;
              ref->stack[ref->level].buf = ref->stack[ref->level - 1].buf;
              ref->stack[ref->level].dt =
                ref->stack[ref->level - 1]
                  .dt->list[ref->stack[ref->level - 1].i]
                  .dt;
            }
        }
    }
  while ((cont) && (ref->level >= 0));

  if (done)
    *done = ref->level < 0;

#ifdef DATATABLEDEBUG
  map<MPI_Datatype, PNMPIMOD_Datatype_storage_p>::iterator i;

  printf("---------------------------------------------------------------------"
         "----------\n");
  printf("OUT l %i / i %i / j %i / b %i\n", ref->level,
         ref->stack[ref->level].i, ref->stack[ref->level].j,
         ref->stack[ref->level].b);
  printf("---------------------------------------------------------------------"
         "----------\n");
  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("OUT Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("OUT Present %i / %i with usage %i / count %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->count);
    }
  printf("---------------------------------------------------------------------"
         "----------\n");
#endif
}

#endif /* not USE_FUNCTIONS */

/*.......................................................*/
/* get item */

#ifdef USE_FUNCTIONS

void PNMPIMOD_Datatype_getItem(PNMPIMOD_Datatype_Parameters_t *ref, char **buf,
                               MPI_Datatype *typ, int *len, int *size,
                               int *last)
{
#ifdef DATATABLEDEBUG
  map<MPI_Datatype, PNMPIMOD_Datatype_storage_p>::iterator i;

  printf("---------------------------------------------------------------------"
         "----------\n");
  printf("l %i / i %i / j %i / b %i\n", ref->level, ref->stack[ref->level].i,
         ref->stack[ref->level].j, ref->stack[ref->level].b);
  printf("---------------------------------------------------------------------"
         "----------\n");
  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("Present %i / %i with usage %i / count %i\n", i->second->mpi_dt,
               i->second->store_dt, i->second->usage, i->second->count);
    }
  printf("---------------------------------------------------------------------"
         "----------\n");
#endif

  *buf = ref->stack[ref->level].buf;
  *typ = ref->stack[ref->level].dt->mpi_dt;
  *len = ref->stack[ref->level].dt->count;
  *size = ref->stack[ref->level].dt->homogeneous;

  PNMPIMOD_Datatype_nextItem(ref, last);
}

#endif /* USE_FUNCTIONS */

/*.......................................................*/
/* start a reference iterator */

int PNMPIMOD_Datatype_getReference(const void *buf, int count, MPI_Datatype dt,
                                   PNMPIMOD_Datatype_Parameters_t *ref)
{
  PNMPIMOD_Datatype_storage_p translated_dt;

#ifdef DATATABLEDEBUG
  map<MPI_Datatype, PNMPIMOD_Datatype_storage_p>::iterator i;

  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("Present %i / %i with usage %i / count %i\n", i->second->mpi_dt,
               i->second->store_dt, i->second->usage, i->second->count);
    }
#endif

  CHECK_DATATYPE(dt);
  translated_dt = datatypetable[dt];

  if (translated_dt->homogeneous)
    {
      ALLOC_DATATYPEBASESIZE(ref->top, translated_dt->mpi_dt, MPI_DATATYPE_NULL,
                             translated_dt->size, count, 0)
    }
  else
    {
      ALLOC_DATATYPENOREG(ref->top, 1, MPI_DATATYPE_NULL, 0);
      ref->top->size = translated_dt->size * count;
      ref->top->list[0].block = count;
      ref->top->list[0].dt = translated_dt;
      DEPTH_DATATYPE(ref->top);
    }

  ref->level = 0;
  ref->stack = (PNMPIMOD_Datatype_stack_t *)malloc(
    sizeof(PNMPIMOD_Datatype_stack_t) * ref->top->depth);
  ref->stack[0].i = 0;
  ref->stack[0].j = 0;
  ref->stack[0].b = -1;
  ref->stack[ref->level].dt = ref->top;
  ref->stack[ref->level].buf = (char *)buf;
  ref->stack[ref->level].blockbuf = (char *)buf;
  ref->stack[ref->level].startbuf = (char *)buf;

  if (translated_dt->homogeneous == 0)
    {
      int done;
      PNMPIMOD_Datatype_nextItem(ref, &done);
    }
  else
    ref->stack[0].b = 0;

  return PNMPI_SUCCESS;
}


/*.......................................................*/
/* next item */

int PNMPIMOD_Datatype_delReference(PNMPIMOD_Datatype_Parameters_t *ref)
{
#ifdef DATATABLEDEBUG
  map<MPI_Datatype, PNMPIMOD_Datatype_storage_p>::iterator i;

  printf("---------------------------------------------------------------------"
         "----------\n");
  printf("BED l %i / i %i / j %i / b %i\n", ref->level,
         ref->stack[ref->level].i, ref->stack[ref->level].j,
         ref->stack[ref->level].b);
  printf("---------------------------------------------------------------------"
         "----------\n");
  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("BED Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("BED Present %i / %i with usage %i / count %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->count);
    }
  printf("---------------------------------------------------------------------"
         "----------\n");
#endif

  if (ref->top->list)
    free(ref->top->list);
  if (ref->top)
    free(ref->top);
  if (ref->stack)
    free(ref->stack);

#ifdef DATATABLEDEBUG
  printf("---------------------------------------------------------------------"
         "----------\n");
  printf("DEL l %i / i %i / j %i / b %i\n", ref->level,
         ref->stack[ref->level].i, ref->stack[ref->level].j,
         ref->stack[ref->level].b);
  printf("---------------------------------------------------------------------"
         "----------\n");
  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("DEL Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("DEL Present %i / %i with usage %i / count %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->count);
    }
  printf("---------------------------------------------------------------------"
         "----------\n");
#endif
  return PNMPI_SUCCESS;
}


/*.......................................................*/
/* get size */

int PNMPIMOD_Datatype_getSize(MPI_Datatype dt, int *sz)
{
  PNMPIMOD_Datatype_storage_p translated_dt;

  CHECK_DATATYPE(dt);
  translated_dt = datatypetable[dt];

  *sz = translated_dt->realsize;

  return PNMPI_SUCCESS;
}

/*------------------------------------------------------------*/
/* Tester */

#ifdef INCLUDE_WRAPPERS

int MPI_Isend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm, MPI_Request *req)
{
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s;
  MPI_Datatype t;

  PNMPIMOD_Datatype_getReference(buf, count, datatype, &ref);

  printf("Sending to %i :\n", dest);
  do
    {
      PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
        ;
#endif
      printf("\t%i ", l);
      switch (t)
        {
        case MPI_INT: printf("INT   "); break;
        case MPI_SHORT: printf("SHORT "); break;
        case MPI_LONG: printf("LONG  "); break;
        case MPI_CHAR: printf("CHAR  "); break;
        case MPI_DOUBLE: printf("DOUBLE"); break;
        case MPI_FLOAT: printf("FLOAT "); break;
        default: printf("Other");
        }
      printf(" of size %i at buf %16p / %li\n", s, b, ((long)b) - ((long)buf));
      fflush(stdout);
    }
  while (!done);

  PNMPIMOD_Datatype_delReference(&ref);

  return PMPI_Isend(buf, count, datatype, dest, tag, comm, req);
}


int MPI_Send(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
             int tag, MPI_Comm comm)
{
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s;
  MPI_Datatype t;

  PNMPIMOD_Datatype_getReference(buf, count, datatype, &ref);

  printf("Sending to %i :\n", dest);
  do
    {
      PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
        ;
#endif
      printf("\t%i ", l);
      switch (t)
        {
        case MPI_INT: printf("INT   "); break;
        case MPI_SHORT: printf("SHORT "); break;
        case MPI_LONG: printf("LONG  "); break;
        case MPI_CHAR: printf("CHAR  "); break;
        case MPI_DOUBLE: printf("DOUBLE"); break;
        case MPI_FLOAT: printf("FLOAT "); break;
        default: printf("Other");
        }
      printf(" of size %i at buf %16p / %li\n", s, b, ((long)b) - ((long)buf));
      fflush(stdout);
    }
  while (!done);

  PNMPIMOD_Datatype_delReference(&ref);

  return PMPI_Send(buf, count, datatype, dest, tag, comm);
}

#endif


/*------------------------------------------------------------*/
/* Wrapper routines */

/*.......................................................*/
/* Registration */

void PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;

  /* register this module and its services */

  err = PNMPI_Service_RegisterModule(PNMPI_MODULE_DATATYPE);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(service.name, "getDatatypeReference");
  service.fct = (PNMPI_Service_Fct_t)(PNMPIMOD_Datatype_getReference);
  sprintf(service.sig, "pimp");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(service.name, "delDatatypeReference");
  service.fct = (PNMPI_Service_Fct_t)(PNMPIMOD_Datatype_delReference);
  sprintf(service.sig, "p");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(service.name, "getDatatypeSize");
  service.fct = (PNMPI_Service_Fct_t)(PNMPIMOD_Datatype_getSize);
  sprintf(service.sig, "mp");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return;
}


/*.......................................................*/
/* Init */

int MPI_Init(int *argc, char ***argv)
{
  int defer_err, err;
  const char *clevel_s;
  PNMPI_modHandle_t handle_datatype;

  /* are we doing checks at the end? */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_DATATYPE, &handle_datatype);
  if (err != PNMPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetArgument(handle_datatype, "check", &clevel_s);
  if (err != PNMPI_SUCCESS)
    {
      if (err == PNMPI_NOARG)
        check_level = 0;
      else
        return err;
    }
  else
    {
      check_level = atoi(clevel_s);
    }


  /* call the init routines */

  defer_err = PMPI_Init(argc, argv);
  if (defer_err != MPI_SUCCESS)
    return defer_err;

  err = PMPI_Comm_rank(MPI_COMM_WORLD, &dt_rank);
  if (err != MPI_SUCCESS)
    return err;

  /* register base types */

  PNMPIMOD_Datatype_storage_p trans;

  /* C */

  ALLOC_DATATYPEBASE(trans, MPI_CHAR, char, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_UNSIGNED_CHAR, unsigned char, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_BYTE, unsigned char, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_PACKED, unsigned char, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_SHORT, short, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_UNSIGNED_SHORT, unsigned short, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_INT, int, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_2INT, int, 2, 1);
  ALLOC_DATATYPEBASE(trans, MPI_UNSIGNED, unsigned int, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_LONG, long, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_UNSIGNED_LONG, unsigned long, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_FLOAT, float, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_DOUBLE, double, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_LONG_LONG, long long, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_LONG_LONG_INT, long long, 1, 1);

  /* Fortran */

  ALLOC_DATATYPEBASE(trans, MPI_CHARACTER, char, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_COMPLEX, float, 2, 1);
  ALLOC_DATATYPEBASE(trans, MPI_DOUBLE_COMPLEX, double, 2, 1);
  ALLOC_DATATYPEBASE(trans, MPI_LOGICAL, int, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_REAL, float, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_2REAL, float, 2, 1);
  ALLOC_DATATYPEBASE(trans, MPI_DOUBLE_PRECISION, double, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_2DOUBLE_PRECISION, double, 2, 1);
  ALLOC_DATATYPEBASE(trans, MPI_INTEGER, int, 1, 1);
  ALLOC_DATATYPEBASE(trans, MPI_2INTEGER, int, 2, 1);
#ifdef MPI_2COMPLEX
  ALLOC_DATATYPEBASE(trans, MPI_2COMPLEX, float, 4, 1);
#endif
#ifdef MPI_2DOUBLE_COMPLEX
  ALLOC_DATATYPEBASE(trans, MPI_2DOUBLE_COMPLEX, double, 4, 1);
#endif

  struct m_l_d
  {
    long a;
    double b;
  };
  ALLOC_DATATYPE(trans, 2, MPI_LONG_DOUBLE, 1);
  trans->list[0].dt = datatypetable[MPI_LONG];
  trans->list[1].dt = datatypetable[MPI_DOUBLE];
  trans->list[1].start = trans->list[0].dt->size;
  trans->size = sizeof(m_l_d);
  trans->realsize = sizeof(m_l_d);
  DEPTH_DATATYPE(trans);
  trans->list[0].dt->usage++;
  trans->list[1].dt->usage++;

  struct m_f_i
  {
    float a;
    int b;
  };
  ALLOC_DATATYPE(trans, 2, MPI_FLOAT_INT, 1);
  trans->list[0].dt = datatypetable[MPI_FLOAT];
  trans->list[1].dt = datatypetable[MPI_INT];
  trans->list[1].start = trans->list[0].dt->size;
  trans->size = sizeof(m_f_i);
  trans->realsize = sizeof(m_f_i);
  DEPTH_DATATYPE(trans);
  trans->list[0].dt->usage++;
  trans->list[1].dt->usage++;

  struct m_d_i
  {
    double a;
    int b;
  };
  ALLOC_DATATYPE(trans, 2, MPI_DOUBLE_INT, 1);
  trans->list[0].dt = datatypetable[MPI_DOUBLE];
  trans->list[1].dt = datatypetable[MPI_INT];
  trans->list[1].start = trans->list[0].dt->size;
  trans->size = sizeof(m_d_i);
  trans->realsize = sizeof(m_d_i);
  DEPTH_DATATYPE(trans);
  trans->list[0].dt->usage++;
  trans->list[1].dt->usage++;

  struct m_l_i
  {
    long a;
    int b;
  };
  ALLOC_DATATYPE(trans, 2, MPI_LONG_INT, 1);
  trans->list[0].dt = datatypetable[MPI_LONG];
  trans->list[1].dt = datatypetable[MPI_INT];
  trans->list[1].start = trans->list[0].dt->size;
  trans->size = sizeof(m_l_i);
  trans->realsize = sizeof(m_l_i);
  DEPTH_DATATYPE(trans);
  trans->list[0].dt->usage++;
  trans->list[1].dt->usage++;

  struct m_s_i
  {
    short a;
    int b;
  };
  ALLOC_DATATYPE(trans, 2, MPI_SHORT_INT, 1);
  trans->list[0].dt = datatypetable[MPI_SHORT];
  trans->list[1].dt = datatypetable[MPI_INT];
  trans->list[1].start = trans->list[0].dt->size;
  trans->size = sizeof(m_s_i);
  trans->realsize = sizeof(m_s_i);
  DEPTH_DATATYPE(trans);
  trans->list[0].dt->usage++;
  trans->list[1].dt->usage++;

  struct m_l_d_i
  {
    long a;
    double b;
    int c;
  };
  ALLOC_DATATYPE(trans, 3, MPI_LONG_DOUBLE_INT, 1);
  trans->list[0].dt = datatypetable[MPI_LONG];
  trans->list[1].dt = datatypetable[MPI_DOUBLE];
  trans->list[1].start = sizeof(long);
  trans->list[2].dt = datatypetable[MPI_INT];
  trans->list[1].start = trans->list[0].dt->size;
  trans->list[1].start += trans->list[1].dt->size;
  trans->size = sizeof(m_l_d_i);
  trans->realsize = sizeof(m_l_d_i);
  DEPTH_DATATYPE(trans);
  trans->list[0].dt->usage++;
  trans->list[1].dt->usage++;
  trans->list[2].dt->usage++;

  return defer_err;
}


/*.......................................................*/
/* finalize */

int MPI_Finalize()
{
  int first;
  map<MPI_Datatype, PNMPIMOD_Datatype_storage_p>::iterator i;

  PMPI_Finalize();
#ifdef DATATABLEDEBUG
  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("Present %i / %i with usage %i / count %i\n", i->second->mpi_dt,
               i->second->store_dt, i->second->usage, i->second->count);
    }
#else /* not debug */
  first = 1;
  if (check_level)
    {
      for (i = datatypetable.begin(); i != datatypetable.end(); i++)
        {
          if (i->second->native == 0)
            {
              if (first)
                printf("DATATYPE CHECK: Node %i had datatypes that have not "
                       "been cleaned up\n",
                       dt_rank);
              first = 0;

              if (i->second->homogeneous)
                printf("\tNode %i: %li / %li with usage %i / base type with "
                       "size %i\n",
                       dt_rank, (long)i->second->mpi_dt,
                       (long)i->second->store_dt, i->second->usage,
                       i->second->homogeneous);
              else
                printf("\tNode %i: %li / %li with usage %i / combined out of "
                       "%i base types\n",
                       dt_rank, (long)i->second->mpi_dt,
                       (long)i->second->store_dt, i->second->usage,
                       i->second->count);
            }
        }
      if (first)
        printf(
          "DATATYPE CHECK: all datatypes have been cleaned up on node %i\n",
          dt_rank);
    }
#endif

#ifndef AIX

  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
#ifdef DATATABLEDEBUG
      printf("Loop Erasing %i\n", i->second->mpi_dt);
#endif
      ERASE_DATATYPE((i->first), 0);
    }

  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second == NULL)
        datatypetable.erase(i->first);
    }

#ifdef DATATABLEDEBUG
  printf("After cleanup:\n");
  for (i = datatypetable.begin(); i != datatypetable.end(); i++)
    {
      if (i->second->homogeneous)
        printf("2nd Present %i / %i with usage %i / bottom with size %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->homogeneous);
      else
        printf("2nd Present %i / %i with usage %i / count %i\n",
               i->second->mpi_dt, i->second->store_dt, i->second->usage,
               i->second->count);
    }
#endif

#endif

  return MPI_SUCCESS;
}


/*.......................................................*/
/* commit a type */

int MPI_Type_commit(MPI_Datatype *dt)
{
  /* no need to do anything different here */
  int ret;
  ret = PMPI_Type_commit(dt);
  return ret;
}


/*.......................................................*/
/* free a type */

int MPI_Type_free(MPI_Datatype *dt)
{
  int ret;
  ERASE_DATATYPE((*dt), 1);
  ret = PMPI_Type_free(dt);
  return ret;
}


/*.......................................................*/
/* Create cont. type */

int MPI_Type_contiguous(int count, MPI_Datatype in_dt, MPI_Datatype *out_dt)
{
  int ret;
  PNMPIMOD_Datatype_storage_p translated_dt, new_dt;

  CHECK_DATATYPE(in_dt);
  translated_dt = datatypetable[in_dt];

  ret = PMPI_Type_contiguous(count, in_dt, out_dt);

#ifdef DT_DEBUG
  printf("CREATE DT: contig count=%i dt=%i --> %i\n", count, in_dt, *out_dt);
#endif

  if (translated_dt->homogeneous)
    {
      ALLOC_DATATYPEBASESIZE(new_dt, translated_dt->mpi_dt, *out_dt,
                             translated_dt->size, count, 0)
    }
  else
    {
      ALLOC_DATATYPE(new_dt, 1, *out_dt, 0);
      new_dt->size = translated_dt->size * count;
      new_dt->realsize = translated_dt->realsize * count;
      new_dt->list[0].block = count;
      new_dt->list[0].dt = translated_dt;
      translated_dt->usage++;
      FIX_SUBTYPES(new_dt);
      DEPTH_DATATYPE(new_dt);
    }
  datatypetable[*out_dt] = new_dt;

  return ret;
}


/*.......................................................*/
/* Create vector type */

int MPI_Type_vector(int count, int blocklen, int stride, MPI_Datatype in_dt,
                    MPI_Datatype *out_dt)
{
  int ret;
  PNMPIMOD_Datatype_storage_p translated_dt, new_dt;

  CHECK_DATATYPE(in_dt);
  translated_dt = datatypetable[in_dt];

  ret = PMPI_Type_vector(count, blocklen, stride, in_dt, out_dt);

#ifdef DT_DEBUG
  printf("CREATE DT: vector count=%i block=%i stride=%i dt=%i --> %i\n", count,
         blocklen, stride, in_dt, *out_dt);
#endif

  if ((translated_dt->homogeneous) && ((count == 1) || (blocklen == stride)))
    {
      ALLOC_DATATYPEBASESIZE(new_dt, translated_dt->mpi_dt, *out_dt,
                             translated_dt->size, count * blocklen, 0);
    }
  else
    {
      ALLOC_DATATYPE(new_dt, 1, *out_dt, 0);
      new_dt->size = translated_dt->size * (stride * (count - 1) + blocklen);
      new_dt->realsize = translated_dt->realsize * count * blocklen;
      new_dt->list[0].block = blocklen;
      new_dt->list[0].repeat = count;
      new_dt->list[0].step = stride * translated_dt->size;
      new_dt->list[0].dt = translated_dt;
      translated_dt->usage++;
      FIX_SUBTYPES(new_dt);
      DEPTH_DATATYPE(new_dt);
    }
  datatypetable[*out_dt] = new_dt;

  return ret;
}


/*.......................................................*/
/* Create hvector type */

int MPI_Type_hvector(int count, int blocklen, MPI_Aint stride,
                     MPI_Datatype in_dt, MPI_Datatype *out_dt)
{
  int ret;
  PNMPIMOD_Datatype_storage_p translated_dt, new_dt;

  CHECK_DATATYPE(in_dt);
  translated_dt = datatypetable[in_dt];

  ret = PMPI_Type_hvector(count, blocklen, stride, in_dt, out_dt);

#ifdef DT_DEBUG
  printf("CREATE DT: hvector count=%i block=%i stride=%i dt=%i --> %i\n", count,
         blocklen, stride, in_dt, *out_dt);
#endif

  if ((translated_dt->homogeneous) &&
      ((count == 1) || (blocklen * translated_dt->size == stride)))
    {
      ALLOC_DATATYPEBASESIZE(new_dt, translated_dt->mpi_dt, *out_dt,
                             translated_dt->size, count * blocklen, 0);
    }
  else
    {
      ALLOC_DATATYPE(new_dt, 1, *out_dt, 0);
      new_dt->size = translated_dt->size * blocklen + stride * (count - 1);
      new_dt->realsize = translated_dt->realsize * count * blocklen;
      new_dt->list[0].block = blocklen;
      new_dt->list[0].repeat = count;
      new_dt->list[0].step = stride;
      new_dt->list[0].dt = translated_dt;
      translated_dt->usage++;
      FIX_SUBTYPES(new_dt);
      DEPTH_DATATYPE(new_dt);
    }
  datatypetable[*out_dt] = new_dt;

  return ret;
}


/*.......................................................*/
/* Create index type */

int MPI_Type_indexed(int count, int *blocklens, int *indices,
                     MPI_Datatype in_dt, MPI_Datatype *out_dt)
{
  int ret, i, hom, sum, stride;
  PNMPIMOD_Datatype_storage_p translated_dt, new_dt;

  CHECK_DATATYPE(in_dt);
  translated_dt = datatypetable[in_dt];

  ret = PMPI_Type_indexed(count, blocklens, indices, in_dt, out_dt);

#ifdef DT_DEBUG
  {
    int __i;
    printf("CREATE DT: indexed count=%i blocks=", count);
    for (__i = 0; __i < count; __i++)
      printf(" %i", blocklens[__i]);
    printf(" index=");
    for (__i = 0; __i < count; __i++)
      printf(" %i", indices[__i]);
    printf(" dt=%i --> %i\n", in_dt, *out_dt);
  }
#endif

  hom = 1;
  sum = blocklens[0];
  for (i = 1; i < count; i++)
    {
      if (indices[i] != blocklens[i - 1])
        hom = 0;
      sum += blocklens[i];
    }

  if ((translated_dt->homogeneous) && (hom) && (indices[0] == 0))
    {
      ALLOC_DATATYPEBASESIZE(new_dt, translated_dt->mpi_dt, *out_dt,
                             translated_dt->size, sum, 0);
    }
  else
    {
      hom = 1;
      stride = -1;
      for (i = 1; i < count; i++)
        {
          if ((blocklens[i - 1] != blocklens[i]) ||
              ((stride >= 0) && (stride != indices[i] - indices[i - 1])))
            hom = 0;
          stride = indices[i] - indices[i - 1];
        }
      if (hom)
        {
          ALLOC_DATATYPE(new_dt, 1, *out_dt, 0);
          new_dt->size =
            translated_dt->size * (blocklens[0] + stride * (count - 1));
          new_dt->realsize = translated_dt->realsize * sum;
          new_dt->list[0].block = blocklens[0];
          new_dt->list[0].repeat = count;
          new_dt->list[0].step = stride * translated_dt->size;
          new_dt->list[0].start = indices[0] * translated_dt->size;
          new_dt->list[0].dt = translated_dt;
          translated_dt->usage++;
          FIX_SUBTYPES(new_dt);
          DEPTH_DATATYPE(new_dt);
        }
      else
        {
          ALLOC_DATATYPE(new_dt, count, *out_dt, 0);
          for (i = 0; i < count; i++)
            {
              new_dt->list[i].block = blocklens[i];
              new_dt->list[i].start = indices[i] * translated_dt->size;
              new_dt->list[i].dt = translated_dt;
            }
          new_dt->size = translated_dt->size * blocklens[count - 1] +
                         new_dt->list[count - 1].start - new_dt->list[0].start;
          new_dt->realsize = translated_dt->realsize * sum;
          translated_dt->usage += count;
          FIX_SUBTYPES(new_dt);
          DEPTH_DATATYPE(new_dt);
        }
    }
  datatypetable[*out_dt] = new_dt;

  return ret;
}


/*.......................................................*/
/* Create hindex type */

int MPI_Type_hindexed(int count, int *blocklens, MPI_Aint *indices,
                      MPI_Datatype in_dt, MPI_Datatype *out_dt)
{
  int ret, i, hom, sum, stride;
  PNMPIMOD_Datatype_storage_p translated_dt, new_dt;

  CHECK_DATATYPE(in_dt);
  translated_dt = datatypetable[in_dt];

  ret = PMPI_Type_hindexed(count, blocklens, indices, in_dt, out_dt);

#ifdef DT_DEBUG
  {
    int __i;
    printf("CREATE DT: hindexed count=%i blocks=", count);
    for (__i = 0; __i < count; __i++)
      printf(" %i", blocklens[__i]);
    printf(" index=");
    for (__i = 0; __i < count; __i++)
      printf(" %i", indices[__i]);
    printf(" dt=%i --> %i\n", in_dt, *out_dt);
  }
#endif

  hom = 1;
  sum = blocklens[0];
  for (i = 1; i < count; i++)
    {
      if (indices[i] != blocklens[i - 1] * translated_dt->size)
        hom = 0;
      sum += blocklens[i];
    }

  if ((translated_dt->homogeneous) && (hom) && (indices[0] == 0))
    {
      ALLOC_DATATYPEBASESIZE(new_dt, translated_dt->mpi_dt, *out_dt,
                             translated_dt->size, sum, 0);
    }
  else
    {
      hom = 1;
      stride = -1;
      for (i = 1; i < count; i++)
        {
          if ((blocklens[i - 1] != blocklens[i]) ||
              ((stride >= 0) && (stride != indices[i] - indices[i - 1])))
            hom = 0;
          stride = indices[i] - indices[i - 1];
        }
      if (hom)
        {
          ALLOC_DATATYPE(new_dt, 1, *out_dt, 0);
          new_dt->size =
            translated_dt->size * blocklens[0] + stride * (count - 1);
          new_dt->realsize = translated_dt->realsize * sum;
          new_dt->list[0].block = blocklens[0];
          new_dt->list[0].repeat = count;
          new_dt->list[0].step = stride;
          new_dt->list[0].start = indices[0];
          new_dt->list[0].dt = translated_dt;
          translated_dt->usage++;
          FIX_SUBTYPES(new_dt);
          DEPTH_DATATYPE(new_dt);
        }
      else
        {
          ALLOC_DATATYPE(new_dt, count, *out_dt, 0);
          for (i = 0; i < count; i++)
            {
              new_dt->list[i].block = blocklens[i];
              new_dt->list[i].start = indices[i];
              new_dt->list[i].dt = translated_dt;
            }
          new_dt->size = translated_dt->size * blocklens[count - 1] +
                         new_dt->list[count - 1].start - new_dt->list[0].start;
          new_dt->realsize = translated_dt->realsize * sum;
          translated_dt->usage += count;
          FIX_SUBTYPES(new_dt);
          DEPTH_DATATYPE(new_dt);
        }
    }
  datatypetable[*out_dt] = new_dt;

  return ret;
}


/*.......................................................*/
/* Create struct type */

int MPI_Type_struct(int count, int *blocklens, MPI_Aint *indices,
                    MPI_Datatype *in_dts, MPI_Datatype *out_dt)
{
  int ret, i, hom, sum, stride;
  PNMPIMOD_Datatype_storage_p translated_dt, new_dt;

  CHECK_DATATYPE(in_dts[0]);
  translated_dt = datatypetable[in_dts[0]];

  ret = PMPI_Type_struct(count, blocklens, indices, in_dts, out_dt);

#ifdef DT_DEBUG
  {
    int __i;
    printf("CREATE DT: struct count=%i blocks=", count);
    for (__i = 0; __i < count; __i++)
      printf(" %i", blocklens[__i]);
    printf(" index=");
    for (__i = 0; __i < count; __i++)
      printf(" %i", indices[__i]);
    printf(" dt=");
    for (__i = 0; __i < count; __i++)
      printf(" %i", in_dts[__i]);
    printf(" --> %i\n", *out_dt);
  }
#endif

  hom = 1;
  for (i = 1; i < count; i++)
    {
      if (in_dts[i] != in_dts[i - 1])
        hom = 0;
    }

  if (hom)
    {
      hom = 1;
      sum = blocklens[0];
      for (i = 1; i < count; i++)
        {
          if (indices[i] != blocklens[i - 1] * translated_dt->size)
            hom = 0;
          sum += blocklens[i];
        }

      if ((translated_dt->homogeneous) && (hom) && (indices[0] == 0))
        {
          ALLOC_DATATYPEBASESIZE(new_dt, translated_dt->mpi_dt, *out_dt,
                                 translated_dt->size, sum, 0);
        }
      else
        {
          hom = 1;
          stride = -1;
          for (i = 1; i < count; i++)
            {
              if ((blocklens[i - 1] != blocklens[i]) ||
                  ((stride >= 0) && (stride != indices[i] - indices[i - 1])))
                hom = 0;
              stride = indices[i] - indices[i - 1];
            }
          if (hom)
            {
              ALLOC_DATATYPE(new_dt, 1, *out_dt, 0);
              new_dt->size =
                translated_dt->size * blocklens[0] + stride * (count - 1);
              new_dt->realsize = translated_dt->realsize * sum;
              new_dt->list[0].block = blocklens[0];
              new_dt->list[0].repeat = count;
              new_dt->list[0].step = stride;
              new_dt->list[0].start = indices[0];
              new_dt->list[0].dt = translated_dt;
              translated_dt->usage++;
              FIX_SUBTYPES(new_dt);
              DEPTH_DATATYPE(new_dt);
            }
          else
            {
              ALLOC_DATATYPE(new_dt, count, *out_dt, 0);
              for (i = 0; i < count; i++)
                {
                  new_dt->list[i].block = blocklens[i];
                  new_dt->list[i].start = indices[i];
                  new_dt->list[i].dt = translated_dt;
                }
              new_dt->size = translated_dt->size * blocklens[count - 1] +
                             new_dt->list[count - 1].start -
                             new_dt->list[0].start;
              new_dt->realsize = translated_dt->realsize * sum;
              translated_dt->usage += count;
              FIX_SUBTYPES(new_dt);
              DEPTH_DATATYPE(new_dt);
            }
        }
    }
  else
    {
      stride = 0;
      ALLOC_DATATYPE(new_dt, count, *out_dt, 0);
      new_dt->realsize = 0;
      for (i = 0; i < count; i++)
        {
          CHECK_DATATYPE(in_dts[i]);
          translated_dt = datatypetable[in_dts[i]];
          new_dt->realsize += translated_dt->realsize * blocklens[i];
          new_dt->list[i].block = blocklens[i];
          new_dt->list[i].start = indices[i];
          new_dt->list[i].dt = translated_dt;
          translated_dt->usage++;
        }
      new_dt->size = translated_dt->size * blocklens[count - 1] +
                     new_dt->list[count - 1].start - new_dt->list[0].start;
      FIX_SUBTYPES(new_dt);
      DEPTH_DATATYPE(new_dt);
    }

  datatypetable[*out_dt] = new_dt;

  return ret;
}


/*.......................................................*/
/* Create distr. array type */

int MPI_Type_create_darray(int size, int rank, int ndim, int *array_of_gsizes,
                           int *array_of_distribs, int *array_of_dargs,
                           int *array_of_psizes, int order, MPI_Datatype in_dt,
                           MPI_Datatype *out_dt)
{
  int ret;

  fprintf(stderr, "WARNING: MPI_Type_create_darray not supported !!!\n");

  ret = PMPI_Type_create_darray(size, rank, ndim, array_of_gsizes,
                                array_of_distribs, array_of_dargs,
                                array_of_psizes, order, in_dt, out_dt);
  return ret;
}


/*.......................................................*/
/* Create subarray type */

int MPI_Type_create_subarray(int ndims, int *array_of_sizes,
                             int *array_of_subsizes, int *array_of_starts,
                             int order, MPI_Datatype in_dt,
                             MPI_Datatype *out_dt)
{
  int ret;

  fprintf(stderr, "WARNING: MPI_Type_create_subarray not supported !!!\n");

  ret = PMPI_Type_create_subarray(ndims, array_of_sizes, array_of_subsizes,
                                  array_of_starts, order, in_dt, out_dt);
  return ret;
}


/*------------------------------------------------------------*/
/* The End. */
