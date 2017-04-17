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

#ifndef PNMPI_MODULE_H
#define PNMPI_MODULE_H


/* The PnMPI API should be C++ compatible, too. We have to add the extern "C"
 * stanza to avoid name mangeling. Otherwise PnMPI modules would not find PnMPI
 * API functions. */
#ifdef __cplusplus
extern "C" {
#endif


/** \class pnmpi_module_hooks pnmpi/hooks.h
 *
 * \brief PnMPI module hooks.
 *
 * \details Module hooks are called at different steps in the PnMPI init- and
 *  finalization.
 */


/** \brief Limit the threading level the module supports.
 *
 * \details Usually it is assumed, that a module is thread safe. However if it
 *  is not, you may set this variable to the highest MPI threading level your
 *  module supports. PnMPI will ensure, that MPI_Init_thread will be called with
 *  the highest available thread level, all modules support.
 *
 *
 * \memberof pnmpi_module_hooks
 */
extern const int PNMPI_SupportedThreadingLevel;

/** \brief Name of the module.
 *
 * \details Instead of registering the module name in \ref
 *  PNMPI_RegistrationPoint, a module may define this symbol to register its
 *  name.
 *
 *
 * \memberof pnmpi_module_hooks
 */
extern const char *PNMPI_ModuleName;

/** \brief Called just after the module has been loaded.
 *
 * \details This hook will be called just after the module has been loaded. It
 *  may be used to register the name of the module, services provided by the
 *  module, etc.
 *
 * \note MPI is not initialized when this hook is called. You MUST NOT use any
 *  MPI routines (except MPI_Initialized and MPI_Finalized) in your hook.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_RegistrationPoint();

/** \brief Called after all modules have been registered.
 *
 * \details As \ref PNMPI_RegistrationPoint this hook is called after the module
 *  has been loaded, but after all modules have registered itself. Modules may
 *  interact with other modules in this hook.
 *
 * \note MPI is not initialized when this hook is called. You MUST NOT use any
 *  MPI routines (except MPI_Initialized and MPI_Finalized) in your hook.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_RegistrationComplete();

/** \brief Called just before the applications main is started.
 *
 * \details This hook will be called just before the applications main is
 *  started and may be used to e.g. print a message or start a timer for the
 *  runtime of main.
 *
 * \note If any loaded module supports this hook, the MPI environment will be
 *  setup before calling this hook, so one may use MPI calls to e.g. broadcast
 *  messages.
 *
 * \warning If none of PnMPIs constructors has been called before `MPI_Init` or
 *  `MPI_Init_thread`, PnMPI will abort the execution. If it's not really
 *  necessary to execute code before `main` but only a nice-to-have feature of
 *  your module, consider using \ref PNMPI_AppStartupOptional instead.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_AppStartup();

/** \brief Optional version of \ref PNMPI_AppStartup.
 *
 * \details This hook has the same functionality as \ref PNMPI_AppStartup.
 *  However, if the environment is unable to call any constructor before `main`,
 *  this hook will be called in the `MPI_Init` or `MPI_Init_thread` wrapper just
 *  after the MPI environment has been initialized, but before any modules will
 *  be called.
 *
 * \note This hook will be called after \ref PNMPI_AppStartup.
 *
 * \warning Use this hook only, if the execution of your code before `main` is
 *  not mandatory, but only should happen as early as possible depending on the
 *  environment.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_AppStartupOptional();

/** \brief Called just after the applications main has finished.
 *
 * \details This hook is the counterpart of \ref PNMPI_AppStartup. It will be
 *  called just after the applications main has finished and may be used to e.g.
 *  print a message or stop timers.
 *
 * \note If any loaded module supports this hook, the MPI environment will not
 *  be destoryed in MPI_Finalize but kept open until calls to this hook have
 *  been finished. This may be used to e.g. broadcast messages.
 *
 * \warning If PnMPI can't call any destructor, PnMPI will abort the execution
 *  in the `MPI_Init` wrapper. If it's not really necessary to execute code
 *  after `main` but only a nice-to-have feature of your module, consider using
 *  \ref PNMPI_AppShutdownOptional instead.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_AppShutdown();

/** \brief Optional version of \ref PNMPI_AppShutdown.
 *
 * \details This hook has the same functionality as \ref PNMPI_AppShutdown.
 *  However, if the environment is unable to call any destructors after `main`,
 *  this hook will be called in the `MPI_Finalize` wrapper just before the MPI
 *  environment will be destroyed, but after all modules have been called.
 *
 * \note This hook will be called before \ref PNMPI_AppShutdown.
 *
 * \warning Use this hook only, if the execution of your code after `main` is
 *  not mandatory, but only should happen as late as possible depending on the
 *  environment.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_AppShutdownOptional();


#ifdef __cplusplus
}
#endif


#endif
