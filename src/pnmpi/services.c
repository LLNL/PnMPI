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

/*===============================================================*/
/* P^N MPI                                                       */
/* PNMPI Core Services                                           */
/*===============================================================*/

#include "core.h"
#include "pnmpimod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*------------------------------------------------------------*/
/* Services available for cross module communication */

/*........................................................*/
/* Register a service */

int PNMPI_Service_RegisterService(const PNMPI_Service_descriptor_t *service)
{
  module_servlist_p newservice;

  newservice = (module_servlist_p)malloc(sizeof(module_servlist_t));
  if (newservice == NULL)
    return PNMPI_NOMEM;

  newservice->desc = *service;
  newservice->next = modules.module[pnmpi_level]->services;
  modules.module[pnmpi_level]->services = newservice;
  return PNMPI_SUCCESS;
}


/*........................................................*/
/* Register a service */

int PNMPI_Service_RegisterGlobal(const PNMPI_Global_descriptor_t *global)
{
  module_globlist_p newglobal;

  newglobal = (module_globlist_p)malloc(sizeof(module_globlist_t));
  if (newglobal == NULL)
    return PNMPI_NOMEM;

  newglobal->desc = *global;
  newglobal->next = modules.module[pnmpi_level]->globals;
  modules.module[pnmpi_level]->globals = newglobal;
  return PNMPI_SUCCESS;
}


/*........................................................*/
/* find a registered module using a given user name */

int PNMPI_Service_GetStackByName(const char *name, PNMPI_modHandle_t *handle)
{
  int i;

  for (i = 0; i < modules.num; i++)
    {
      if (modules.module[i]->stack_delimiter)
        {
          if (strcmp(modules.module[i]->name, name) == 0)
            {
              /* module found */

              *handle = i + 1;
              return PNMPI_SUCCESS;
            }
        }
    }
  return PNMPI_NOMODULE;
}


/*........................................................*/
/* get handle for the own modues */

int PNMPI_Service_GetModuleSelf(PNMPI_modHandle_t *handle)
{
  *handle = pnmpi_level;
  return PNMPI_SUCCESS;
}


/*........................................................*/
/* find a registered module using a given user name */

int PNMPI_Service_GetModuleByName(const char *name, PNMPI_modHandle_t *handle)
{
  int i;

  for (i = 0; i < modules.num; i++)
    {
      if (modules.module[i]->registered)
        {
          if (strcmp(modules.module[i]->username, name) == 0)
            {
              /* module found */

              *handle = i;
              return PNMPI_SUCCESS;
            }
        }
    }
  return PNMPI_NOMODULE;
}


/*........................................................*/
/* find a service in a given module and return description */

int PNMPI_Service_GetServiceByName(PNMPI_modHandle_t handle, const char *name,
                                   const char *sig,
                                   PNMPI_Service_descriptor_t *serv)
{
  int err;
  module_servlist_p s;

  err = PNMPI_NOSERVICE;
  for (s = modules.module[handle]->services; s != NULL; s = s->next)
    {
      if (strcmp(s->desc.name, name) == 0)
        {
          /* name match, chech sig */

          if (strcmp(s->desc.sig, sig) == 0)
            {
              /* found service */

              *serv = s->desc;
              return PNMPI_SUCCESS;
            }
          else
            err = PNMPI_SIGNATURE;
        }
    }

  return err;
}


/*........................................................*/
/* find a service in a given module and return description */

int PNMPI_Service_GetGlobalByName(PNMPI_modHandle_t handle, const char *name,
                                  const char sig,
                                  PNMPI_Global_descriptor_t *glob)
{
  int err;
  module_globlist_p g;

  err = PNMPI_NOGLOBAL;
  for (g = modules.module[handle]->globals; g != NULL; g = g->next)
    {
      if (strcmp(g->desc.name, name) == 0)
        {
          /* name match, chech sig */

          if (g->desc.sig == sig)
            {
              /* found service */

              *glob = g->desc;
              return PNMPI_SUCCESS;
            }
          else
            err = PNMPI_SIGNATURE;
        }
    }

  return err;
}


/*........................................................*/
/* query an argument */

int PNMPI_Service_GetArgument(PNMPI_modHandle_t handle, const char *name,
                              const char **val)
{
  module_arg_p a;

  for (a = modules.module[handle]->args; a != NULL; a = a->next)
    {
      if (strcmp(a->name, name) == 0)
        {
          /* name match, chech sig */

          *val = a->value;
          return PNMPI_SUCCESS;
        }
    }

  return PNMPI_NOARG;
}


/*------------------------------------------------------------*/
/* The End. */
