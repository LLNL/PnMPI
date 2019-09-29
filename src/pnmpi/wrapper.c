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

#include <stdarg.h>

#include "core.h"
#include "pnmpi-config.h"
#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/private/force_link.h>
#include <pnmpi/private/function_address.h>
#include <pnmpi/private/initialization.h>
#include <pnmpi/private/modules.h>
#include <pnmpi/private/mpi_interface.h>
#include <pnmpi/private/mpi_reentry.h>
#include <pnmpi/private/pmpi.h>
#include <pnmpi/private/pmpi_assert.h>
#include <pnmpi/private/return_address.h>
#include <pnmpi/private/tls.h>

/* Special wrappers not covered by the generator */
#ifdef EXPERIMENTAL_UNWIND
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif


/* Map the old debug macros to the new debug functions and macros.
 *
 * WARNING:
 * - These macros are not portable to non-GCC compatible compilers, as
 *   ##__VA_ARGS__ is a GNU extension.
 * - These macros are for legacy support only. Use the functions and macros
 *   defined in debug_io.h for new code!
 */
#define WARNPRINT(format, ...) PNMPI_Warning(format "\n", ##__VA_ARGS__);
#define DBGPRINT3(format, ...) \
  PNMPI_Debug(PNMPI_DEBUG_CALL, format "\n", ##__VA_ARGS__);

/* Enable all debug levels, as checking the debug level to print is part of
 * pnmpi_debug now. */
#define DBGLEVEL


extern void *MPIR_ToPointer(int idx);


/*-------------------------------------------------------------------*/
/* MPI_Init */

static int PNMPI_Common_MPI_Init(int *_pnmpi_arg_0, char ***_pnmpi_arg_1)
{
#ifndef __PIC__
  /* The following call has no effect on the PnMPI execution, but forces any
   * required symbols of PnMPI to be linked, if one links the static PnMPI
   * library. */
  pnmpi_force_link();
#endif

  /* Initialize PnMPI before using any of the PnMPI functions. This will load
   * the configuration and the defined modules.
   *
   * Note: Duplicated calls to the initialization function will be ignored, so
   *       this doesn't need to be checked. */
  pnmpi_initialize();


  int returnVal = MPI_SUCCESS;

  inc_pnmpi_mpi_level();
  if (NOT_ACTIVATED(MPI_Init_ID))
    {
#ifdef COMPILE_FOR_FORTRAN
      if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_FORTRAN)
        pmpi_init_(&returnVal);
      else
#endif
        returnVal = PMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);

      pnmpi_print_banner();
    }
  else
    returnVal = Internal_XMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);

  dec_pnmpi_mpi_level();

  if (returnVal != MPI_SUCCESS)
    return returnVal;

  // Mpi is initialized now
  pnmpi_initialization_complete = 1;

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
  if (pnmpi_reentry_enter())
    {
      pmpi_init_(ierr);
      return;
    }

  /* Set the MPI interface language used to call MPI initialization, so NQJ_Init
   * knows which MPI interface to initialize. */
  pnmpi_set_mpi_interface_fortran();

  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Init);


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

  /* Exit the reentry-guarded wrapper section, reset the MPI interface used for
   * this MPI call and the return and function addresses to the default ones. */
  pnmpi_reentry_exit();
  pnmpi_reset_mpi_interface();
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
}

void MPI_INIT(int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_init_(ierr);
}

void mpi_init(int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_init_(ierr);
}

void mpi_init__(int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_init_(ierr);
}
#endif


int MPI_Init(int *argc, char ***argv)
{
  /* Enter the reentry-guarded wrapper section. If the section was already
   * entered, all calls will be passed to the PMPI interface without calling the
   * wrappers. This is neccessary, as some (older) MPI implementations send
   * Fortran PMPI calls back to the C MPI interface, so the wrappers would get
   * called twice. */
  if (pnmpi_reentry_enter())
    return PMPI_Init(argc, argv);

  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Init);

  /* Recurse into the MPI_Init wrapper. After the recursion reaches the bottom
   * of the stack, the real PMPI_Init will be called and its return status
   * returned. */
  int status = PNMPI_Common_MPI_Init(argc, argv);

  /* Exit the reentry-guarded wrapper section, reset the return and function
   * addresses of this MPI call and return the status of the MPI initialization
   * stored earlier */
  pnmpi_reentry_exit();
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
  return status;
}

int NQJ_Init(int *_pnmpi_arg_0, char ***_pnmpi_arg_1)
{
  int res = MPI_SUCCESS;
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
      if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_FORTRAN)
        pmpi_init_(&res);
      else
#endif
        res = PMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);
      pnmpi_init_done = 1;
    }
  DBGPRINT3("Done with original MPI in MPI_Init");
  pnmpi_level = start_level;

  pnmpi_print_banner();

  return res;
}

/*-------------------------------------------------------------------*/
/* MPI_Init_thread */
#ifdef HAVE_MPI_INIT_THREAD_C
static int PNMPI_Common_MPI_Init_thread(int *_pnmpi_arg_0, char ***_pnmpi_arg_1,
                                        int required, int *provided)
{
#ifndef __PIC__
  /* The following call has no effect on the PnMPI execution, but forces any
   * required symbols of PnMPI to be linked, if one links the static PnMPI
   * library. */
  pnmpi_force_link();
#endif

  /* Initialize PnMPI before using any of the PnMPI functions. This will load
   * the configuration and the defined modules.
   *
   * Note: Duplicated calls to the initialization function will be ignored, so
   *       this doesn't need to be checked. */
  pnmpi_initialize();


#ifdef PNMPI_COMPILER_NO_TLS
  /* If PnMPI has no thread local storage, it can't be used thread safe and the
   * max. allowed thread level of MPI has to be limited. */
  if (required > MPI_THREAD_SERIALIZED)
    {
      PNMPI_Warning("The application requested a MPI threading level of %d, "
                    "but PnMPI is not compiled thread safe. MPI threading will "
                    "be limited to 'MPI_THREAD_SERIALIZED'.\n",
                    required);
      required = MPI_THREAD_SERIALIZED;
    }
#endif

  int returnVal = MPI_SUCCESS;

  inc_pnmpi_mpi_level();
  if (NOT_ACTIVATED(MPI_Init_thread_ID))
    {
#ifdef COMPILE_FOR_FORTRAN
      if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_FORTRAN)
        pmpi_init_thread_(&required, provided, &returnVal);
      else
#endif
        returnVal =
          PMPI_Init_thread(_pnmpi_arg_0, _pnmpi_arg_1, required, provided);

      pnmpi_print_banner();
    }
  else
    returnVal =
      Internal_XMPI_Init_thread(_pnmpi_arg_0, _pnmpi_arg_1, required, provided);

  dec_pnmpi_mpi_level();

  if (returnVal != MPI_SUCCESS)
    return returnVal;

  // Mpi is initialized now
  pnmpi_initialization_complete = 1;

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
  if (pnmpi_reentry_enter())
    {
      pmpi_init_thread_(required, provided, ierr);
      return;
    }

  /* Set the MPI interface language used to call MPI initialization, so
   * NQJ_Init_thread knows which MPI interface to initialize. */
  pnmpi_set_mpi_interface_fortran();

  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Init_thread);


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

  /* Exit the reentry-guarded wrapper section, reset the return and function
   * addresses of this MPI call and reset the MPI interface used for this MPI
   * call to the default one. */
  pnmpi_reentry_exit();
  pnmpi_reset_mpi_interface();
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
}

void MPI_INIT_THREAD(int *required, int *provided, int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_init_thread_(required, provided, ierr);
}

void mpi_init_thread(int *required, int *provided, int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_init_thread_(required, provided, ierr);
}

void mpi_init_thread__(int *required, int *provided, int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_init_thread_(required, provided, ierr);
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
  if (pnmpi_reentry_enter())
    return PMPI_Init_thread(argc, argv, required, provided);

  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Init_thread);

  /* Recurse into the MPI_Init wrapper. After the recursion reaches the bottom
   * of the stack, the real PMPI_Init will be called and its return status
   * returned. */
  int err = PNMPI_Common_MPI_Init_thread(argc, argv, required, provided);

  /* Exit the reentry-guarded wrapper section, reset the return and function
   * addresses of this MPI call and return the status of the MPI initialization
   * stored earlier. */
  pnmpi_reentry_exit();
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
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
      if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_FORTRAN)
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

  pnmpi_print_banner();

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
  if (pnmpi_reentry_enter())
    return PMPI_Finalize();

  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Finalize);


  int err = MPI_ERR_UNKNOWN;

  DBGPRINT3("Entering Old MPI_Finalize at base level - Location = %px",
            &(MPI_Finalize));

  inc_pnmpi_mpi_level();
  if (NOT_ACTIVATED(MPI_Finalize_ID))
    {
#ifdef COMPILE_FOR_FORTRAN
      if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_FORTRAN)
        pmpi_finalize_(&err);
      else
#endif
        err = PMPI_Finalize();
    }
  else
    {
      err = Internal_XMPI_Finalize();
    }
  dec_pnmpi_mpi_level();

  /* Finalize PnMPI. This will unload the configuration and the defined modules
   * and frees allocated memory.
   *
   * Note: Duplicated calls to the finalization function will be ignored, so
   *       this doesn't need to be checked. */
  pnmpi_finalize();


  /* Exit the reentry-guarded wrapper section, reset the return and function
   * addresses of the MPI call and return the status of the MPI finalization. */
  pnmpi_reentry_exit();
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
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
#ifdef COMPILE_FOR_FORTRAN
  if (pnmpi_get_mpi_interface() == PNMPI_INTERFACE_FORTRAN)
    pmpi_finalize_(&res);
  else
#endif
    res = PMPI_Finalize();
  DBGPRINT3("Done with original MPI in MPI_Finalize");
  pnmpi_level = start_level;
  return res;
}

#ifdef COMPILE_FOR_FORTRAN
void mpi_finalize_(int *ierr)
{
  /* Set the MPI interface language used to call MPI finalization, so
   * MPI_Finalize and NQJ_Finalize know which MPI interface to finalize. */
  pnmpi_set_mpi_interface_fortran();

  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Finalize);

  *ierr = MPI_Finalize();

  /* Reset the MPI interface used for this MPI call, the return and function
   * address to the default ones. */
  pnmpi_reset_mpi_interface();
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
}

void MPI_FINALIZE(int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_finalize_(ierr);
}

void mpi_finalize(int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_finalize_(ierr);
}

void mpi_finalize__(int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_finalize_(ierr);
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
  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Pcontrol);

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
                {
                  /* Reset the return and function addresses to the default ones
                   * and return the status code. */
                  pnmpi_reset_return_address();
                  pnmpi_reset_function_address();
                  return ret;
                }
            }
        }
      pnmpi_level = curr_pnmpi_level;

      /* Reset the return and function addresses to the default ones and return
       * with a success status code. */
      pnmpi_reset_return_address();
      pnmpi_reset_function_address();
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
                {
                  /* Reset the return and function addresses to the default ones
                   * and return the status code. */
                  pnmpi_reset_return_address();
                  pnmpi_reset_function_address();
                  return ret;
                }
            }
        }
      pnmpi_level = curr_pnmpi_level;

      /* Reset the return and function addresses to the default ones and return
       * with a success status code. */
      pnmpi_reset_return_address();
      pnmpi_reset_function_address();
      return MPI_SUCCESS;
    }


  if (modules.pcontrol == PNMPI_PCONTROL_OFF)
    {
      /* Reset the return and function addresses to the default ones and return
       * with a success status code. */
      pnmpi_reset_return_address();
      pnmpi_reset_function_address();
      return MPI_SUCCESS;
    }

  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) &&
      (level != PNMPI_PCONTROL_LEVEL))
    {
      /* Reset the return and function addresses to the default ones and return
       * with an error status code. */
      pnmpi_reset_return_address();
      pnmpi_reset_function_address();
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

      /* Reset the return and function addresses to the default ones and return
       * with an error status code. */
      pnmpi_reset_return_address();
      pnmpi_reset_function_address();
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

      /* Reset the return and function addresses to the default ones and return
       * with an error status code. */
      pnmpi_reset_return_address();
      pnmpi_reset_function_address();
      return error;
    }

  /* Reset the return and function addresses to the default ones. */
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
}


#ifdef COMPILE_FOR_FORTRAN
void mpi_pcontrol_(int *level, int *ierr)
{
  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Pcontrol);

  int i, ret;

  DBGPRINT3("Entering Old Fortran MPI_Pcontrol at base level");

  if (modules.pcontrol == PNMPI_PCONTROL_OFF)
    {
      *ierr = MPI_SUCCESS;
      goto fin;
    }
  if ((modules.pcontrol == PNMPI_PCONTROL_PNMPI) ||
      (modules.pcontrol == PNMPI_PCONTROL_TYPED) ||
      ((modules.pcontrol == PNMPI_PCONTROL_MIXED) &&
       (*level == PNMPI_PCONTROL_LEVEL)))
    {
      /* can't do that in Fortran */
      *ierr = PNMPI_ERROR;
      goto fin;
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

fin:
  /* Reset the return and function addresses to the default ones. */
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
}

void MPI_PCONTROL(int *level, int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_pcontrol_(level, ierr);
}

void mpi_pcontrol(int *level, int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_pcontrol_(level, ierr);
}

void mpi_pcontrol__(int *level, int *ierr)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  mpi_pcontrol_(level, ierr);
}

/*-------------------------------------------------------------------*/
/* special case for FORTRAN timing routines */

double mpi_wtick_(void)
{
  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Wtick);

  DBGPRINT3("Entering Old Fortran mpi_wtick_ at base level - Location = %px",
            &(MPI_Wtick));

  double ret;
  if (NOT_ACTIVATED(MPI_Wtick_ID))
    ret = PMPI_Wtick();
  else
    ret = Internal_XMPI_Wtick();

  /* Reset the return and function addresses to the default ones and return the
   * value returned from PMPI_Wtick. */
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
  return ret;
}

double MPI_WTICK(void)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  return mpi_wtick_();
}

double mpi_wtick(void)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  return mpi_wtick_();
}

double mpi_wtick__(void)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  return mpi_wtick_();
}


double mpi_wtime_(void)
{
  /* Store the return address to the application, and the address of the
   * original MPI call, so modules may check the origin of this MPI call. */
  pnmpi_set_return_address();
  pnmpi_set_function_address(PMPI_Wtime);

  DBGPRINT3("Entering Old Fortran mpi_wtime_ at base level - Location = %px",
            &(MPI_Wtime));

  double ret;
  if (NOT_ACTIVATED(MPI_Wtime_ID))
    ret = PMPI_Wtime();
  else
    ret = Internal_XMPI_Wtime();

  /* Reset the return and function addresses to the default ones and return the
   * value returned from PMPI_Wtime. */
  pnmpi_reset_return_address();
  pnmpi_reset_function_address();
  return ret;
}

double MPI_WTIME(void)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  return mpi_wtime_();
}

double mpi_wtime(void)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  return mpi_wtime_();
}

double mpi_wtime__(void)
{
  /* Store the return address to the application, so modules may check the
   * origin of this MPI call.
   *
   * NOTE: Although this function is just an alias to another symbol, the return
   *       address needs to be stored here, otherwise a pointer to this function
   *       instead of the callee will be saved in the aliased function. However,
   *       as the aliased function resets the return address, this doesn't need
   *       to be done in this function anymore. */
  pnmpi_set_return_address();

  return mpi_wtime_();
}

#endif

/*-------------------------------------------------------------------*/
/* The End. */

#ifdef COMPILE_FOR_FORTRAN
#include "wrapper_cf.c"
#else
#include "wrapper_c.c"
#endif
