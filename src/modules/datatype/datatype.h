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

#ifndef _PNMPI_MOD_DATATYPE
#define _PNMPI_MOD_DATATYPE


#include <mpi.h>


/*------------------------------------------------------------*/
/* Note: this module must be loaded into the stack close
   to the module(s) using it (i.e., no other module changing
   datatypes can be instantiated between this module and
   any module using it. Also, any call to its services must
   occur after calling PMPI_Init */

/* #define DATATABLEDEBUG */
/* #define USE_FUNCTIONS */
/* #define INCLUDE_WRAPPERS */

/*------------------------------------------------------------*/
/* track datatype layout */

#define PNMPI_MODULE_DATATYPE "datatype-tracking"


/*..........................................................*/
/* Transparent type to hold reference to a datatype */

typedef struct PNMPIMOD_Datatype_storage_d *PNMPIMOD_Datatype_storage_p;

typedef struct PNMPIMOD_Datatype_stack_d
{
  int i, j, b;
  char *buf, *startbuf, *blockbuf;
  PNMPIMOD_Datatype_storage_p dt;
} PNMPIMOD_Datatype_stack_t;

typedef struct PNMPIMOD_Datatype_Parameters_d
{
  int level;
  PNMPIMOD_Datatype_storage_p top;
  PNMPIMOD_Datatype_stack_t *stack;
} PNMPIMOD_Datatype_Parameters_t;

typedef struct PNMPIMOD_Datatype_entry_d
{
  MPI_Aint start;
  int block, repeat, step;
  PNMPIMOD_Datatype_storage_p dt;
} PNMPIMOD_Datatype_entry_t;

typedef struct PNMPIMOD_Datatype_storage_d
{
  int count;
  int depth;
  int size;
  int realsize;
  int homogeneous;
  int usage;
  int native;
  MPI_Datatype mpi_dt, store_dt;
  PNMPIMOD_Datatype_entry_t *list;
} PNMPIMOD_Datatype_storage_t;


/*..........................................................*/
/* get a reference to a datatype to loop over */

typedef int (*PNMPIMOD_Datatype_getReference_t)(
  const void *, int, MPI_Datatype, PNMPIMOD_Datatype_Parameters_t *);
int PNMPIMOD_Datatype_getReference(const void *buf, int count, MPI_Datatype dt,
                                   PNMPIMOD_Datatype_Parameters_t *ref);

typedef int (*PNMPIMOD_Datatype_delReference_t)(
  PNMPIMOD_Datatype_Parameters_t *);
int PNMPIMOD_Datatype_delReference(PNMPIMOD_Datatype_Parameters_t *ref);

typedef int (*PNMPIMOD_Datatype_getSize_t)(MPI_Datatype, int *);
int PNMPIMOD_Datatype_getSize(MPI_Datatype, int *);

/*------------------------------------------------------------*/
/* External service routines */

/*.......................................................*/
/* next item */

#ifndef USE_FUNCTIONS

#define PNMPIMOD_Datatype_nextItem_wrap(_ref, _done)              \
  {                                                               \
    int cont;                                                     \
    do                                                            \
      {                                                           \
        cont = 0;                                                 \
        _ref->stack[_ref->level].b++;                             \
        if ((_ref->stack[_ref->level].dt->list == NULL) ||        \
            (_ref->stack[_ref->level].b ==                        \
             _ref->stack[_ref->level]                             \
               .dt->list[_ref->stack[_ref->level].i]              \
               .block))                                           \
          {                                                       \
            _ref->stack[_ref->level].b = 0;                       \
            _ref->stack[_ref->level].j++;                         \
            if ((_ref->stack[_ref->level].dt->list == NULL) ||    \
                (_ref->stack[_ref->level].j ==                    \
                 _ref->stack[_ref->level]                         \
                   .dt->list[_ref->stack[_ref->level].i]          \
                   .repeat))                                      \
              {                                                   \
                _ref->stack[_ref->level].j = 0;                   \
                _ref->stack[_ref->level].i++;                     \
                if ((_ref->stack[_ref->level].i ==                \
                     _ref->stack[_ref->level].dt->count) ||       \
                    (_ref->stack[_ref->level].dt->homogeneous))   \
                  {                                               \
                    _ref->level--;                                \
                    cont = 1;                                     \
                  }                                               \
                else                                              \
                  {                                               \
                    _ref->stack[_ref->level].blockbuf =           \
                      _ref->stack[_ref->level].startbuf +         \
                      _ref->stack[_ref->level]                    \
                        .dt->list[_ref->stack[_ref->level].i]     \
                        .start;                                   \
                    _ref->stack[_ref->level].buf =                \
                      _ref->stack[_ref->level].blockbuf;          \
                  }                                               \
              }                                                   \
            else                                                  \
              {                                                   \
                _ref->stack[_ref->level].blockbuf +=              \
                  _ref->stack[_ref->level]                        \
                    .dt->list[_ref->stack[_ref->level].i]         \
                    .step;                                        \
                _ref->stack[_ref->level].buf =                    \
                  _ref->stack[_ref->level].blockbuf;              \
              }                                                   \
          }                                                       \
        else                                                      \
          {                                                       \
            if (_ref->stack[_ref->level].b != 0)                  \
              _ref->stack[_ref->level].buf +=                     \
                _ref->stack[_ref->level]                          \
                  .dt->list[_ref->stack[_ref->level].i]           \
                  .dt->size;                                      \
          }                                                       \
                                                                  \
        if (cont == 0)                                            \
          {                                                       \
            while (_ref->stack[_ref->level].dt->homogeneous == 0) \
              {                                                   \
                _ref->level++;                                    \
                _ref->stack[_ref->level].i = 0;                   \
                _ref->stack[_ref->level].j = 0;                   \
                _ref->stack[_ref->level].b = 0;                   \
                _ref->stack[_ref->level].dt =                     \
                  _ref->stack[_ref->level - 1]                    \
                    .dt->list[_ref->stack[_ref->level - 1].i]     \
                    .dt;                                          \
                _ref->stack[_ref->level].startbuf =               \
                  _ref->stack[_ref->level - 1].buf;               \
                _ref->stack[_ref->level].blockbuf =               \
                  _ref->stack[_ref->level].startbuf;              \
                if (_ref->stack[_ref->level].dt->list)            \
                  _ref->stack[_ref->level].blockbuf +=            \
                    _ref->stack[_ref->level]                      \
                      .dt->list[_ref->stack[_ref->level].i]       \
                      .start;                                     \
                _ref->stack[_ref->level].buf =                    \
                  _ref->stack[_ref->level].blockbuf;              \
              }                                                   \
          }                                                       \
      }                                                           \
    while ((cont) && (_ref->level >= 0));                         \
                                                                  \
    if (_ref->level >= 0)                                         \
      *_done = 0;                                                 \
    else                                                          \
      *_done = 1;                                                 \
  }

#define PNMPIMOD_Datatype_nextItem(ref, done) \
  PNMPIMOD_Datatype_nextItem_wrap((ref), (done))

#endif /* not USE_FUNCTIONS */

/*.......................................................*/
/* get item */

#ifndef USE_FUNCTIONS

#define PNMPIMOD_Datatype_getItem_wrap(_ref, _buf, _typ, _len, _size, _last) \
  {                                                                          \
    *_buf = _ref->stack[_ref->level].buf;                                    \
    *_typ = _ref->stack[_ref->level].dt->mpi_dt;                             \
    *_len = _ref->stack[_ref->level].dt->count;                              \
    *_size = _ref->stack[_ref->level].dt->homogeneous;                       \
    PNMPIMOD_Datatype_nextItem(_ref, _last)                                  \
  }

#define PNMPIMOD_Datatype_getItem(ref, buf, typ, len, size, last) \
  PNMPIMOD_Datatype_getItem_wrap((ref), (buf), (typ), (len), (size), (last))

#endif /* not USE_FUNCTIONS */


#endif /* _PNMPI_MOD_DATATYPE */
