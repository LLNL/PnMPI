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

#ifndef PNMPI_MODULE_H
#define PNMPI_MODULE_H


/* The PnMPI API should be C++ compatible, too. We have to add the extern "C"
 * stanza to avoid name mangling. Otherwise PnMPI modules would not find PnMPI
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
void PNMPI_RegistrationPoint(void);

/** \brief Called just before the module will be unloaded.
 *
 * \details This hook will be called just before the module will be unloaded. It
 *  may be used to free allocated memory.
 *
 * \note MPI is not initialized anymore when this hook is called. You MUST NOT
 *  use any MPI routines (except MPI_Initialized and MPI_Finalized) in your
 *  hook.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_UnregistrationPoint(void);

/** \brief Called just after the modules have been registered.
 *
 * \details This hook will be called just after all modules have been registered
 *  to initialize the module. It may be used to initialize the module's code and
 *  to communicate with other modules.
 *
 * \note MPI is not initialized when this hook is called. You MUST NOT use any
 *  MPI routines (except MPI_Initialized and MPI_Finalized) in your hook.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_Init(void);

/** \brief Called just before the module will be unregistered.
 *
 * \details This hook will be called just before all modules will be
 *  unregistered. The modules may communicate with each other and should execute
 *  some final steps in here.
 *
 * \note MPI is not initialized anymore when this hook is called. You MUST NOT
 *  use any MPI routines (except MPI_Initialized and MPI_Finalized) in your
 *  hook.
 *
 *
 * \memberof pnmpi_module_hooks
 */
void PNMPI_Fini(void);


#ifdef __cplusplus
}
#endif


#endif
