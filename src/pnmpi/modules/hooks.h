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
 * \note If the environment is unable to call any constructor before `main`,
 *  this hook will be called in the `MPI_Init` or `MPI_Init_thread` wrapper just
 *  after the MPI environment has been initialized, but before any modules will
 *  be called.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_AppStartup();

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
 * \note If the environment is unable to call any destructors after `main`,
 *  this hook will be called in the `MPI_Finalize` wrapper just before the MPI
 *  environment will be destroyed, but after all modules have been called.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_AppShutdown();


#ifdef __cplusplus
}
#endif


#endif
