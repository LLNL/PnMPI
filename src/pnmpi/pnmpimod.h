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

#ifndef _PNMPIMOD_HEADER
#define _PNMPIMOD_HEADER

#include <pnmpi/service.h>


/*===============================================================*/
/* P^N MPI                                                       */
/* Service header file                                           */
/* This file needs to be included by any module that explicitly  */
/* is designed to work with P^N MPI and uses its extra services  */
/*===============================================================*/

#include "newstack.h"
#include "pnmpi-config.h"
#include "pnmpi.h"
#include "xmpi.h"
#include <mpi.h>

/*===============================================================*/
/* Error codes */

#define MPI_ERROR_MEM MPI_ERR_INTERN
#define MPI_ERROR_PNMPI MPI_ERR_INTERN


/*===============================================================*/
/* Services available for cross module communication */

/*------------------------------------------------------------*/
/* Register modules and services */

/*.......................................................*/
/* Constants */

#define PNMPI_SERVICE_NAMELEN 30
#define PNMPI_MODULE_FILENAMELEN 512
#define PNMPI_SERVICE_SIGLEN 20
#define PNMPI_MODULE_USERNAMELEN 256

#define PNMPI_REGISTRATION_POINT "PNMPI_RegistrationPoint"
#define PNMPI_INITCOMPLETE_POINT "PNMPI_RegistrationComplete"

#define PNMPI_MAX_MOD 100000

#define PNMPI_MODHANDLE_NULL -1

/*.......................................................*/
/* Types */

typedef int (*PNMPI_Service_Fct_t)();

typedef struct PNMPI_Service_descriptor_d
{
  char name[PNMPI_SERVICE_NAMELEN];
  char sig[PNMPI_SERVICE_SIGLEN];
  PNMPI_Service_Fct_t fct;
} PNMPI_Service_descriptor_t;

typedef union PNMPI_Global_Addr_d
{
  char *c;
  short *s;
  int *i;
  unsigned int *u;
  long *l;
  double *d;
  float *f;
  void **p;
} PNMPI_Global_Addr_t;

typedef struct PNMPI_Global_descriptor_d
{
  char name[PNMPI_SERVICE_NAMELEN];
  char sig;
  PNMPI_Global_Addr_t addr;
} PNMPI_Global_descriptor_t;


/*.......................................................*/
/* Functions */

#ifdef __cplusplus
extern "C" {
#endif


int PNMPI_Service_RegisterService(const PNMPI_Service_descriptor_t *service);
int PNMPI_Service_RegisterGlobal(const PNMPI_Global_descriptor_t *global);

int PNMPI_Service_GetServiceByName(PNMPI_modHandle_t handle, const char *name,
                                   const char *sig,
                                   PNMPI_Service_descriptor_t *serv);

int PNMPI_Service_GetGlobalByName(PNMPI_modHandle_t handle, const char *name,
                                  const char sig,
                                  PNMPI_Global_descriptor_t *serv);


#ifdef __cplusplus
}
#endif

#endif /* _PNMPIMOD_HEADER */
