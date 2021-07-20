#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpimod.h>
#include <status.h>
#include <requests.h>
#include "pb_mod.h"

#define PBDRIVER_CHECK

#define PNMPI_MODULE_PBD "pbdriver"

#define PB_PATTERN 0x42434445

#ifdef PBDRIVER_CHECK
#ifndef PBDRIVER_SET
#define PBDRIVER_SET
#endif
#endif

#ifdef PBDRIVER_SET
#define PBSET \
  if (run_set) { \
  ((int*)pbdata)[0]=PB_PATTERN; \
  pb_set(pb_size,pbdata);}
int run_set=1;
char *pbdata;
#endif


#ifdef PBDRIVER_CHECK
#define PBCHECKARRAY(res,status,count,num) { \
  if (res==MPI_SUCCESS) \
    { if ((STATUS_STORAGE_ARRAY(status,*pb_offset,*TotalStatusExtension,int,count,num)!=PB_PATTERN) || (0) )\ 
	printf("Received wrong pattern %08x\n",\
	       STATUS_STORAGE_ARRAY(status,*pb_offset,*TotalStatusExtension,int,count,num));\
    } }
#define PBCHECK(res,status) PBCHECKARRAY(res,status,1,0)
int run_check=1;
#endif


PNMPIMOD_Piggyback_t       pb_set;
PNMPIMOD_Piggyback_Size_t  pb_setsize;
static int *pb_offset;
static int pb_size=4;
static int *TotalStatusExtension;
static int *StatusOffsetInRequest;

#define COMM_REQ_FROM_STATUS(status) REQ_FROM_STATUS(status,(*StatusOffsetInRequest))
#define COMM_REQ_FROM_STATUSARRAY(status,count,num) REQ_FROM_STATUSARRAY(status,(*StatusOffsetInRequest),*TotalStatusExtension,count,num)

/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;

  /* register this module and its services */

  err=PNMPI_Service_RegisterModule(PNMPI_MODULE_PBD);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  return err;
}


/*.......................................................*/
/* Init */


int MPI_Init(int *argc, char ***argv)
{
  int err;
  PNMPI_modHandle_t handle_pb,handle_pbd,handle_status,handle_req;
  PNMPI_Service_descriptor_t serv;
  PNMPI_Global_descriptor_t global;
  char *vlevel_s;

  /* query pb module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_PB,&handle_pb);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetServiceByName(handle_pb,"piggyback","ip",&serv);
  if (err!=PNMPI_SUCCESS)
    return err;
  pb_set=(PNMPIMOD_Piggyback_t) ((void*)serv.fct);

  err=PNMPI_Service_GetServiceByName(handle_pb,"piggyback-size","i",&serv);
  if (err!=PNMPI_SUCCESS)
    return err;
  pb_setsize=(PNMPIMOD_Piggyback_Size_t) ((void*)serv.fct);

  err=PNMPI_Service_GetGlobalByName(handle_pb,"piggyback-offset",'i',&global);
  if (err!=PNMPI_SUCCESS)
    return err;
  pb_offset=(global.addr.i);


  /* query the status module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_STATUS,&handle_status);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetGlobalByName(handle_status,"total-status-extension",'i',&global);
  if (err!=PNMPI_SUCCESS)
    return err;
  TotalStatusExtension=(global.addr.i);


  /* query the request module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_REQUEST,&handle_req);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetGlobalByName(handle_req,"status-offset",'i',&global);
  if (err!=PNMPI_SUCCESS)
    return err;
  StatusOffsetInRequest=(global.addr.i);


  /* query own module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_PBD,&handle_pbd);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetArgument(handle_pbd,"size",&vlevel_s);
  if (err!=PNMPI_SUCCESS)
    {
      if (err==PNMPI_NOARG)
	pb_size=4;
      else
	return err;
    }
  else
    {
      pb_size=atoi(vlevel_s);
    }

  err=PNMPI_Service_GetArgument(handle_pbd,"set",&vlevel_s);
  if (err!=PNMPI_SUCCESS)
    {
      if (err==PNMPI_NOARG)
	run_set=1;
      else
	return err;
    }
  else
    {
      run_set=atoi(vlevel_s);
    }

  err=PNMPI_Service_GetArgument(handle_pbd,"check",&vlevel_s);
  if (err!=PNMPI_SUCCESS)
    {
      if (err==PNMPI_NOARG)
	run_check=1;
      else
	return err;
    }
  else
    {
      run_check=atoi(vlevel_s);
    }

  err=pb_setsize(pb_size);
  if (err!=PNMPI_SUCCESS)
    return err;

  pbdata=malloc(pb_size);
  if (pbdata==NULL) return MPI_ERROR_MEM;

  printf("Piggyback driver enabled with size %i (set %i, check %i)\n",
	 pb_size,run_set,run_check);

  return PMPI_Init(argc,argv);
}


/*.......................................................*/
/* setting PB if enabled */

#ifdef PBDRIVER_SET

int MPI_Send(void* buf, int num, MPI_Datatype dtype, int node, 
	     int tag, MPI_Comm comm)
{
  int res;
  PBSET;
  res=PMPI_Send(buf,num,dtype,node,tag,comm);
  return res;
}

int MPI_Bsend(void* buf, int num, MPI_Datatype dtype, int node, 
	     int tag, MPI_Comm comm)
{
  int res;
  PBSET;
  res=PMPI_Bsend(buf,num,dtype,node,tag,comm);
  return res;
}

int MPI_Ssend(void* buf, int num, MPI_Datatype dtype, int node, 
	     int tag, MPI_Comm comm)
{
  int res;
  PBSET;
  res=PMPI_Ssend(buf,num,dtype,node,tag,comm);
  return res;
}

int MPI_Rsend(void* buf, int num, MPI_Datatype dtype, int node, 
	     int tag, MPI_Comm comm)
{
  int res;
  PBSET;
  res=PMPI_Rsend(buf,num,dtype,node,tag,comm);
  return res;
}

int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  PBSET;
  err=PMPI_Isend(buf,count,datatype,dest,tag,comm,request);
  return err;
}

int MPI_Ibsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  PBSET;
  err=PMPI_Ibsend(buf,count,datatype,dest,tag,comm,request);
  return err;
}

int MPI_Issend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  PBSET;
  err=PMPI_Issend(buf,count,datatype,dest,tag,comm,request);
  return err;
}

int MPI_Irsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  PBSET;
  err=PMPI_Irsend(buf,count,datatype,dest,tag,comm,request);
  return err;
}

#endif


/*.......................................................*/
/* Checks if enabled */

#ifdef PBDRIVER_CHECK

int MPI_Recv(void* buf, int num, MPI_Datatype dtype, int node,
             int tag, MPI_Comm comm, MPI_Status *status)
{
  int err;
  err=PMPI_Recv(buf,num,dtype,node,tag,comm,status);
  if (run_check==0) return err;
  PBCHECK(err,status);
  return err;
}


int MPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, 
		 void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag,
		 MPI_Comm comm, MPI_Status *status)
{
  int err;
  err=PMPI_Sendrecv(sendbuf,sendcount,sendtype,dest,sendtag,
		    recvbuf,recvcount,recvtype,source,recvtag,comm,status);
  if (run_check==0) return err;
  PBCHECK(err,status);
  return err;
}

int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, 
			 MPI_Comm comm, MPI_Status *status)
{
  int err;
  err=PMPI_Sendrecv_replace(buf,count,datatype,sendtag,dest,recvtag,dest,comm,status);
  PBCHECK(err,status);
  return err;
}


int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
  int err;
  err=PMPI_Wait(request,status);
  if (run_check==0) return err;
  if (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL)  
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCHECK(err,status);
	}
    }
  return err;
}


int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status)
{
  int err;
  err=PMPI_Waitany(count,array_of_requests,index,status);
  if (run_check==0) return err;
  if (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL)  
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCHECK(err,status);
	}
    }
  return err;
}

int MPI_Waitsome(int count, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err,i;
  err=PMPI_Waitsome(count,array_of_requests,outcount,array_of_indices,array_of_statuses);
  if (run_check==0) return err;
  for (i=0; i<*outcount; i++)
    {
      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
	    {
	      PBCHECKARRAY(err,array_of_statuses,count,i);
	    }
	}
    }
  return err;  
}


int MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)
{
  int err,i;
  err=PMPI_Waitall(count,array_of_requests,array_of_statuses);
  if (run_check==0) return err;
  for (i=0; i<count; i++)
    {
      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
	    {
	      PBCHECKARRAY(err,array_of_statuses,count,i);
	    }
	}
    }
  return err;
}


int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  int err;
  err=PMPI_Test(request, flag, status);
  if (run_check==0) return err;
  if ((*flag) && (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL))
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCHECK(err,status);
	}
    }
  return err;
}

int MPI_Testany(int count, MPI_Request *array_of_requests, int *index, int *flag, MPI_Status *status)
{
  int err;
  err=PMPI_Testany(count,array_of_requests,index,flag,status);
  if (run_check==0) return err;
  if ((*flag) && (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL))
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCHECK(err,status);
	}
    }
  return err;  
}

int MPI_Testsome(int count, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err,i;
  err=PMPI_Testsome(count,array_of_requests,outcount,array_of_indices,array_of_statuses);
  if (run_check==0) return err;
  for (i=0; i<*outcount; i++)
    {
      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
	    {
	      PBCHECKARRAY(err,array_of_statuses,count,i);
	    }
	}
    }
  return err;  
}

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag, MPI_Status *array_of_statuses)
{
  int err,i;
  err=PMPI_Testall(count, array_of_requests, flag, array_of_statuses);
  if (run_check==0) return err;
  if (*flag) 
    {
      for (i=0; i<count; i++)
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	    {
	      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
		{
		  PBCHECKARRAY(err,array_of_statuses,count,i);
		}
	    }
	}
    }
  return err;
}


#endif

/*.......................................................*/
/* The End. */
