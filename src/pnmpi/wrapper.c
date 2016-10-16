/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

/* Special wrappers not covered by the generator */
#ifdef EXPERIMENTAL_UNWIND
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif
#include <stdarg.h>

#include <mpi.h>

#include "app_hooks.h"
#include "core.h"
#include "fallback_init.h"
#include "pnmpi-config.h"


/*-------------------------------------------------------------------*/
/* timing and count variables for DBGLEVEL 5 and 6 */

#ifdef DBGLEVEL5
pnmpi_functions_statscount_t pnmpi_totalstats_count;
#endif
#ifdef DBGLEVEL6
pnmpi_functions_statstiming_t pnmpi_totalstats_timing;
timing_t pnmpi_overall_timing;
#endif


extern void *MPIR_ToPointer(int idx);

#ifdef COMPILE_FOR_FORTRAN
void pmpi_init_(int *ierror);
#ifdef HAVE_MPI_INIT_THREAD_Fortran
void pmpi_init_thread_(int *ierror, int *required, int *provided);
#endif /*HAVE_MPI_INIT_THREAD_Fortran*/
#endif


/*-------------------------------------------------------------------*/
/* Statistics output for Debuglevel 5 */

#ifdef DBGLEVEL5 /* additional timing statistics */

#define DBGPRINT5_COUNTS_FCT_ALL(_buf, _num, _div, _out, _routine, _name) \
  {                                                                       \
    int _sum, _i;                                                         \
    _sum = 0;                                                             \
    for (_i = 0; _i < _num; _i++)                                         \
      _sum += (_buf[_i])._routine;                                        \
    if (_sum > 0)                                                         \
      {                                                                   \
        fprintf(_out, "%s\t%li", _name, ((_buf[_num])._routine) / _div);  \
        for (_i = 0; _i < _num; _i++)                                     \
          fprintf(_out, "\t%li", ((_buf[_i])._routine) / _div);           \
        fprintf(_out, "\n");                                              \
      }                                                                   \
  }

#define DBGPRINT5_TIMING_FCT_ALL(_buf, _num, _div, _out, _routine, _name) \
  {                                                                       \
    timing_t _sum;                                                        \
    int _i;                                                               \
    _sum = 0;                                                             \
    for (_i = 0; _i < _num; _i++)                                         \
      _sum += (_buf[_i])._routine;                                        \
    if (sum > 0)                                                          \
      {                                                                   \
        fprintf(_out, "%s\t%f", _name, (_buf[_num])._routine / _div);     \
        for (_i = 0; _i < _num; _i++)                                     \
          fprintf(_out, "\t%f", (_buf[_i])._routine / _div);              \
        fprintf(_out, "\n");                                              \
      }                                                                   \
  }

void pnmpi_int_print_countstats()
{
  if (DBGCHECK(DBGLEVEL5))
    {
      FILE *out;
      int num, rank, size, i, err;
      pnmpi_functions_statscount_t *collect_max, *collect_min, *collect_sum;


      err = PMPI_Barrier(MPI_COMM_WORLD);
      if (err != MPI_SUCCESS)
        {
          DBGPRINT5("MPI Operation failed while writing statistics");
          return;
        }
      err = PMPI_Comm_rank(MPI_COMM_WORLD, &rank);
      if (err != MPI_SUCCESS)
        {
          DBGPRINT5("MPI Operation failed while writing statistics");
          return;
        }
      err = PMPI_Comm_size(MPI_COMM_WORLD, &size);
      if (err != MPI_SUCCESS)
        {
          DBGPRINT5("MPI Operation failed while writing statistics");
          return;
        }

      num = sizeof(pnmpi_functions_statscount_t) / sizeof(long);

      if (rank == 0)
        {
          DBGPRINT5("Writing statistics for counts");
          out = fopen("pnmpi-stats-count.txt", "w");

          collect_min = (pnmpi_functions_statscount_t *)malloc(
            sizeof(pnmpi_functions_statscount_t) * (modules.num + 1));
          if (collect_min == NULL)
            {
              DBGPRINT5("Memory allocation failed while writing statistics");
              return;
            }
          collect_max = (pnmpi_functions_statscount_t *)malloc(
            sizeof(pnmpi_functions_statscount_t) * (modules.num + 1));
          if (collect_min == NULL)
            {
              DBGPRINT5("Memory allocation failed while writing statistics");
              return;
            }
          collect_sum = (pnmpi_functions_statscount_t *)malloc(
            sizeof(pnmpi_functions_statscount_t) * (modules.num + 1));
          if (collect_min == NULL)
            {
              DBGPRINT5("Memory allocation failed while writing statistics");
              return;
            }
        }

      for (i = 0; i < modules.num; i++)
        {
          err = PMPI_Reduce(&(modules.module[i]->statscount), &(collect_min[i]),
                            num, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
          if (err != MPI_SUCCESS)
            {
              DBGPRINT5("MPI Operation failed while writing statistics");
              return;
            }
          err = PMPI_Reduce(&(modules.module[i]->statscount), &(collect_max[i]),
                            num, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
          if (err != MPI_SUCCESS)
            {
              DBGPRINT5("MPI Operation failed while writing statistics");
              return;
            }
          err = PMPI_Reduce(&(modules.module[i]->statscount), &(collect_sum[i]),
                            num, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
          if (err != MPI_SUCCESS)
            {
              DBGPRINT5("MPI Operation failed while writing statistics");
              return;
            }
        }

      err = PMPI_Reduce(&(pnmpi_totalstats_count), &(collect_min[modules.num]),
                        num, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
      if (err != MPI_SUCCESS)
        {
          DBGPRINT5("MPI Operation failed while writing statistics");
          return;
        }
      err = PMPI_Reduce(&(pnmpi_totalstats_count), &(collect_max[modules.num]),
                        num, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
      if (err != MPI_SUCCESS)
        {
          DBGPRINT5("MPI Operation failed while writing statistics");
          return;
        }
      err = PMPI_Reduce(&(pnmpi_totalstats_count), &(collect_sum[modules.num]),
                        num, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
      if (err != MPI_SUCCESS)
        {
          DBGPRINT5("MPI Operation failed while writing statistics");
          return;
        }

      if (rank == 0)
        {
          fprintf(out, "AVG\tPnMPI");
          for (i = 0; i < modules.num; i++)
            {
              fprintf(out, "\t%s", modules.module[i]->name);
            }
          fprintf(out, "\n");
          DBGPRINT5_COUNTS_ALL_ALL(collect_sum, modules.num, size, out);
          fprintf(out, "\n");
        }

      if (rank == 0)
        {
          fprintf(out, "MAX\tPnMPI");
          for (i = 0; i < modules.num; i++)
            {
              fprintf(out, "\t%s", modules.module[i]->name);
            }
          fprintf(out, "\n");
          DBGPRINT5_COUNTS_ALL_ALL(collect_max, modules.num, 1, out);
          fprintf(out, "\n");
        }

      if (rank == 0)
        {
          fprintf(out, "MIN\tPnMPI");
          for (i = 0; i < modules.num; i++)
            {
              fprintf(out, "\t%s", modules.module[i]->name);
            }
          fprintf(out, "\n");
          DBGPRINT5_COUNTS_ALL_ALL(collect_min, modules.num, 1, out);
          fprintf(out, "\n");
        }

      if (rank == 0)
        {
          fprintf(out, "SUM\tPnMPI");
          for (i = 0; i < modules.num; i++)
            {
              fprintf(out, "\t%s", modules.module[i]->name);
            }
          fprintf(out, "\n");
          DBGPRINT5_COUNTS_ALL_ALL(collect_sum, modules.num, 1, out);

          fclose(out);
        }

      PMPI_Barrier(MPI_COMM_WORLD);
    }
}

#endif

/*-------------------------------------------------------------------*/
/* Statistics output for Debuglevel 5 */

#ifdef DBGLEVEL6 /* additional timing statistics */

void pnmpi_int_print_timingstats()
{
  if (DBGCHECK(DBGLEVEL6))
    {
    }
}

#endif


/*-------------------------------------------------------------------*/
/* MPI_Init */

static int PNMPI_Common_MPI_Init(int *_pnmpi_arg_0, char ***_pnmpi_arg_1)
{
#ifndef __GNUC__
  /* If the compiler does not support GCC's constructors, check if the fallback
   * constructor was called before this function. */
  pnmpi_fallback_init();
#endif


  int returnVal;

  inc_pnmpi_mpi_level();
  if (NOT_ACTIVATED(MPI_Init_ID))
    {
      /* If app_startup is activated, MPI was initialized before in _init. A
       * second call of MPI_Init is not allowed. */
      if (pnmpi_init_done)
        {
          returnVal = MPI_SUCCESS;
        }
      else
        {
#ifdef COMPILE_FOR_FORTRAN
          if (pnmpi_init_was_fortran)
            pmpi_init_(&returnVal);
          else
#endif
            returnVal = PMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);

          pnmpi_init_done = 1;
        }
    }
  else
    returnVal = Internal_XMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);

  if (returnVal != MPI_SUCCESS)
    return returnVal;

  // Mpi is initialized now
  pnmpi_initialization_complete = 1;

  PRINTINIT();
  DBGLATEINIT();
  STATUSINIT();
  DBGPRINT1("Leaving Init");
  dec_pnmpi_mpi_level();

  if (getenv("PNMPI_BE_SILENT") == NULL)
    {
      STATUSPRINT1("");
      STATUSPRINT1("\t\t  ---------------------------");
      STATUSPRINT1("\t\t | P^N-MPI Interface         |");
      STATUSPRINT1("\t\t | Martin Schulz, 2005, LLNL |");
      STATUSPRINT1("\t\t  ---------------------------");
      STATUSPRINT1("");

      {
        int i;
        module_servlist_p serv;
        module_globlist_p glob;
        module_arg_t *args;

        STATUSPRINT1("Number of modules: %i", modules.num);
        STATUSPRINT1("Pcontrol Setting:  %i", modules.pcontrol);
        STATUSPRINT1("");
        for (i = 0; i < modules.num; i++)
          {
            if (modules.module[i]->registered)
              {
                STATUSPRINT1("Module %s: registered as %s (Pctrl %i)",
                             modules.module[i]->name,
                             modules.module[i]->username,
                             modules.module[i]->pcontrol);
              }
            else
              {
                if (modules.module[i]->stack_delimiter)
                  {
                    STATUSPRINT1("Stack %s: not registered",
                                 modules.module[i]->name);
                  }
                else
                  {
                    STATUSPRINT1("Module %s: not registered (Pctrl %i)",
                                 modules.module[i]->name,
                                 modules.module[i]->pcontrol);
                  }
              }

            for (args = modules.module[i]->args; args != NULL;
                 args = args->next)
              {
                STATUSPRINT1("\tArgument: %s = %s", args->name, args->value);
              }

            for (serv = modules.module[i]->services; serv != NULL;
                 serv = serv->next)
              {
                STATUSPRINT1("\tService: %s (%s)", serv->desc.name,
                             serv->desc.sig);
              }

            for (glob = modules.module[i]->globals; glob != NULL;
                 glob = glob->next)
              {
                STATUSPRINT1("\tGlobal: %s (%c)", glob->desc.name,
                             glob->desc.sig);
              }
          }
        STATUSPRINT1("");
      }
    }

  return returnVal;
}

#ifdef COMPILE_FOR_FORTRAN
void mpi_init_(int *ierr)
{
  /* If MPI was already initialized (by app_startup), then do not start it
   * again, as this this forbidden by the MPI standard.
   *
   * TODO: This is only a temporary fix. The better fix will be to fix the
   * pnmpi_get_mpi_interface function.
   */
  if (pnmpi_init_done)
    {
      *ierr = MPI_SUCCESS;
      return;
    }

  /* some code in here is taken from MPICH-1 */

  int argc;
  char **argv;

#ifdef DBGLEVEL6
  timing_t start_timer;
#endif

#ifdef DBGLEVEL6 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL6))
    {
      pnmpi_overall_timing = get_time_ns();
    }
#endif

#if 0
  int i, argsize = 1024;
  char *p;
  /*  int  argcSave;           Save the argument count */
  char **argvSave;         /* Save the pointer to the argument vector */
#endif

  DBGEARLYINIT();

  DBGPRINT3("Entering Old Fortran MPI_Init at base level");

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Init++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer = get_time_ns();
#endif

  if (pnmpi_init_was_fortran == 0)
    {
      pmpi_init_(ierr);
      return;
    }

  pnmpi_init_was_fortran = 1;

#if 0
  /* argcSave    = */ argc = iargc_() + 1;
  argvSave    = argv = (char **) malloc( argc * sizeof(char *) );

  if (!argv)
    {
      WARNPRINT("Can't allocate memory for argv table - exiting");
      exit(1);
    }

    for (i=0; i<argc; i++)
      {
        argvSave[i] = argv[i] = (char *)malloc( argsize + 1 );
        if (!argv[i])
      {
        WARNPRINT("Can't allocate memory for argv[%i] - exiting",i);
        exit(1);
      }
        getarg_( &i, argv[i], argsize );

        /* Trim trailing blanks */
        p = argv[i] + argsize - 1;
        while (p > argv[i])
      {
            if (*p != ' ')
          {
                p[1] = '\0';
                break;
          }
            p--;
      }
      }

#ifdef DBGLEVEL
    DBGPRINT4("ARGUMENT COUNT IS %i\n",argc);
    for (i=0; i<argc; i++)
      {
    DBGPRINT4("ARGUMENT %i IS %s",i,argv[i]);
      }
#endif /* DBGLEVEL */

  *ierr=PNMPI_Common_MPI_Init(&argc,&argv);
#endif

  argc = 0;
  argv = NULL;

  *ierr = PNMPI_Common_MPI_Init(&argc, &argv);

#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    pnmpi_totalstats_timing.MPI_Init = get_time_ns() - start_timer;
#endif

  return;
}
#endif


int MPI_Init(int *argc, char ***argv)
{
  int err;
#ifdef DBGLEVEL6
  timing_t start_timer;
#endif

#ifdef DBGLEVEL6 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL6))
    {
      pnmpi_overall_timing = get_time_ns();
    }
#endif

  DBGEARLYINIT();

  DBGPRINT3("Entering Old MPI_Init at base level");

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Init++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer = get_time_ns();
#endif

  if (pnmpi_init_was_fortran == 1)
    return PMPI_Init(argc, argv);

  pnmpi_init_was_fortran = 0;

  err = PNMPI_Common_MPI_Init(argc, argv);

#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    pnmpi_totalstats_timing.MPI_Init = get_time_ns() - start_timer;
#endif

  return err;
}

int NQJ_Init(int *_pnmpi_arg_0, char ***_pnmpi_arg_1)
{
  int res;
  int start_level;

  int pnmpi_level = get_pnmpi_level();
  start_level = pnmpi_level;

  if (IS_ACTIVATED(MPI_Init_ID))
    {
      while ((pnmpi_level < pnmpi_max_level) &&
             (modules.module[pnmpi_level]->stack_delimiter == 0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Init[pnmpi_level] != NULL)
            {
#ifdef DBGLEVEL6
              timing_t start_timer;
#endif

              DBGPRINT3("Calling a wrapper in MPI_Init at level %i FROM %px",
                        pnmpi_level, &(Internal_XMPI_Init));
#ifdef DBGLEVEL5
              if (DBGCHECK(DBGLEVEL5))
                modules.module[pnmpi_level]->statscount.MPI_Init++;
#endif
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                start_timer = get_time_ns();
#endif
              res = (pnmpi_function_ptrs.pnmpi_int_MPI_Init)[pnmpi_level](
                _pnmpi_arg_0, _pnmpi_arg_1);
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                modules.module[pnmpi_level]->statstiming.MPI_Init +=
                  get_time_ns() - start_timer;
#endif
              DBGPRINT3(
                "Done with wrapper in MPI_Init at level %i - reseting to %i",
                pnmpi_level, start_level);
              set_pnmpi_level(start_level);
              return res;
            }
          pnmpi_level = inc_pnmpi_level();
        }
    }

  if (pnmpi_init_done)
    {
      DBGPRINT3("Duplicated: calling a original MPI in MPI_Init");
      res = MPI_SUCCESS;
    }
  else
    {
      DBGPRINT3("Calling a original MPI in MPI_Init");
#ifdef COMPILE_FOR_FORTRAN
      if (pnmpi_init_was_fortran)
        pmpi_init_(&res);
      else
#endif
        res = PMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);
      pnmpi_init_done = 1;
    }
  DBGPRINT3("Done with original MPI in MPI_Init");
  set_pnmpi_level(start_level);
  return res;
}

/*-------------------------------------------------------------------*/
/* MPI_Init_thread */
#ifdef HAVE_MPI_INIT_THREAD_C
static int PNMPI_Common_MPI_Init_thread(int *_pnmpi_arg_0, char ***_pnmpi_arg_1,
                                        int required, int *provided)
{
#ifndef __GNUC__
  /* If the compiler does not support GCC's constructors, check if the fallback
   * constructor was called before this function. */
  pnmpi_fallback_init();
#endif


  int returnVal;


  /* Check the maximum threading level of all activated modules. If the
   * application requires a higher threading level as the supported maximum
   * level of all modules, the level will be downgraded. */
  int max_level = pnmpi_max_module_threading_level();
  if (required > max_level)
    {
      WARNPRINT("The application requested a MPI threading level of %d, but "
                "the combination of the selected PnMPI modules provide a "
                "maximum of %d. The threading level will be downgraded.\n",
                required, max_level);
      required = max_level;
    }


  /* If app_startup is activated, we've initialized MPI before. Restore the
   * provided value, MPI_Init_thread gave us in the constructor. */
  if (pnmpi_hook_activated("app_startup"))
    *provided = pnmpi_mpi_thread_level_provided;

  inc_pnmpi_mpi_level();
  if (NOT_ACTIVATED(MPI_Init_thread_ID))
    {
      /* If app_startup is activated, MPI was initialized before in _init. A
       * second call of MPI_Init_thread is not allowed. */
      if (pnmpi_init_done)
        {
          returnVal = MPI_SUCCESS;
        }
      else
        {
#ifdef COMPILE_FOR_FORTRAN
#ifdef HAVE_MPI_INIT_THREAD_Fortran
          if (pnmpi_init_was_fortran)
            pmpi_init_thread_(&required, provided, &returnVal);
          else
#endif /*HAVE_MPI_INIT_THREAD_Fortran*/
#endif
            returnVal =
              PMPI_Init_thread(_pnmpi_arg_0, _pnmpi_arg_1, required, provided);

          pnmpi_init_done = 1;
        }
    }
  else
    returnVal =
      Internal_XMPI_Init_thread(_pnmpi_arg_0, _pnmpi_arg_1, required, provided);

  if (returnVal != MPI_SUCCESS)
    return returnVal;

  // Mpi is initialized now
  pnmpi_initialization_complete = 1;

  PRINTINIT();
  DBGLATEINIT();
  STATUSINIT();
  DBGPRINT1("Leaving Init");
  dec_pnmpi_mpi_level();

  if (getenv("PNMPI_BE_SILENT") == NULL)
    {
      STATUSPRINT1("");
      STATUSPRINT1("\t\t  ---------------------------");
      STATUSPRINT1("\t\t | P^N-MPI Interface         |");
      STATUSPRINT1("\t\t | Martin Schulz, 2005, LLNL |");
      STATUSPRINT1("\t\t  ---------------------------");
      STATUSPRINT1("");

      {
        int i;
        module_servlist_p serv;
        module_globlist_p glob;
        module_arg_t *args;

        STATUSPRINT1("Number of modules: %i", modules.num);
        STATUSPRINT1("Pcontrol Setting:  %i", modules.pcontrol);
        STATUSPRINT1("");
        for (i = 0; i < modules.num; i++)
          {
            if (modules.module[i]->registered)
              {
                STATUSPRINT1("Module %s: registered as %s (Pctrl %i)",
                             modules.module[i]->name,
                             modules.module[i]->username,
                             modules.module[i]->pcontrol);
              }
            else
              {
                if (modules.module[i]->stack_delimiter)
                  {
                    STATUSPRINT1("Stack %s: not registered",
                                 modules.module[i]->name);
                  }
                else
                  {
                    STATUSPRINT1("Module %s: not registered (Pctrl %i)",
                                 modules.module[i]->name,
                                 modules.module[i]->pcontrol);
                  }
              }

            for (args = modules.module[i]->args; args != NULL;
                 args = args->next)
              {
                STATUSPRINT1("\tArgument: %s = %s", args->name, args->value);
              }

            for (serv = modules.module[i]->services; serv != NULL;
                 serv = serv->next)
              {
                STATUSPRINT1("\tService: %s (%s)", serv->desc.name,
                             serv->desc.sig);
              }

            for (glob = modules.module[i]->globals; glob != NULL;
                 glob = glob->next)
              {
                STATUSPRINT1("\tGlobal: %s (%c)", glob->desc.name,
                             glob->desc.sig);
              }
          }
        STATUSPRINT1("");
      }
    }

  return returnVal;
}
#endif /*HAVE_MPI_INIT_THREAD_C*/

#ifdef COMPILE_FOR_FORTRAN
#ifdef HAVE_MPI_INIT_THREAD_Fortran
void mpi_init_thread_(int *required, int *provided, int *ierr)
{
  /* If MPI was already initialized (by app_startup), then do not start it
   * again, as this this forbidden by the MPI standard.
   *
   * TODO: This is only a temporary fix. The better fix will be to fix the
   * pnmpi_get_mpi_interface function.
   */
  if (pnmpi_init_done)
    {
      *ierr = MPI_SUCCESS;
      return;
    }

  /* some code in here is taken from MPICH-1 */

  int argc;
  char **argv;

#ifdef DBGLEVEL6
  timing_t start_timer;
#endif

#ifdef DBGLEVEL6 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL6))
    {
      pnmpi_overall_timing = get_time_ns();
    }
#endif

#if 0
  int i, argsize = 1024;
  char *p;
  /*  int  argcSave;           Save the argument count */
  char **argvSave;         /* Save the pointer to the argument vector */
#endif

  DBGEARLYINIT();

  DBGPRINT3("Entering Old Fortran MPI_Init_thread at base level");

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Init_thread++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer = get_time_ns();
#endif

  if (pnmpi_init_was_fortran == 0)
    {
      pmpi_init_thread_(required, provided, ierr);
      return;
    }

  pnmpi_init_was_fortran = 1;

#if 0
  /* argcSave    = */ argc = iargc_() + 1;
  argvSave    = argv = (char **) malloc( argc * sizeof(char *) );

  if (!argv)
    {
      WARNPRINT("Can't allocate memory for argv table - exiting");
      exit(1);
    }

    for (i=0; i<argc; i++)
      {
        argvSave[i] = argv[i] = (char *)malloc( argsize + 1 );
        if (!argv[i])
      {
        WARNPRINT("Can't allocate memory for argv[%i] - exiting",i);
        exit(1);
      }
        getarg_( &i, argv[i], argsize );

        /* Trim trailing blanks */
        p = argv[i] + argsize - 1;
        while (p > argv[i])
      {
            if (*p != ' ')
          {
                p[1] = '\0';
                break;
          }
            p--;
      }
      }

#ifdef DBGLEVEL
    DBGPRINT4("ARGUMENT COUNT IS %i\n",argc);
    for (i=0; i<argc; i++)
      {
    DBGPRINT4("ARGUMENT %i IS %s",i,argv[i]);
      }
#endif /* DBGLEVEL */

  *ierr=PNMPI_Common_MPI_Init_thread(&argc,&argv);
#endif

  argc = 0;
  argv = NULL;

  *ierr = PNMPI_Common_MPI_Init_thread(&argc, &argv, *required, provided);

#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    pnmpi_totalstats_timing.MPI_Init_thread = get_time_ns() - start_timer;
#endif

  return;
}
#endif /*HAVE_MPI_INIT_THREAD_Fortran*/
#endif

#ifdef HAVE_MPI_INIT_THREAD_C
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
  int err;
#ifdef DBGLEVEL6
  timing_t start_timer;
#endif

#ifdef DBGLEVEL6 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL6))
    {
      pnmpi_overall_timing = get_time_ns();
    }
#endif

  DBGEARLYINIT();

  DBGPRINT3("Entering Old MPI_Init_thread at base level");

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Init_thread++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer = get_time_ns();
#endif

  if (pnmpi_init_was_fortran == 1)
    return PMPI_Init_thread(argc, argv, required, provided);

  pnmpi_init_was_fortran = 0;

  err = PNMPI_Common_MPI_Init_thread(argc, argv, required, provided);

#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    pnmpi_totalstats_timing.MPI_Init_thread = get_time_ns() - start_timer;
#endif

  return err;
}
#endif /*HAVE_MPI_INIT_THREAD_C*/

#ifdef HAVE_MPI_INIT_THREAD_C
int NQJ_Init_thread(int *_pnmpi_arg_0, char ***_pnmpi_arg_1, int _required,
                    int *_provided)
{
  int res;
  int start_level;

  int pnmpi_level = get_pnmpi_level();
  start_level = pnmpi_level;

  if (IS_ACTIVATED(MPI_Init_thread_ID))
    {
      while ((pnmpi_level < pnmpi_max_level) &&
             (modules.module[pnmpi_level]->stack_delimiter == 0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Init_thread[pnmpi_level] !=
              NULL)
            {
#ifdef DBGLEVEL6
              timing_t start_timer;
#endif

              DBGPRINT3(
                "Calling a wrapper in MPI_Init_thread at level %i FROM %px",
                pnmpi_level, &(Internal_XMPI_Init_thread));
#ifdef DBGLEVEL5
              if (DBGCHECK(DBGLEVEL5))
                modules.module[pnmpi_level]->statscount.MPI_Init_thread++;
#endif
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                start_timer = get_time_ns();
#endif
              res =
                (pnmpi_function_ptrs.pnmpi_int_MPI_Init_thread)[pnmpi_level](
                  _pnmpi_arg_0, _pnmpi_arg_1, _required, _provided);
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                modules.module[pnmpi_level]->statstiming.MPI_Init_thread +=
                  get_time_ns() - start_timer;
#endif
              DBGPRINT3("Done with wrapper in MPI_Init_thread at level %i - "
                        "reseting to %i",
                        pnmpi_level, start_level);
              set_pnmpi_level(start_level);
              return res;
            }
          pnmpi_level = inc_pnmpi_level();
        }
    }

  if (pnmpi_init_done)
    {
      DBGPRINT3("Duplicated: calling a original MPI in MPI_Init_thread");
      res = MPI_SUCCESS;
    }
  else
    {
      DBGPRINT3("Calling a original MPI in MPI_Init_thread");
#ifdef COMPILE_FOR_FORTRAN
#ifdef HAVE_MPI_INIT_THREAD_Fortran
      if (pnmpi_init_was_fortran)
        pmpi_init_thread_(&_required, _provided, &res);
      else
#endif /*HAVE_MPI_INIT_THREAD_Fortran*/
#endif
        res =
          PMPI_Init_thread(_pnmpi_arg_0, _pnmpi_arg_1, _required, _provided);
      pnmpi_init_done = 1;
    }
  DBGPRINT3("Done with original MPI in MPI_Init_thread");
  set_pnmpi_level(start_level);
  return res;
}
#endif /*HAVE_MPI_INIT_THREAD_C*/

/*-------------------------------------------------------------------*/
/* MPI_Finalize */

int MPI_Finalize(void)
{
  int err = MPI_ERR_UNKNOWN;
#ifdef DBGLEVEL6
  timing_t start_timer;
#endif

  DBGPRINT3("Entering Old MPI_Finalize at base level - Location = %px",
            &(MPI_Finalize));

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Finalize++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer = get_time_ns();
#endif

  if (NOT_ACTIVATED(MPI_Finalize_ID))
    {
    }
  else
    {
      err = Internal_XMPI_Finalize();
    }

#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    pnmpi_totalstats_timing.MPI_Finalize = get_time_ns() - start_timer;
#endif

#ifdef DBGLEVEL6 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL6))
    {
      pnmpi_overall_timing -= get_time_ns();
    }
#endif

#ifdef DBGLEVEL5 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL5))
    {
      pnmpi_int_print_countstats();
    }
#endif

#ifdef DBGLEVEL6 /* additional timing statistics */
  if (DBGCHECK(DBGLEVEL6))
    {
      pnmpi_int_print_timingstats();
    }
#endif

  /* If the app_shutdown hook is provided by any module, do NOT call the
   * original MPI_Finalize function, because it will be called in _fini after
   * calling the app_shutdown handler. */
  if (pnmpi_hook_activated("app_shutdown") && !pnmpi_finalize_done)
    return MPI_SUCCESS;

  err = PMPI_Barrier(MPI_COMM_WORLD);

  /* It would be better to distinguish between C and Fortran at this point, so
   * if we've called a C PMPI_Init, PMPI_Finalize would be used and for Fortran
   * pmpi_finalize_. We won't do this here, because for a long time there was a
   * bug in OpenMPI that let pmpi_finalize_ calls redirect to MPI_Finalize,
   * which is wrapped by PnMPI and will result in an endless recursion.
   *
   * The bug may be avoided with a complex set of guard variables arround the
   * finalize calls, but until now we didn't got any errors with simply calling
   * the MPI C finalize here. */
  inc_pnmpi_mpi_level();
  err = PMPI_Finalize();
  dec_pnmpi_mpi_level();

  return err;
}

int NQJ_Finalize(void)
{
  int res;
  int start_level;

  int pnmpi_level = get_pnmpi_level();
  start_level = pnmpi_level;

  if (IS_ACTIVATED(MPI_Finalize_ID))
    {
      while ((pnmpi_level < pnmpi_max_level) &&
             (modules.module[pnmpi_level]->stack_delimiter == 0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Finalize[pnmpi_level] != NULL)
            {
#ifdef DBGLEVEL6
              timing_t start_timer;
#endif

              DBGPRINT3(
                "Calling a wrapper in MPI_Finalize at level %i FROM %px",
                pnmpi_level, &(Internal_XMPI_Finalize));
#ifdef DBGLEVEL5
              if (DBGCHECK(DBGLEVEL5))
                modules.module[pnmpi_level]->statscount.MPI_Finalize++;
#endif
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                start_timer = get_time_ns();
#endif
              res = (pnmpi_function_ptrs.pnmpi_int_MPI_Finalize)[pnmpi_level]();
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                modules.module[pnmpi_level]->statstiming.MPI_Finalize +=
                  get_time_ns() - start_timer;
#endif
              DBGPRINT3("Done with wrapper in MPI_Finalize at level %i - "
                        "reseting to %i",
                        pnmpi_level, start_level);
              set_pnmpi_level(start_level);
              return res;
            }
          pnmpi_level = inc_pnmpi_level();
        }
    }

  DBGPRINT3("Calling a original MPI in MPI_Finalize");
  res = MPI_SUCCESS;
  DBGPRINT3("Done with original MPI in MPI_Finalize");
  set_pnmpi_level(start_level);
  return res;
}

#ifdef COMPILE_FOR_FORTRAN
void mpi_finalize_(int *ierr)
{
  *ierr = MPI_Finalize();
  return;
}
#endif

/*-------------------------------------------------------------------*/
/* MPI_Pcontrol */

#define TRANSTYPE doubledouble

typedef struct doubledouble_d
{
  double a, b;
} doubledouble;

typedef struct dummy10_d
{
  TRANSTYPE d[10];
} dummy10_t;

typedef struct dummy100_d
{
  TRANSTYPE d[100];
} dummy100_t;


static int PNMPI_Internal_CallVarg(pnmpi_int_MPI_Pcontrol_t fct, int level,
                                   TRANSTYPE *args, int len)
{
  if (args == NULL)
    {
      return fct(level);
    }
  else
    {
      switch (len)
        {
#include "vargcall.c"
        default:
          WARNPRINT("Pcontrol arguments too long - reporting error");
          return PNMPI_ERROR;
        }
    }
}


int MPI_Pcontrol(int level, ...)
{
#ifdef EXPERIMENTAL_UNWIND
  unw_context_t ctx;
  unw_cursor_t ptr;
  unw_word_t sp1, sp2, len;
#endif
  TRANSTYPE *start;
  int leni, mod, modnum, size, type, actlevel;
  int error = MPI_SUCCESS;
  int *mods, *errors;
  va_list va_alist;

  int ret, i;

#ifdef DBGLEVEL6
  timing_t start_timer2;
#endif

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer2 = get_time_ns();
#endif

  DBGPRINT3("Entering Old MPI_Pcontrol at base level");

  /* This mode just delegates the level arg to modules w/pcontrol on. */
  if ((modules.pcontrol == PNMPI_PCONTROL_INT) ||
      ((modules.pcontrol == PNMPI_PCONTROL_TYPED) &&
       (modules.pcontrol_typed_level != level)))
    {
      int curr_pnmpi_level = get_pnmpi_level();
      for (i = 0; i < pnmpi_max_level; i++)
        {
          set_pnmpi_level(i);
          if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i] != NULL) &&
              (modules.module[i]->pcontrol))
            {
#ifdef DBGLEVEL6
              timing_t start_timer;
#endif

#ifdef DBGLEVEL5
              if (DBGCHECK(DBGLEVEL5))
                modules.module[i]->statscount.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                start_timer = get_time_ns();
#endif
              /* yes, we need to call this Pcontrol */
              ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level);
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                modules.module[i]->statstiming.MPI_Pcontrol +=
                  get_time_ns() - start_timer;
#endif
              if (ret != MPI_SUCCESS)
                return ret;
            }
        }
      set_pnmpi_level(curr_pnmpi_level);
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      return MPI_SUCCESS;
    }

  /* This mode delegates the level arg plus one typed parameter to modules
   * w/pcontrol on. */
  /* in this case: modules.pcontrol_typed_level == level */
  if (modules.pcontrol == PNMPI_PCONTROL_TYPED)
    {
      int arg_int = 0;
      long arg_long = 0;
      void *arg_ptr = NULL;
      double arg_double = 0.0;

      va_start(va_alist, level);
      switch (modules.pcontrol_typed_type)
        {
        case PNMPI_PCONTROL_TYPE_INT: arg_int = va_arg(va_alist, int); break;
        case PNMPI_PCONTROL_TYPE_LONG: arg_long = va_arg(va_alist, long); break;
        case PNMPI_PCONTROL_TYPE_PTR: arg_ptr = va_arg(va_alist, void *); break;
        case PNMPI_PCONTROL_TYPE_DOUBLE:
          arg_double = va_arg(va_alist, double);
          break;
        default:
          WARNPRINT("Unknown pcontrol argument type, defaulting to "
                    "PNMPI_PCONTROL_TYPED\n");
        }
      va_end(va_alist);

      int curr_pnmpi_level = get_pnmpi_level();
      for (i = 0; i < pnmpi_max_level; i++)
        {
          set_pnmpi_level(i);
          if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i] != NULL) &&
              (modules.module[i]->pcontrol))
            {
#ifdef DBGLEVEL6
              timing_t start_timer;
#endif

#ifdef DBGLEVEL5
              if (DBGCHECK(DBGLEVEL5))
                modules.module[i]->statscount.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                start_timer = get_time_ns();
#endif
              switch (modules.pcontrol_typed_type)
                {
                case PNMPI_PCONTROL_TYPE_INT:
                  ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,
                                                                      arg_int);
                  break;
                case PNMPI_PCONTROL_TYPE_LONG:
                  ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,
                                                                      arg_long);
                  break;
                case PNMPI_PCONTROL_TYPE_PTR:
                  ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,
                                                                      arg_ptr);
                  break;
                case PNMPI_PCONTROL_TYPE_DOUBLE:
                  ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](
                    level, arg_double);
                  break;
                default:
                  ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level);
                }
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                modules.module[i]->statstiming.MPI_Pcontrol +=
                  get_time_ns() - start_timer2;
#endif
              if (ret != MPI_SUCCESS)
                return ret;
            }
        }
      set_pnmpi_level(curr_pnmpi_level);
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      return MPI_SUCCESS;
    }


  if (modules.pcontrol == PNMPI_PCONTROL_OFF)
    {
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      return MPI_SUCCESS;
    }

  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) &&
      (level != PNMPI_PCONTROL_LEVEL))
    {
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      return PNMPI_ERROR;
    }

  /* start processing the variable list arguments */

  va_start(va_alist, level);

  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) ||
      ((modules.pcontrol == PNMPI_PCONTROL_MIXED) &&
       (level == PNMPI_PCONTROL_LEVEL)))
    {
      /* own format */
      /* int level = PNMPI_PCONTROL_LEVEL
         int level = Actual level to transmit
         int type  = PNMPI_PCONTROL_SINGLE or MULTIPLE |
                     PNMPI_PCONTROL_VARG or PTR
         int mod   = module number (if SINGLE)
         int modnum / int* mods = module numbers (if MULTIPLE)
         int *errors (if MULTIPLE and MULTIPLEERROR)
         int size   = size of all arguments in byte
         ...       = arguments (if VARG)
         void *buf = arguments (if PTR) */

      actlevel = va_arg(va_alist, int);
      type = va_arg(va_alist, int);
      errors = NULL;
      if (type & PNMPI_PCONTROL_SINGLEorMULTIPLE)
        {
          /* MULTIPLE */
          modnum = va_arg(va_alist, int);
          mods = va_arg(va_alist, int *);
          mod = mods[0];
          if (type & PNMPI_PCONTROL_SINGLEorMULTIPLEERROR)
            {
              errors = va_arg(va_alist, int *);
            }
        }
      else
        {
          /* SINGLE */
          mod = va_arg(va_alist, int);
          modnum = 1;
          mods = &mod;
        }
      size = va_arg(va_alist, int);
      if (type & PNMPI_PCONTROL_VARGorPTR)
        {
          /* PTR */
          start = (TRANSTYPE *)(va_arg(va_alist, void *));
        }
      else
        {
          start = (TRANSTYPE *)va_alist;
        }
      leni = (size + sizeof(TRANSTYPE) - 1) / sizeof(TRANSTYPE);

      error = MPI_SUCCESS;
      for (i = 0; i < modnum; i++)
        {
          if ((mods[i] < 0) || (mods[i] >= pnmpi_max_level))
            ret = PNMPI_ERROR;
          else
            {
              if (pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[mods[i]] != NULL)
                {
/* yes, we need to call this Pcontrol */
#ifdef DBGLEVEL6
                  timing_t start_timer;
#endif

#ifdef DBGLEVEL5
                  if (DBGCHECK(DBGLEVEL5))
                    modules.module[i]->statscount.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
                  if (DBGCHECK(DBGLEVEL6))
                    start_timer = get_time_ns();
#endif
                  ret = PNMPI_Internal_CallVarg(
                    pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[mods[i]],
                    actlevel, start, leni);
#ifdef DBGLEVEL6
                  if (DBGCHECK(DBGLEVEL6))
                    modules.module[i]->statstiming.MPI_Pcontrol +=
                      get_time_ns() - start_timer2;
#endif
                }
              else
                ret = MPI_SUCCESS;
            }
          if (ret != MPI_SUCCESS)
            error = ret;
          if (errors)
            errors[i] = ret;
        }

      va_end(va_alist);

#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      return error;
    }
  else
    {
#ifndef EXPERIMENTAL_UNWIND
      WARNPRINT(
        "This Pcontrol option is not supported (enable EXPERIMENTAL_UNWIND)\n");
#else
      /* not our format, need to extract arguments */

      unw_getcontext(&ctx); /* check for error */
      unw_init_local(&ptr, &ctx);
      unw_get_reg(&ptr, UNW_REG_SP, &sp1);
      unw_step(&ptr);
      unw_get_reg(&ptr, UNW_REG_SP, &sp2);

      /* Difference between stack frames minus all local vars */
      /* THIS MUST BE ADJUSTED IF THE LOCAL VARIABLES OF THIS
         ROUTINE ARE CHANGED !!! */

      len = sp2 - sp1 - sizeof(ctx) - sizeof(ptr) - 3 * sizeof(sp1) -
            sizeof(start) - 9 * sizeof(int) - sizeof(va_alist) -
            2 * sizeof(int *);

      unw_step(&ptr);
      unw_get_reg(&ptr, UNW_REG_SP, &sp1);
      len = sp1 - sp2 - len - sizeof(level);

      start = (TRANSTYPE *)(sp2 + sizeof(level));

      leni = (len + sizeof(TRANSTYPE) - 1) / sizeof(TRANSTYPE);

      error = MPI_SUCCESS;
      for (i = 0; i < pnmpi_max_level; i++)
        {
          if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i] != NULL) &&
              ((modules.pcontrol == PNMPI_PCONTROL_ON) ||
               (modules.module[i]->pcontrol)))
            {
/* yes, we need to call this Pcontrol */
#ifdef DBGLEVEL6
              timing_t start_timer;
#endif

#ifdef DBGLEVEL5
              if (DBGCHECK(DBGLEVEL5))
                modules.module[i]->statscount.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                start_timer = get_time_ns();
#endif
              ret = PNMPI_Internal_CallVarg(
                pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i], level, start,
                leni);
#ifdef DBGLEVEL6
              if (DBGCHECK(DBGLEVEL6))
                modules.module[i]->statstiming.MPI_Pcontrol +=
                  get_time_ns() - start_timer2;
#endif
              if (ret != MPI_SUCCESS)
                error = ret;
            }
        }

#endif /*else EXPERIMENTAL_UNWIND*/
      va_end(va_alist);

#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      return error;
    }
}


void mpi_pcontrol_(int *level, int *ierr)
{
  int i, ret;

#ifdef DBGLEVEL6
  timing_t start_timer2;
#endif

#ifdef DBGLEVEL5
  if (DBGCHECK(DBGLEVEL5))
    pnmpi_totalstats_count.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    start_timer2 = get_time_ns();
#endif

  DBGPRINT3("Entering Old Fortran MPI_Pcontrol at base level");

  if (modules.pcontrol == PNMPI_PCONTROL_OFF)
    {
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      *ierr = MPI_SUCCESS;
      return;
    }
  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) ||
      (modules.pcontrol == PNMPI_PCONTROL_TYPED) ||
      ((modules.pcontrol == PNMPI_PCONTROL_MIXED) &&
       (*level == PNMPI_PCONTROL_LEVEL)))
    {
/* can't do that in Fortran */
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
      *ierr = PNMPI_ERROR;
      return;
    }

  *ierr = MPI_SUCCESS;
  for (i = 0; i < pnmpi_max_level; i++)
    {
      if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i] != NULL) &&
          ((modules.pcontrol == PNMPI_PCONTROL_ON) ||
           (modules.module[i]->pcontrol)))
        {
#ifdef DBGLEVEL6
          timing_t start_timer;
#endif
/* yes, we need to call this Pcontrol */
#ifdef DBGLEVEL5
          if (DBGCHECK(DBGLEVEL5))
            modules.module[i]->statscount.MPI_Pcontrol++;
#endif
#ifdef DBGLEVEL6
          if (DBGCHECK(DBGLEVEL6))
            start_timer = get_time_ns();
#endif
          ret = PNMPI_Internal_CallVarg(
            pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i], *level, NULL, 0);
#ifdef DBGLEVEL6
          if (DBGCHECK(DBGLEVEL6))
            modules.module[i]->statstiming.MPI_Pcontrol +=
              get_time_ns() - start_timer2;
#endif
          if (ret != MPI_SUCCESS)
            *ierr = ret;
        }
    }
#ifdef DBGLEVEL6
  if (DBGCHECK(DBGLEVEL6))
    pnmpi_totalstats_timing.MPI_Pcontrol = get_time_ns() - start_timer2;
#endif
  return;
}

/*-------------------------------------------------------------------*/
/* special case for FORTRAN timing routines */

double mpi_wtick_(void)
{
  DBGPRINT3("Entering Old Fortran mpi_wtick_ at base level - Location = %px",
            &(MPI_Wtick));

  if (NOT_ACTIVATED(MPI_Wtick_ID))
    return PMPI_Wtick();
  else
    {
      double ret;
#ifdef DBGLEVEL6
      timing_t start_timer;
#endif

#ifdef DBGLEVEL5
      if (DBGCHECK(DBGLEVEL5))
        pnmpi_totalstats_count.MPI_Wtick++;
#endif
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        start_timer = get_time_ns();
#endif
      ret = Internal_XMPI_Wtick();
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Wtick = get_time_ns() - start_timer;
#endif
      return ret;
    }
}

double mpi_wtime_(void)
{
  DBGPRINT3("Entering Old Fortran mpi_wtime_ at base level - Location = %px",
            &(MPI_Wtime));

  if (NOT_ACTIVATED(MPI_Wtime_ID))
    return PMPI_Wtime();
  else
    {
      double ret;
#ifdef DBGLEVEL6
      timing_t start_timer;
#endif

#ifdef DBGLEVEL5
      if (DBGCHECK(DBGLEVEL5))
        pnmpi_totalstats_count.MPI_Wtime++;
#endif
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        start_timer = get_time_ns();
#endif
      ret = Internal_XMPI_Wtime();
#ifdef DBGLEVEL6
      if (DBGCHECK(DBGLEVEL6))
        pnmpi_totalstats_timing.MPI_Wtime = get_time_ns() - start_timer;
#endif
      return ret;
    }
}


/*-------------------------------------------------------------------*/
/* The End. */

#include "wrapper_c.c"
