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

#ifndef PNMPI_MODULE_H
#define PNMPI_MODULE_H


/* The PnMPI API should be C++ compatible, too. We have to add the extern "C"
 * stanza to avoid name mangeling. Otherwise PnMPI modules would not find PnMPI
 * API functions. */
#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup pnmpi_module_hooks PnMPI module hooks.
 *
 * /details Module hooks are called at different steps in the PnMPI init- and
 *  finalization.
 */

/** \brief Called just after the module has been loaded.
 *
 * \details This hook will be called just after the module has been loaded. It
 *  may be used to register the name of the module, services provided by the
 *  module, etc.
 */
void PNMPI_RegistrationPoint();

/** \brief Called after all modules have been registered.
 *
 * \details As \ref PNMPI_RegistrationPoint this hook is called after the module
 *  has been loaded, but after all modules have registered itself. Modules may
 *  interact with other modules in this hook.
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
 */
void app_startup();

/** \brief Called just after the applications main has finished.
 *
 * \details This hook is the counterpart of \ref app_startup. It will be called
 *  just after the applications main has finished and may be used to e.g. print
 *  a message or stop timers.
 *
 * \note If any loaded module supports this hook, the MPI environment will not
 *  be destoryed in MPI_Finalize but kept open until calls to this hook have
 *  been finished. This may be used to e.g. broadcast messages.
 */
void app_shutdown();


#ifdef __cplusplus
}
#endif


#endif
