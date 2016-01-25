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

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PNMPI_IMPLEMENTATION

#include "pnmpi.h"
#include "pnmpimod.h"
#include <mpi.h>

#include "debug.h"
#include "wrapper.h"

/* jfm Modification (ELP AP THREAD SAFETY) BEGIN */
#ifdef PNMPI_ENABLE_THREAD_SAFETY
#include <pthread.h>
pthread_mutex_t pnmpi_level_lock;
pthread_key_t pnmpi_level_key;
#endif
/* jfm Modification (ELP AP THREAD SAFETY) END */

/* Private Interface for PNMPI */

#define NUM_MPI_CALLS 300

#define MODULE_SKIP 20
#define MAX_ARG_NAME 30
#define MAX_ARG_VALUE 2048
#define MAX_LINE MAX_ARG_VALUE + MAX_ARG_NAME
/*Was once: (2*PNMPI_MODULE_FILENAMELEN), needs to be long for some projects
 * ...*/

#define MAX_CMDARGS 10


#define PNMPI_PCONTROL_SINGLEorMULTIPLE 0x1
#define PNMPI_PCONTROL_VARGorPTR 0x2
#define PNMPI_PCONTROL_SINGLEorMULTIPLEERROR 0x4

#define CONFNAME ".pnmpi-conf"


typedef char module_name_t[PNMPI_MODULE_FILENAMELEN];

typedef struct module_servlist_d *module_servlist_p;
typedef struct module_servlist_d
{
  PNMPI_Service_descriptor_t desc;
  module_servlist_p next;
} module_servlist_t;

typedef struct module_globlist_d *module_globlist_p;
typedef struct module_globlist_d
{
  PNMPI_Global_descriptor_t desc;
  module_globlist_p next;
} module_globlist_t;

typedef struct module_arg_d *module_arg_p;
typedef struct module_arg_d
{
  char name[MAX_ARG_NAME];
  char value[MAX_ARG_VALUE];
  module_arg_p next;
} module_arg_t;

typedef struct module_def_d *module_def_p;
typedef struct module_def_d
{
  module_name_t name;
  module_name_t path;
  char username[PNMPI_MODULE_USERNAMELEN];
  int registered;
  void *handle;
  module_arg_p args;
  int pcontrol;
  module_servlist_p services;
  module_globlist_p globals;
  int stack_delimiter;
#ifdef DBGLEVEL5
  pnmpi_functions_statscount_t statscount;
#endif
#ifdef DBGLEVEL6
  pnmpi_functions_statstiming_t statstiming;
#endif
} module_def_t;

typedef struct modules_d
{
  int num;
  int numalloc;
  int pcontrol;
  int pcontrol_typed_level;
  int pcontrol_typed_type;
  module_def_p *module;
} modules_t;

extern modules_t modules;

// extern int pnmpi_level;
extern int pnmpi_mpi_level; /**< Is used to control recursive wrapping of MPI
                               calls, right now we only use it for MPI_X calls
                               that are triggered be MPI_Finalize (as these
                               really hurt as we shut down our tools already in
                               that case).*/
extern int pnmpi_max_level;

/* jfm Modification (ELP AP THREAD SAFETY) BEGIN */
#ifdef PNMPI_ENABLE_THREAD_SAFETY
extern pthread_mutex_t pnmpi_level_lock;
#endif /*PNMPI_ENABLE_THREAD_SAFETY*/
/* jfm Modification (ELP AP THREAD SAFETY) END */

void pnmpi_PreInit(void);

extern int iargc_(void);
extern char *getarg_(int *, char *, int);

// bit vector implementation for determining which functions are activated
#define NUM_MPI_CELLS (NUM_MPI_CALLS / sizeof(pnmpi_cell_t) + 1)
typedef uintptr_t pnmpi_cell_t;
extern pnmpi_cell_t pnmpi_activated[NUM_MPI_CELLS];
extern pnmpi_functions_t pnmpi_function_ptrs;

#define CELL_INDEX(id) (id / (sizeof(pnmpi_cell_t) * 8))
#define CELL_BMASK(id) (((pnmpi_cell_t)1) << (id % (sizeof(pnmpi_cell_t) * 8)))
#define IS_ACTIVATED(id) (pnmpi_activated[CELL_INDEX(id)] & CELL_BMASK(id))
#define SET_ACTIVATED(id) pnmpi_activated[CELL_INDEX(id)] |= CELL_BMASK(id)
#define NOT_ACTIVATED(id) (!IS_ACTIVATED(id))

// Statistics collection for stack initialization
// TODO: Does all the initialization really need to live in macros?  Does it
// need to happen
// TODO: that fast?  It's only done once.
#ifdef DBGLEVEL5
#define MODULE_STATS_5(mods, fn) \
  if (DBGCHECK(DBGLEVEL5))       \
    mods.module[i]->statscount.fn = 0;
#define TOTAL_STATS_5(fn)  \
  if (DBGCHECK(DBGLEVEL5)) \
    pnmpi_totalstats_count.fn = 0;
#else
#define MODULE_STATS_5(mods, fn)
#define TOTAL_STATS_5(fn)
#endif

#ifdef DBGLEVEL6
#define MODULE_STATS_6(mods, fn) \
  if (DBGCHECK(DBGLEVEL6))       \
    mods.module[i]->statstiming.fn = 0;
#define TOTAL_STATS_6(fn)  \
  if (DBGCHECK(DBGLEVEL6)) \
    pnmpi_totalstats_timing.fn = 0;
#else
#define MODULE_STATS_6(mods, fn)
#define TOTAL_STATS_6(fn)
#endif

/*This can be removed at a later point in time, it allows convenient bugfixing
 * for a broken mpi.h of MVAPICH1 (Sierra at LLNL, April 2014, by Tobias)*/
#ifdef PNMPI_MVAPICH1FIX
int PMPI_Type_create_indexed_block(int, int, int *, MPI_Datatype,
                                   MPI_Datatype *);
int PMPI_Finalized(int *);
#endif /*PNMPI_MVAPICH1FIX*/

// Helper to check for RTLDNEXT Availability
/**
 * Rational on RTLDNEXT usage:
 * If a module has a dependency to the MPI libs (which makes sense as it uses
 * MPI symbols),
 * a call to dlsym on this module will either return the modules own MPI wrapper
 * for the call (if present) or
 * the MPI call implementation in the MPI library. In our
 * INITIALIZE_FUNCTION_STACK macro however,
 * we are only interested in MPI calls within the module (i.e. wrappers), not
 * any symbols of the original
 * MPI library (i.e. implementations). With that, modules must not have MPI
 * dependencies, which sometimes makes
 * your live a bit harder. So we provide a mechanism that uses RTLD_NEXT to
 * check what
 * the symbol of a respective call is in the MPI lib, with that we can check
 * whether a dlsym
 * call on a module returned a wrapper function or just a function in the MPI
 * lib, if the later
 * happens we do not mark the module as having a wrapper for this function.
 *
 * Note: RTLD_NEXT is unfortunately not available everywhere, thats why we use
 * the templates below.
 */
#ifdef RTLD_NEXT
#define RTLDNEXT_RETRIEVAL(r_type, routine) \
  r_type __tmp_function_ptr = (r_type)mydlsym(RTLD_NEXT, routine);
#define RTLDNEXT_CHECK(stack) pnmpi_function_ptrs.stack[i] != __tmp_function_ptr
#else
#define RTLDNEXT_RETRIEVAL(r_type, routine)
#define RTLDNEXT_CHECK(stack) 1
#endif

// actual stack initialization macro
#define INITIALIZE_FUNCTION_STACK(routine, routine_id, r_type, stack, mods, \
                                  mpiroutine)                               \
  {                                                                         \
    int i;                                                                  \
    DBGPRINT2("Initialize stack for %s\n", routine);                        \
    if (pnmpi_function_ptrs.stack == NULL)                                  \
      {                                                                     \
        pnmpi_function_ptrs.stack =                                         \
          (r_type *)malloc(mods.num * sizeof(r_type));                      \
      }                                                                     \
    if (pnmpi_function_ptrs.stack == NULL)                                  \
      {                                                                     \
        WARNPRINT("Can't allocate stack for (%i) - exiting", routine_id);   \
        exit(1);                                                            \
      }                                                                     \
    for (i = 0; i < mods.num; i++)                                          \
      {                                                                     \
        if (mods.module[i]->stack_delimiter)                                \
          continue;                                                         \
        pnmpi_function_ptrs.stack[i] =                                      \
          (r_type)find_symbol(mods.module[i], routine);                     \
        RTLDNEXT_RETRIEVAL(r_type, routine)                                 \
        if (pnmpi_function_ptrs.stack[i] != NULL &&                         \
            pnmpi_function_ptrs.stack[i] != (r_type)P##mpiroutine &&        \
            RTLDNEXT_CHECK(stack))                                          \
          {                                                                 \
            SET_ACTIVATED(routine_id);                                      \
          }                                                                 \
        else                                                                \
          {                                                                 \
            pnmpi_function_ptrs.stack[i] =                                  \
              NULL; /*needed to make RTLD_NEXT check work*/                 \
          }                                                                 \
        DBGPRINT2("Symbol for routine %s in module %s: value %px", routine, \
                  mods.module[i]->name, pnmpi_function_ptrs.stack[i]);      \
        MODULE_STATS_5(mods, mpiroutine);                                   \
        MODULE_STATS_6(mods, mpiroutine);                                   \
      }                                                                     \
    TOTAL_STATS_5(mpiroutine);                                              \
    TOTAL_STATS_6(mpiroutine);                                              \
  }

/* jfm Modification (ELP AP THREAD SAFETY) BEGIN */
#ifdef PNMPI_ENABLE_THREAD_SAFETY
static inline int set_pnmpi_level(int value)
{
  pthread_setspecific(pnmpi_level_key, (void *)(long)value);
  return value;
}

// pnmpi_level++
static inline int inc_pnmpi_level()
{
  int value = (int)(unsigned long)pthread_getspecific(pnmpi_level_key) + 1;
  pthread_setspecific(pnmpi_level_key, (void *)(long)value);
  return value;
}

// pnmpi_level--
static inline int dec_pnmpi_level()
{
  int value = (int)(unsigned long)pthread_getspecific(pnmpi_level_key) - 1;
  pthread_setspecific(pnmpi_level_key, (void *)(long)value);
  return value;
}

static inline int get_pnmpi_level()
{
  return (int)((unsigned long)pthread_getspecific(pnmpi_level_key));
}

static inline int get_pnmpi_mpi_level()
{
  int value;
  pthread_mutex_lock(&pnmpi_level_lock);
  value = pnmpi_mpi_level;
  pthread_mutex_unlock(&pnmpi_level_lock);
  return value;
}

static inline void inc_pnmpi_mpi_level()
{
  pthread_mutex_lock(&pnmpi_level_lock);
  pnmpi_mpi_level++;
  pthread_mutex_unlock(&pnmpi_level_lock);
}

static inline void dec_pnmpi_mpi_level()
{
  pthread_mutex_lock(&pnmpi_level_lock);
  pnmpi_mpi_level--;
  pthread_mutex_unlock(&pnmpi_level_lock);
}

#else  /*PNMPI_ENABLE_THREAD_SAFETY*/
// build w/o thread safety enabled
int __pnmpi_level;
static inline int set_pnmpi_level(int value)
{
  __pnmpi_level = value;
  return value;
}

static inline void inc_pnmpi_mpi_level()
{
  pnmpi_mpi_level++;
}

static inline void dec_pnmpi_mpi_level()
{
  pnmpi_mpi_level--;
}

static inline int get_pnmpi_level()
{
  return __pnmpi_level;
}

static inline int inc_pnmpi_level()
{
  return ++__pnmpi_level;
}

static inline int dec_pnmpi_level()
{
  return --__pnmpi_level;
}

static inline int get_pnmpi_mpi_level()
{
  return pnmpi_mpi_level;
}
#endif /*PNMPI_ENABLE_THREAD_SAFETY*/
/* jfm Modification (ELP AP THREAD SAFETY) END */
