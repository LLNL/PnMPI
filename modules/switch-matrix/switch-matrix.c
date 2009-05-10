#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pnmpimod.h>

MPI_Comm Reserve_Smaller_Comm = MPI_COMM_WORLD;

#define MACRO_MPI_Comm(_c) \
{ int _size; \
  PMPI_Comm_size(_c,&_size); \
  if (_size==select_size_A) _stack=select_stack_A; \
  if (_size==select_size_B) _stack=select_stack_B; } 

int select_size_A;
int select_size_B;
PNMPI_modHandle_t select_stack_A,select_stack_B;

#include "switch-matrix.h"


/*--------------------------------------------------------------------------*/
/* Setup */

#define PNMPI_SWITCH_MATRIX "switch-matrix"


/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;

  /* register this module and its services */

  err=PNMPI_Service_RegisterModule(PNMPI_SWITCH_MATRIX);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  return err;
}


/*.......................................................*/
/* Init */

int  MPI_Init(int * argc, char *** argv)
{
  int err;
  char *inp;
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

