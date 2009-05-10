#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pnmpimod.h>
#include "timelapse.h"



/*--------------------------------------------------------------------------*/
/* globals */

/*........................................................................*/
/* Variables */

double _timelapse_correct=0.0;
double _timelapse_off;
int    _timelapse_status=0;


/*--------------------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;


  /* register this module and its services */

  err=PNMPI_Service_RegisterModule(PNMPI_MODULE_TIMELAPSE);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(service.name,"timelapse");
  service.fct=(PNMPI_Service_Fct_t*) PNMPIMOD_Timelapse_Switch;
  sprintf(service.sig,"i");
  err=PNMPI_Service_RegisterService(&service);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  return err;
}


/*.......................................................*/
/* Init */

int MPI_Init(int *argc, char ***argv)
{
  int err;

  err=PMPI_Init(argc,argv);

  /* initial state is off */

  _timelapse_off=PMPI_Wtime();

  return err;
}


/*------------------------------------------------------------*/
/* External service routines */
/* Comment: this routine is NOT thread safe, since it can
   only be called from MPI_Init - hence there should not be
   more than one thread calling it anyway */

int PNMPIMOD_Timelapse_Switch(int onoff)
{
  if (onoff!=_timelapse_status)
    {
      if (onoff)
	{
	  double diff;
	  /* turned on now */
	  diff=PMPI_Wtime_NewStack(PNMPI_MAX_MOD)-_timelapse_off;
	  _timelapse_correct += diff;
	}
      else
	{
	  /* turned off now */
	  _timelapse_off = PMPI_Wtime();
	}
      _timelapse_status=onoff;
    }

  return PNMPI_SUCCESS;
}


/*--------------------------------------------------------------------------*/
/* Wrapper Routines */

double MPI_Wtime()
{
  if (_timelapse_status)
    return PMPI_Wtime()-_timelapse_correct;
  else
    return _timelapse_off;
}



