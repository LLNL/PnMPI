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

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/hooks.h>
#include <pnmpi/newstack.h>
#include <pnmpi/service.h>


MPI_Comm Reserve_Smaller_Comm = MPI_COMM_WORLD;

#define MACRO_MPI_Comm(_c) \
{ int _size; \
  PMPI_Comm_size(_c,&_size); \
  if (_size==select_size_A) _stack=select_stack_A; \
  if (_size==select_size_B) _stack=select_stack_B; }

int select_size_A;
int select_size_B;
PNMPI_modHandle_t select_stack_A,select_stack_B;

/* automatically generated wrapper code */
{{fnall fn_name MPI_Init MPI_Pcontrol MPI_Finalize}} {
  int _stack=-1;
  {{apply_to_type MPI_Comm MACRO_MPI_Comm}}
  WRAP_MPI_CALL_PREFIX
  {{ret_val}} = P{{fn_name}}_NewStack({{list _stack {{args}} }});
  WRAP_MPI_CALL_POSTFIX
}{{endfnall}}

/*--------------------------------------------------------------------------*/
/* Setup */

#define PNMPI_SWITCH_MATRIX "switch-matrix"


/*.......................................................*/
/* Registration */

void PNMPI_RegistrationPoint()
{
  PNMPI_Service_RegisterModule(PNMPI_SWITCH_MATRIX);
}


/*.......................................................*/
/* Init */

int  MPI_Init(int * argc, char *** argv)
{
  int err;
  const char *inp;
  int handle_comm;

  /* query own module */

  err=PNMPI_Service_GetModuleByName(PNMPI_SWITCH_MATRIX,&handle_comm);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetArgument(handle_comm,"commA",&inp);
  if (err!=PNMPI_SUCCESS)
    return err;
  select_size_A=atoi(inp);

  err=PNMPI_Service_GetArgument(handle_comm,"commB",&inp);
  if (err!=PNMPI_SUCCESS)
    return err;
  select_size_B=atoi(inp);

  err=PNMPI_Service_GetArgument(handle_comm,"stackA",&inp);
  if (err!=PNMPI_SUCCESS)
    return err;
  err=PNMPI_Service_GetStackByName(inp,&select_stack_A);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetArgument(handle_comm,"stackB",&inp);
  if (err!=PNMPI_SUCCESS)
    return err;
  err=PNMPI_Service_GetStackByName(inp,&select_stack_B);
  if (err!=PNMPI_SUCCESS)
    return err;


  /* run init */

  printf("Running main Init\n");
  err=PMPI_Init(argc, argv);
  printf("Running Init A\n");
  err=PMPI_Init_NewStack(select_stack_A,argc, argv);
  printf("Running Init B\n");
  err=PMPI_Init_NewStack(select_stack_B,argc, argv);
  printf("Init done\n");

  return err;
}

int MPI_Finalize()
{
  int err;

  printf("Running main Finalize\n");
  err=PMPI_Finalize();
  printf("Running Finalize A\n");
  err=PMPI_Finalize_NewStack(select_stack_A);
  printf("Running Finalize B\n");
  err=PMPI_Finalize_NewStack(select_stack_B);
  printf("Finalize done\n");

  return err;
}

