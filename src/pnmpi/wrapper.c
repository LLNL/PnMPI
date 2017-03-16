/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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
#include <pnmpi/debug_io.h>
#include <pnmpi/private/mpi_reentry.h>


/* Map the old debug macros to the new debug functions and macros.
 *
 * WARNING:
 * - These macros are not portable to non-GCC compatible compilers, as
 *   ##__VA_ARGS__ is a GNU extension.
 * - These macros are for legacy support only. Use the functions and macros
 *   defined in debug_io.h for new code!
 */
#define WARNPRINT(format, ...) pnmpi_warning(format "\n", ##__VA_ARGS__);
#define DBGPRINT3(format, ...) \
  pnmpi_debug(PNMPI_DEBUG_CALL, format "\n", ##__VA_ARGS__);

/* Enable all debug levels, as checking the debug level to print is part of
 * pnmpi_debug now. */
#define DBGLEVEL


extern void *MPIR_ToPointer(int idx);

#ifdef COMPILE_FOR_FORTRAN
void pmpi_init_(int *ierror);
void pmpi_finalize_(int *ierr);
#ifdef HAVE_MPI_INIT_THREAD_Fortran
void pmpi_init_thread_(int *ierror, int *required, int *provided);
#endif /*HAVE_MPI_INIT_THREAD_Fortran*/
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
      /* If PNMPI_AppStartup is activated, MPI was initialized before in _init.
       * A second call of MPI_Init is not allowed. */
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

  dec_pnmpi_mpi_level();

  if (returnVal != MPI_SUCCESS)
    return returnVal;

  // Mpi is initialized now
  pnmpi_initialization_complete = 1;

  pnmpi_print_banner();

  return returnVal;
}

#ifdef COMPILE_FOR_FORTRAN
void mpi_init_(int *ierr)
{
  /* Enter the reentry-guarded wrapper section. If the section was already
   * entered, all calls will be passed to the PMPI interface without calling the
   * wrappers. This is neccessary, as some (older) MPI implementations send
   * Fortran PMPI calls back to the C MPI interface, so the wrappers would get
   * called twice. */
  if (PNMPI_REENTRY_ENTER())
    {
      pmpi_init_(ierr);
      return;
    }

  /* If MPI was already initialized (by the PNMPI_AppStartup hook), then do not
   * start it again, as this this forbidden by the MPI standard.
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

#if 0
  int i, argsize = 1024;
  char *p;
  /*  int  argcSave;           Save the argument count */
  char **argvSave;         /* Save the pointer to the argument vector */
#endif

  DBGPRINT3("Entering Old Fortran MPI_Init at base level");

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

  PNMPI_REENTRY_EXIT();
  return;
}
#endif


int MPI_Init(int *argc, char ***argv)
{
  /* Enter the reentry-guarded wrapper section. If the section was already
   * entered, all calls will be passed to the PMPI interface without calling the
   * wrappers. This is neccessary, as some (older) MPI implementations send
   * Fortran PMPI calls back to the C MPI interface, so the wrappers would get
   * called twice. */
  if (PNMPI_REENTRY_ENTER())
    return PMPI_Init(argc, argv);

  int err;

  DBGPRINT3("Entering Old MPI_Init at base level");

  if (pnmpi_init_was_fortran == 1)
    return PMPI_Init(argc, argv);

  pnmpi_init_was_fortran = 0;

  err = PNMPI_Common_MPI_Init(argc, argv);

  PNMPI_REENTRY_EXIT();
  return err;
}

int NQJ_Init(int *_pnmpi_arg_0, char ***_pnmpi_arg_1)
{
  int res;
  int start_level;

  start_level = pnmpi_level;

  if (IS_ACTIVATED(MPI_Init_ID))
    {
      while ((pnmpi_level < pnmpi_max_level) &&
             (modules.module[pnmpi_level]->stack_delimiter == 0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Init[pnmpi_level] != NULL)
            {
              DBGPRINT3("Calling a wrapper in MPI_Init at level %i FROM %px",
                        pnmpi_level, &(Internal_XMPI_Init));
              res = (pnmpi_function_ptrs.pnmpi_int_MPI_Init)[pnmpi_level](
                _pnmpi_arg_0, _pnmpi_arg_1);
              DBGPRINT3(
                "Done with wrapper in MPI_Init at level %i - reseting to %i",
                pnmpi_level, start_level);
              pnmpi_level = start_level;
              return res;
            }
          ++pnmpi_level;
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
  pnmpi_level = start_level;
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


  /* If the PNMPI_AppStartup hook is activated, we've initialized MPI before.
   * Restore the provided value, MPI_Init_thread gave us in the constructor. */
  if (pnmpi_hook_activated("PNMPI_AppStartup"))
    *provided = pnmpi_mpi_thread_level_provided;

  inc_pnmpi_mpi_level();
  if (NOT_ACTIVATED(MPI_Init_thread_ID))
    {
      /* If the PNMPI_AppStartup hook is activated, MPI was initialized before
       * in _init. A second call of MPI_Init_thread is not allowed. */
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

  dec_pnmpi_mpi_level();

  if (returnVal != MPI_SUCCESS)
    return returnVal;

  // Mpi is initialized now
  pnmpi_initialization_complete = 1;

  pnmpi_print_banner();

  return returnVal;
}
#endif /*HAVE_MPI_INIT_THREAD_C*/

#ifdef COMPILE_FOR_FORTRAN
#ifdef HAVE_MPI_INIT_THREAD_Fortran
void mpi_init_thread_(int *required, int *provided, int *ierr)
{
  /* Enter the reentry-guarded wrapper section. If the section was already
   * entered, all calls will be passed to the PMPI interface without calling the
   * wrappers. This is neccessary, as some (older) MPI implementations send
   * Fortran PMPI calls back to the C MPI interface, so the wrappers would get
   * called twice. */
  if (PNMPI_REENTRY_ENTER())
    {
      pmpi_init_thread_(required, provided, ierr);
      return;
    }

  /* If MPI was already initialized (by the PNMPI_AppStartup hook), then do not
   * start it again, as this this forbidden by the MPI standard.
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

#if 0
  int i, argsize = 1024;
  char *p;
  /*  int  argcSave;           Save the argument count */
  char **argvSave;         /* Save the pointer to the argument vector */
#endif

  DBGPRINT3("Entering Old Fortran MPI_Init_thread at base level");

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

  PNMPI_REENTRY_EXIT();
  return;
}
#endif /*HAVE_MPI_INIT_THREAD_Fortran*/
#endif

#ifdef HAVE_MPI_INIT_THREAD_C
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
  /* Enter the reentry-guarded wrapper section. If the section was already
   * entered, all calls will be passed to the PMPI interface without calling the
   * wrappers. This is neccessary, as some (older) MPI implementations send
   * Fortran PMPI calls back to the C MPI interface, so the wrappers would get
   * called twice. */
  if (PNMPI_REENTRY_ENTER())
    return PMPI_Init_thread(argc, argv, required, provided);

  int err;

  DBGPRINT3("Entering Old MPI_Init_thread at base level");

  if (pnmpi_init_was_fortran == 1)
    return PMPI_Init_thread(argc, argv, required, provided);

  pnmpi_init_was_fortran = 0;

  err = PNMPI_Common_MPI_Init_thread(argc, argv, required, provided);

  PNMPI_REENTRY_EXIT();
  return err;
}
#endif /*HAVE_MPI_INIT_THREAD_C*/

#ifdef HAVE_MPI_INIT_THREAD_C
int NQJ_Init_thread(int *_pnmpi_arg_0, char ***_pnmpi_arg_1, int _required,
                    int *_provided)
{
  int res;
  int start_level;

  start_level = pnmpi_level;

  if (IS_ACTIVATED(MPI_Init_thread_ID))
    {
      while ((pnmpi_level < pnmpi_max_level) &&
             (modules.module[pnmpi_level]->stack_delimiter == 0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Init_thread[pnmpi_level] !=
              NULL)
            {
              DBGPRINT3(
                "Calling a wrapper in MPI_Init_thread at level %i FROM %px",
                pnmpi_level, &(Internal_XMPI_Init_thread));
              res =
                (pnmpi_function_ptrs.pnmpi_int_MPI_Init_thread)[pnmpi_level](
                  _pnmpi_arg_0, _pnmpi_arg_1, _required, _provided);
              DBGPRINT3("Done with wrapper in MPI_Init_thread at level %i - "
                        "reseting to %i",
                        pnmpi_level, start_level);
              pnmpi_level = start_level;
              return res;
            }
          ++pnmpi_level;
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
  pnmpi_level = start_level;
  return res;
}
#endif /*HAVE_MPI_INIT_THREAD_C*/

/*-------------------------------------------------------------------*/
/* MPI_Finalize */

int MPI_Finalize(void)
{
  /* Enter the reentry-guarded wrapper section. If the section was already
   * entered, all calls will be passed to the PMPI interface without calling the
   * wrappers. This is neccessary, as some (older) MPI implementations send
   * Fortran PMPI calls back to the C MPI interface, so the wrappers would get
   * called twice. */
  if (PNMPI_REENTRY_ENTER())
    return PMPI_Finalize();


  int err = MPI_ERR_UNKNOWN;

  DBGPRINT3("Entering Old MPI_Finalize at base level - Location = %px",
            &(MPI_Finalize));

  if (NOT_ACTIVATED(MPI_Finalize_ID))
    {
    }
  else
    {
      err = Internal_XMPI_Finalize();
    }

  /* If the PNMPI_AppShutdown hook is provided by any module, do NOT call the
   * original MPI_Finalize function, because it will be called in _fini after
   * calling the app_shutdown handler. */
  if (pnmpi_hook_activated("PNMPI_AppShutdown"))
    return MPI_SUCCESS;

  inc_pnmpi_mpi_level();
#ifdef COMPILE_FOR_FORTRAN
  if (pnmpi_get_mpi_interface(NULL) == PNMPI_INTERFACE_FORTRAN)
    pmpi_finalize_(&err);
  else
#endif
    err = PMPI_Finalize();
  dec_pnmpi_mpi_level();

  PNMPI_REENTRY_EXIT();
  return err;
}

int NQJ_Finalize(void)
{
  int res;
  int start_level;

  start_level = pnmpi_level;

  if (IS_ACTIVATED(MPI_Finalize_ID))
    {
      while ((pnmpi_level < pnmpi_max_level) &&
             (modules.module[pnmpi_level]->stack_delimiter == 0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Finalize[pnmpi_level] != NULL)
            {
              DBGPRINT3(
                "Calling a wrapper in MPI_Finalize at level %i FROM %px",
                pnmpi_level, &(Internal_XMPI_Finalize));
              res = (pnmpi_function_ptrs.pnmpi_int_MPI_Finalize)[pnmpi_level]();
              DBGPRINT3("Done with wrapper in MPI_Finalize at level %i - "
                        "reseting to %i",
                        pnmpi_level, start_level);
              pnmpi_level = start_level;
              return res;
            }
          ++pnmpi_level;
        }
    }

  DBGPRINT3("Calling a original MPI in MPI_Finalize");
  res = MPI_SUCCESS;
  DBGPRINT3("Done with original MPI in MPI_Finalize");
  pnmpi_level = start_level;
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

  DBGPRINT3("Entering Old MPI_Pcontrol at base level");

  /* This mode just delegates the level arg to modules w/pcontrol on. */
  if ((modules.pcontrol == PNMPI_PCONTROL_INT) ||
      ((modules.pcontrol == PNMPI_PCONTROL_TYPED) &&
       (modules.pcontrol_typed_level != level)))
    {
      int curr_pnmpi_level = pnmpi_level;
      for (i = 0; i < pnmpi_max_level; i++)
        {
          pnmpi_level = i;
          if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i] != NULL) &&
              (modules.module[i]->pcontrol))
            {
              /* yes, we need to call this Pcontrol */
              ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level);
              if (ret != MPI_SUCCESS)
                return ret;
            }
        }
      pnmpi_level = curr_pnmpi_level;
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

      int curr_pnmpi_level = pnmpi_level;
      for (i = 0; i < pnmpi_max_level; i++)
        {
          pnmpi_level = i;
          if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i] != NULL) &&
              (modules.module[i]->pcontrol))
            {
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

              if (ret != MPI_SUCCESS)
                return ret;
            }
        }
      pnmpi_level = curr_pnmpi_level;

      return MPI_SUCCESS;
    }


  if (modules.pcontrol == PNMPI_PCONTROL_OFF)
    return MPI_SUCCESS;

  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) &&
      (level != PNMPI_PCONTROL_LEVEL))
    return PNMPI_ERROR;

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
                /* yes, we need to call this Pcontrol */
                ret = PNMPI_Internal_CallVarg(
                  pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[mods[i]], actlevel,
                  start, leni);
              else
                ret = MPI_SUCCESS;
            }
          if (ret != MPI_SUCCESS)
            error = ret;
          if (errors)
            errors[i] = ret;
        }

      va_end(va_alist);

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
              ret = PNMPI_Internal_CallVarg(
                pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i], level, start,
                leni);

              if (ret != MPI_SUCCESS)
                error = ret;
            }
        }

#endif /*else EXPERIMENTAL_UNWIND*/
      va_end(va_alist);

      return error;
    }
}


void mpi_pcontrol_(int *level, int *ierr)
{
  int i, ret;

  DBGPRINT3("Entering Old Fortran MPI_Pcontrol at base level");

  if (modules.pcontrol == PNMPI_PCONTROL_OFF)
    {
      *ierr = MPI_SUCCESS;
      return;
    }
  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) ||
      (modules.pcontrol == PNMPI_PCONTROL_TYPED) ||
      ((modules.pcontrol == PNMPI_PCONTROL_MIXED) &&
       (*level == PNMPI_PCONTROL_LEVEL)))
    {
      /* can't do that in Fortran */
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
          /* yes, we need to call this Pcontrol */
          ret = PNMPI_Internal_CallVarg(
            pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i], *level, NULL, 0);

          if (ret != MPI_SUCCESS)
            *ierr = ret;
        }
    }

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
    return Internal_XMPI_Wtick();
}

double mpi_wtime_(void)
{
  DBGPRINT3("Entering Old Fortran mpi_wtime_ at base level - Location = %px",
            &(MPI_Wtime));

  if (NOT_ACTIVATED(MPI_Wtime_ID))
    return PMPI_Wtime();
  else
    return Internal_XMPI_Wtime();
}


/*-------------------------------------------------------------------*/
/* The End. */

#include "wrapper_c.c"
