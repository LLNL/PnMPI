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

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pnmpimod.h>
#include "status.h"

#ifdef AIX
#pragma alloca
#endif

/*--------------------------------------------------------------------------*/
/* globals */

/*........................................................................*/
/* Variables */

int add_status_storage = 0;


/*........................................................................*/
/* macros */

#define ALLOCATE_STATUS(newstatus,c) \
   MPI_Status *newstatus=(MPI_Status*) alloca(c*(add_status_storage+sizeof(MPI_Status)));\
   { int _i; for (_i=0; _i<c; _i++) {  newstatus[_i].MPI_ERROR=PNMPIMOD_STATUS_TAG; } }

#define COPY_STATUS(oldstatus,newstatus,c) \
  { if (oldstatus!=MPI_STATUS_IGNORE) { int __i; for (__i=0; __i<c; __i++) oldstatus[__i]=newstatus[__i]; }}


/*--------------------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;
  PNMPI_Global_descriptor_t global;


  /* reset variables */

  add_status_storage = -1;

  /* register this module and its services */

  err=PNMPI_Service_RegisterModule(PNMPI_MODULE_STATUS);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(service.name,"add-storage");
  service.fct=(PNMPI_Service_Fct_t) PNMPIMOD_Status_RequestStorage;
  sprintf(service.sig,"i");
  err=PNMPI_Service_RegisterService(&service);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(global.name,"total-status-extension");
  global.addr.i=&add_status_storage;
  global.sig='i';
  err=PNMPI_Service_RegisterGlobal(&global);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  return err;
}


/*.......................................................*/
/* Init */

int MPI_Init(int *argc, char ***argv)
{
  int err;

  /* call the init routines */

  err=PMPI_Init(argc,argv);

  /* when this returns, all modules have registered their
     needs for extra storage - this number is now fixed */

  if (add_status_storage<=0)
    {
      /* No module requested extra space,
	 we should really disable this module, but
	 for now we just ignore it */

      add_status_storage=0;
    }

  return err;
}


/*------------------------------------------------------------*/
/* External service routines */
/* Comment: this routine is NOT thread safe, since it can
   only be called from MPI_Init - hence there should not be
   more than one thread calling it anyway */

int PNMPIMOD_Status_RequestStorage(int size)
{
  int ret;

  if (add_status_storage<0)
    add_status_storage=0;

  ret=add_status_storage;
  add_status_storage+=size;

  return ret;
}


/*--------------------------------------------------------------------------*/
/* Wrapper Routines */

/*........................................................................*/
/* Evaluate Status */

int MPI_Get_count(PNMPI_CONST MPI_Status *status, MPI_Datatype datatype, int *count)
{
  int err;
  err=PMPI_Get_count(status, datatype, count);
  return err;
}

int MPI_Get_elements(PNMPI_CONST MPI_Status *status, MPI_Datatype datatype, int *count)
{
  int err;
  err=PMPI_Get_elements(status, datatype, count);
  return err;
}


/*........................................................................*/
/* Sync. Receive */

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Recv(buf,count,datatype,source,tag,comm,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}


/*.......................................................*/
/* Irecv */

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, 
		    int source, int tag, MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err=PMPI_Irecv(buf,count,datatype,source,tag,comm,request);
  return err;
}


/*........................................................................*/
/* Send/Recv */

int MPI_Sendrecv(PNMPI_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, 
		 void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag,
		 MPI_Comm comm, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Sendrecv(sendbuf,sendcount,sendtype,dest,sendtag,
		    recvbuf,recvcount,recvtype,source,recvtag,comm,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}

int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, 
			 MPI_Comm comm, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Sendrecv_replace(buf,count,datatype,dest,sendtag,source,recvtag,comm,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}


/*........................................................................*/
/* Probes */

int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Iprobe(source,tag,comm,flag,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}

int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Probe(source,tag,comm,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}


/*........................................................................*/
/* Waits */

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Wait(request,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}

int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Waitany(count,array_of_requests,index,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}

int MPI_Waitsome(int count, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err;
  ALLOCATE_STATUS(array_of_newstatuses,count)
  err=PMPI_Waitsome(count,array_of_requests,outcount,array_of_indices,array_of_newstatuses);
  COPY_STATUS(array_of_statuses,array_of_newstatuses,*outcount)
  return err;  
}

int MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)
{
  int err;
  ALLOCATE_STATUS(array_of_newstatuses,count)
  err=PMPI_Waitall(count,array_of_requests,array_of_newstatuses);
  COPY_STATUS(array_of_statuses,array_of_newstatuses,count)
  return err;
}


/*........................................................................*/
/* Tests */

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Test(request, flag, newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;
}

int MPI_Testany(int count, MPI_Request *array_of_requests, int *index, int *flag, MPI_Status *status)
{
  int err;
  ALLOCATE_STATUS(newstatus,1)
  err=PMPI_Testany(count,array_of_requests,index,flag,newstatus);
  COPY_STATUS(status,newstatus,1)
  return err;  
}

int MPI_Testsome(int incount, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err;
  ALLOCATE_STATUS(array_of_newstatuses,incount)
  err=PMPI_Testsome(incount,array_of_requests,outcount,array_of_indices,array_of_newstatuses);
  COPY_STATUS(array_of_statuses,array_of_newstatuses,*outcount)
  return err;  
}

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag, MPI_Status *array_of_statuses)
{
  int err;
  ALLOCATE_STATUS(array_of_newstatuses,count)
  err=PMPI_Testall(count, array_of_requests, flag, array_of_newstatuses);
  COPY_STATUS(array_of_statuses,array_of_newstatuses,count)
  return err;
}


