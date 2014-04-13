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

#include "timelapse.h"
#include <mpi.h>
#include <pnmpimod.h>
#include <stdio.h>
#include <stdlib.h>



/*--------------------------------------------------------------------------*/
/* globals */

/*........................................................................*/
/* Variables */

double _timelapse_correct = 0.0;
double _timelapse_off;
int _timelapse_status = 0;


/*--------------------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;


  /* register this module and its services */

  err = PNMPI_Service_RegisterModule(PNMPI_MODULE_TIMELAPSE);
  if (err != PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(service.name, "timelapse");
  service.fct = (PNMPI_Service_Fct_t)PNMPIMOD_Timelapse_Switch;
  sprintf(service.sig, "i");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  _timelapse_off = PMPI_Wtime();

  return err;
}


/*.......................................................*/
/* Init */

int MPI_Init(int *argc, char ***argv)
{
  int err;

  err = PMPI_Init(argc, argv);

  /* initial state is off */

  return err;
}


/*------------------------------------------------------------*/
/* External service routines */
/* Comment: this routine is NOT thread safe, since it can
   only be called from MPI_Init - hence there should not be
   more than one thread calling it anyway */

int PNMPIMOD_Timelapse_Switch(int onoff)
{
  if (onoff != _timelapse_status)
    {
      if (onoff)
        {
          double diff;
          /* turned on now */
          diff = PMPI_Wtime_NewStack(PNMPI_MAX_MOD) - _timelapse_off;
          _timelapse_correct += diff;
        }
      else
        {
          /* turned off now */
          _timelapse_off = PMPI_Wtime();
        }
      _timelapse_status = onoff;
    }

  return PNMPI_SUCCESS;
}


/*--------------------------------------------------------------------------*/
/* Wrapper Routines */

double MPI_Wtime()
{
  if (_timelapse_status)
    return PMPI_Wtime() - _timelapse_correct;
  else
    return _timelapse_off;
}
