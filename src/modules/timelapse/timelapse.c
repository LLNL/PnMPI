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

#include "timelapse.h"

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>
#include <pnmpi/xmpi.h>


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

void PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;


  /* register this module and its services */

  err = PNMPI_Service_RegisterModule(PNMPI_MODULE_TIMELAPSE);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(service.name, "timelapse");
  service.fct = (PNMPI_Service_Fct_t)PNMPIMOD_Timelapse_Switch;
  sprintf(service.sig, "i");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return;

  _timelapse_off = PMPI_Wtime();
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
          diff = PMPI_Wtime() - _timelapse_off;
          _timelapse_correct += diff;
        }
      else
        {
          /* turned off now */
          _timelapse_off = XMPI_Wtime();
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
    return XMPI_Wtime() - _timelapse_correct;
  else
    return _timelapse_off;
}
