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

/* Private Interface for PNMPI */

#define NUM_MPI_CALLS 300

#define MODULE_SKIP 20
#define MAX_ARG_NAME 30
#define MAX_ARG_VALUE 1024
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
  char name[PNMPI_MODULE_FILENAMELEN];
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

extern int pnmpi_level;
extern int pnmpi_max_level;

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
    mods.module[__i]->statscount.fn = 0;
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
    mods.module[__i]->statstiming.fn = 0;
#define TOTAL_STATS_6(fn)  \
  if (DBGCHECK(DBGLEVEL6)) \
    pnmpi_totalstats_timing.fn = 0;
#else
#define MODULE_STATS_6(mods, fn)
#define TOTAL_STATS_6(fn)
#endif

// actual stack initialization macro
#define INITIALIZE_FUNCTION_STACK(routine, routine_id, r_type, stack, mods, \
                                  mpiroutine)                               \
  {                                                                         \
    int __i;                                                                \
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
    for (__i = 0; __i < mods.num; __i++)                                    \
      {                                                                     \
        if (mods.module[__i]->stack_delimiter)                              \
          continue;                                                         \
        pnmpi_function_ptrs.stack[__i] =                                    \
          (r_type)mydlsym(mods.module[__i]->handle, routine);               \
        if (pnmpi_function_ptrs.stack[__i] != NULL)                         \
          {                                                                 \
            SET_ACTIVATED(routine_id);                                      \
          }                                                                 \
        DBGPRINT2("Symbol for routine %s in module %s: value %px", routine, \
                  mods.module[__i]->name, pnmpi_function_ptrs.stack[__i]);  \
        MODULE_STATS_5(mods, mpiroutine);                                   \
        MODULE_STATS_6(mods, mpiroutine);                                   \
      }                                                                     \
    TOTAL_STATS_5(mpiroutine);                                              \
    TOTAL_STATS_6(mpiroutine);                                              \
  }
