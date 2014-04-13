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

/*===============================================================*/
/* P^N MPI                                                       */
/* Service header file                                           */
/* This file needs to be included by any module that explicitly  */
/* is designed to work with P^N MPI and uses its extra services  */
/*===============================================================*/

#ifndef _PNMPIMOD_HEADER
#define _PNMPIMOD_HEADER

#include "pnmpi.h"
#include <mpi.h>


/*===============================================================*/
/* Error codes */

#define PNMPI_SUCCESS 0
#define PNMPI_NOT_INIT -1
#define PNMPI_NOMEM -2
#define PNMPI_NOMODULE -3
#define PNMPI_NOSERVICE -4
#define PNMPI_NOGLOBAL -5
#define PNMPI_SIGNATURE -6
#define PNMPI_NOARG -7

#define MPI_ERROR_MEM MPI_ERR_INTERN
#define MPI_ERROR_PNMPI MPI_ERR_INTERN


/*===============================================================*/
/* Services available for cross module communication */

/*------------------------------------------------------------*/
/* Register modules and services */

/*.......................................................*/
/* Constants */

#define PNMPI_SERVICE_NAMELEN 30
#define PNMPI_MODULE_FILENAMELEN 256
#define PNMPI_SERVICE_SIGLEN 20
#define PNMPI_MODULE_USERNAMELEN 100

#define PNMPI_REGISTRATION_POINT "PNMPI_RegistrationPoint"

#define PNMPI_MAX_MOD 100000

/*.......................................................*/
/* Types */

typedef int PNMPI_modHandle_t;

typedef int (*PNMPI_Service_Fct_t)();
typedef int (*PNMPI_RegistrationPoint_t)(void);

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

int PNMPI_RegistrationPoint(void);

int PNMPI_Service_RegisterModule(const char *name);
int PNMPI_Service_RegisterService(const PNMPI_Service_descriptor_t *service);
int PNMPI_Service_RegisterGlobal(const PNMPI_Global_descriptor_t *global);

int PNMPI_Service_GetModuleByName(const char *name, PNMPI_modHandle_t *handle);

int PNMPI_Service_GetModuleSelf(PNMPI_modHandle_t *handle);

int PNMPI_Service_GetStackByName(const char *name, PNMPI_modHandle_t *handle);

int PNMPI_Service_GetServiceByName(PNMPI_modHandle_t handle, const char *name,
                                   const char *sig,
                                   PNMPI_Service_descriptor_t *serv);

int PNMPI_Service_GetGlobalByName(PNMPI_modHandle_t handle, const char *name,
                                  const char sig,
                                  PNMPI_Global_descriptor_t *serv);

int PNMPI_Service_GetArgument(PNMPI_modHandle_t handle, const char *name,
                              const char **val);


/*===============================================================*/
/* Prototypes for PMPI routines on alternative stacks */

#include "newstack.h"


/*===============================================================*/
/* Serives available for external communication */

/*------------------------------------------------------------*/
/* Log funtionality into global log */

/*..........................................................*/
/* Log a string */
/* Write a string to a global log

   IN: parameters as in printf
   OUT: -
*/

void PNMPI_Service_log(char *format, ...);


/*===============================================================*/
/* Services available for module management */

/*------------------------------------------------------------*/
/* Query functionality */

/*..........................................................*/
/* Query one specific module */
/* Ask if a given module has been loaded into the PMPI stack

   IN:  name = string identifying the module
   IN:  pointer to location info, afterwards:
        <0 : module found and higher in the stack (no access)
         0 : own module
        >0 : module found and lower in the stack (accessible)
   OUT:  0 : successful, location info valid
        -1 : module not found
*/


int PNMPI_Service_QueryModule(char *name, int *locinfo);


/*------------------------------------------------------------*/
/* Load functionality */


/*===============================================================*/
/* The End. */

#ifdef __cplusplus
}
#endif

#endif /* _PNMPIMOD_HEADER */
