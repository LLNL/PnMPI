#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <pnmpimod.h>

#include <datatype.h>
#include <requests.h>

#include "pb_mod.h"

#define PNMPI_MAX_PIGGYBACK 10000

PNMPIMOD_Requests_MapRequest_t PNMPIMOD_requestmap;
static PNMPIMOD_Requests_RequestStorage_t req_add;

static PNMPIMOD_Status_RequestStorage_t status_add;
static int *TotalStatusExtension;

static int request_offset;
static int piggyback_offset;
static int *StatusOffsetInRequest;
static int piggyback_size;
static char pb_outbuffer[PNMPI_MAX_PIGGYBACK];



/*==========================================================================*/
/* Macros */

#define DT_DECLARE(var) MPI_Datatype var;

#ifdef PBSECOND

#define DT_GENERALALLOC(b,c,dt,pb,var) \
{ int err,lens[2]; MPI_Aint addr[2]; MPI_Datatype types[2]; \
  types[0]=MPI_CHAR; lens[0]=piggyback_size; err=MPI_Address((void*) pb, &(addr[0])); \
  if (err!=MPI_SUCCESS) { return err; } \
  types[1]=dt; lens[1]=c; err=MPI_Address((void*) b, &(addr[1])); \
  if (err!=MPI_SUCCESS) { return err; } \
  err=MPI_Type_struct(2,lens,addr,types,&var); \
  if (err!=MPI_SUCCESS) { return err; } \
  err=MPI_Type_commit(&var); \
  if (err!=MPI_SUCCESS) { return err; } \
}                     

#else /* PBSECOND */

#define DT_GENERALALLOC(b,c,dt,pb,var) \
{ int err,lens[2]; MPI_Aint addr[2]; MPI_Datatype types[2]; \
  types[1]=MPI_CHAR; lens[1]=piggyback_size; err=MPI_Address((void*) pb, &(addr[1])); \
  if (err!=MPI_SUCCESS) { return err; } \
  types[0]=dt; lens[0]=c; err=MPI_Address((void*) b, &(addr[0])); \
  if (err!=MPI_SUCCESS) { return err; } \
  err=MPI_Type_struct(2,lens,addr,types,&var); \
  if (err!=MPI_SUCCESS) { return err; } \
  err=MPI_Type_commit(&var); \
  if (err!=MPI_SUCCESS) { return err; } \
}                     

#endif /* PBSECOND */


#define DT_OUTALLOC(b,c,dt,var) DT_GENERALALLOC(b,c,dt,pb_outbuffer,var)
#define DT_STATUSALLOC(b,c,dt,st,var) DT_GENERALALLOC(b,c,dt,&(STATUS_STORAGE(status,piggyback_offset,char)),var);
#define DT_REQALLOC(b,c,dt,var,ptr) { ptr=(char*) malloc(piggyback_size);\
        if (ptr==NULL) { return MPI_ERROR_MEM; } \
        DT_GENERALALLOC(b,c,dt,ptr,var); }

#define PBCOPY(to,from,len) memcpy(to,from,len);
#define PBCOPYDEF(to,from) PBCOPY(to,from,piggyback_size)

#define DT_FREE(var) { int ret = MPI_Type_free(&var); if (ret!=MPI_SUCCESS) { return ret; } }

#define PBPTR_STORAGE(req,data) REQ_STORAGE(req,PNMPIMOD_requestmap,request_offset,void*,data);


#define COMM_REQ_FROM_STATUS(status) REQ_FROM_STATUS(status,(*StatusOffsetInRequest))
#define COMM_INFO_FROM_STATUS(status,type) INFO_FROM_STATUS(status,(*StatusOffsetInRequest),type)

#define COMM_REQ_FROM_STATUSARRAY(status,count,num) REQ_FROM_STATUSARRAY(status,(*StatusOffsetInRequest),*TotalStatusExtension,count,num)
#define COMM_INFO_FROM_STATUSARRAY(status,type,count,num) INFO_FROM_STATUSARRAY(status,(*StatusOffsetInRequest),*TotalStatusExtension,type,count,num)



/*==========================================================================*/
/* Implementation */


/*------------------------------------------------------------*/
/* External service routines */

int PNMPIMOD_Piggyback(int size,char *ptr)
{
  PBCOPY(pb_outbuffer,ptr,size);
  return MPI_SUCCESS;
}

int PNMPIMOD_Piggyback_Size(int size)
{
  /* Must be called before first communication */

  if (size<=PNMPI_MAX_PIGGYBACK)
    {
      piggyback_size=size;
      return MPI_SUCCESS;
    }
  else
    return MPI_ERROR_PNMPI;
}



/*--------------------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;
  PNMPI_Global_descriptor_t global;

  /* register this module and its services */

  err=PNMPI_Service_RegisterModule(PNMPI_MODULE_PB);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(service.name,"piggyback");
  service.fct=(PNMPI_Service_Fct_t*) PNMPIMOD_Piggyback;
  sprintf(service.sig,"ip");
  err=PNMPI_Service_RegisterService(&service);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(service.name,"piggyback-size");
  service.fct=(PNMPI_Service_Fct_t*) PNMPIMOD_Piggyback_Size;
  sprintf(service.sig,"i");
  err=PNMPI_Service_RegisterService(&service);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  sprintf(global.name,"piggyback-offset");
  global.addr.i=&piggyback_offset;
  global.sig='i';
  err=PNMPI_Service_RegisterGlobal(&global);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  piggyback_size=PNMPI_MAX_PIGGYBACK;

  return err;
}


/*.......................................................*/
/* Init */


int MPI_Init(int *argc, char ***argv)
{
  int err;
  PNMPI_modHandle_t handle_req,handle_status;
  PNMPI_Service_descriptor_t serv;
  PNMPI_Global_descriptor_t global;


  /* query the request module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_REQUEST,&handle_req);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetServiceByName(handle_req,"add-storage","i",&serv);
  if (err!=PNMPI_SUCCESS)
    return err;
  req_add=(PNMPIMOD_Requests_RequestStorage_t) ((void*)serv.fct);

  err=PNMPI_Service_GetServiceByName(handle_req,"map-request","r",&serv);
  if (err!=PNMPI_SUCCESS)
    return err;
  PNMPIMOD_requestmap=(PNMPIMOD_Requests_MapRequest_t) ((void*)serv.fct);

  err=PNMPI_Service_GetGlobalByName(handle_req,"status-offset",'i',&global);
  if (err!=PNMPI_SUCCESS)
    return err;
  StatusOffsetInRequest=(global.addr.i);


  /* query the status module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_STATUS,&handle_status);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetServiceByName(handle_status,"add-storage","i",&serv);
  if (err!=MPI_SUCCESS)
    return err;
  status_add=(PNMPIMOD_Requests_RequestStorage_t) serv.fct;

  err=PNMPI_Service_GetGlobalByName(handle_status,"total-status-extension",'i',&global);
  if (err!=PNMPI_SUCCESS)
    return err;
  TotalStatusExtension=(global.addr.i);


  /* request to track requests */

  request_offset=req_add(sizeof(char*));

  /* call the init routines */

  err=PMPI_Init(argc,argv);

  piggyback_offset=status_add(PNMPI_MAX_PIGGYBACK);

  return err;
}


/*.......................................................*/
/* Finalize */


int MPI_Finalize(void)
{
  return PMPI_Finalize();
}


/*--------------------------------------------------------------------------*/
/* Point to Point */

int MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Send(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm);
  DT_FREE(tmp_datatype);
  return err;
}

int MPI_Bsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Bsend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm);
  DT_FREE(tmp_datatype);
  return err;
}

int MPI_Rsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Rsend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm);
  DT_FREE(tmp_datatype);
  return err;
}

int MPI_Ssend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Ssend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm);
  DT_FREE(tmp_datatype);
  return err;
}


/*........................................................................*/
/* Async Sends */

int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Isend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm,request);
  DT_FREE(tmp_datatype);
  return err;
}

int MPI_Ibsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Ibsend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm,request);
  DT_FREE(tmp_datatype);
  return err;
}

int MPI_Irsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Irsend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm,request);
  DT_FREE(tmp_datatype);
  return err;
}

int MPI_Issend(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_OUTALLOC(buf,count,datatype,tmp_datatype);
  err=PMPI_Issend(MPI_BOTTOM,1,tmp_datatype,dest,tag,comm,request);
  DT_FREE(tmp_datatype);
  return err;
}


/*........................................................................*/
/* Sync. Receive */

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  int err;
  DT_DECLARE(tmp_datatype);

  DT_STATUSALLOC(buf,count,datatype,status,tmp_datatype);
  err=PMPI_Recv(MPI_BOTTOM,1,tmp_datatype,source,tag,comm,status);
  DT_FREE(tmp_datatype);
  return err;
}


/*........................................................................*/
/* Async. Receive */

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  void *ptr;
  DT_DECLARE(tmp_datatype);

  DT_REQALLOC(buf,count,datatype,tmp_datatype,ptr);
  err=PMPI_Irecv(MPI_BOTTOM,1,tmp_datatype,source,tag,comm,request);
  PBPTR_STORAGE(*request,ptr);
  DT_FREE(tmp_datatype);
  return err;
}


/*........................................................................*/
/* Send/Recv */

int MPI_Sendrecv(void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, 
		 void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag,
		 MPI_Comm comm, MPI_Status *status)
{
  int err;
  DT_DECLARE(tmp1_datatype);
  DT_DECLARE(tmp2_datatype);

  DT_OUTALLOC(sendbuf,sendcount,sendtype,tmp1_datatype);
  DT_STATUSALLOC(recvbuf,recvcount,recvtype,status,tmp2_datatype);

  err=PMPI_Sendrecv(MPI_BOTTOM,1,tmp1_datatype,dest,sendtag,
		    MPI_BOTTOM,1,tmp2_datatype,source,recvtag,comm,status);
  
  DT_FREE(tmp1_datatype);
  DT_FREE(tmp2_datatype);
  return err;
}

int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, 
			 MPI_Comm comm, MPI_Status *status)
{
  int err;
  DT_DECLARE(tmp_datatype);

  PBCOPYDEF(&(STATUS_STORAGE(status,piggyback_offset,char)),pb_outbuffer);
  DT_STATUSALLOC(buf,count,datatype,status,tmp_datatype);

  err=PMPI_Sendrecv_replace(MPI_BOTTOM,1,tmp_datatype,dest,sendtag,source,recvtag,comm,status);

  DT_FREE(tmp_datatype);
  return err;
}


/*........................................................................*/
/* Waits */

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
  int err;

  err=PMPI_Wait(request,status);
  if (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL)  
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCOPYDEF(&(STATUS_STORAGE(status,piggyback_offset,char)),
		    INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));
	  free(INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));    
	}
    }

  return err;
}

int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status)
{
  int err;

  err=PMPI_Waitany(count,array_of_requests,index,status);
  if (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL)  
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCOPYDEF(&(STATUS_STORAGE(status,piggyback_offset,char)),
		    INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));
	  free(INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));    
	}
    }

  return err;
}

int MPI_Waitsome(int count, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err,i;

  err=PMPI_Waitsome(count,array_of_requests,outcount,array_of_indices,array_of_statuses);

  for (i=0; i<*outcount; i++)
    {
      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
	    {
	      PBCOPYDEF(&(STATUS_STORAGE_ARRAY(array_of_statuses,piggyback_offset,*TotalStatusExtension,char,count,i)),
			INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
	      free(INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
	    }
	}
    }

  return err;  
}

#define PB_PATTERN 0x42434445

int MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)
{
  int err,i;

  err=PMPI_Waitall(count,array_of_requests,array_of_statuses);

  for (i=0; i<count; i++)
    {
      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
	    {
	      PBCOPYDEF(&(STATUS_STORAGE_ARRAY(array_of_statuses,piggyback_offset,*TotalStatusExtension,char,count,i)),
			INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
	      free(INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));

	      if (STATUS_STORAGE_ARRAY(array_of_statuses,piggyback_offset,*TotalStatusExtension,int,count,i)!=PB_PATTERN)
		printf("Received wrong pattern %08x\n",STATUS_STORAGE_ARRAY(array_of_statuses,piggyback_offset,*TotalStatusExtension,int,count,i));
	    }
	}
    }

  return err;
}


/*........................................................................*/
/* Tests */

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  int err;

  err=PMPI_Test(request, flag, status);
  if ((flag) && (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL))
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCOPYDEF(&(STATUS_STORAGE(status,piggyback_offset,char)),
		    INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));
	  free(INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));
	}
    }

  return err;
}

int MPI_Testany(int count, MPI_Request *array_of_requests, int *index, int *flag, MPI_Status *status)
{
  int err;

  err=PMPI_Testany(count,array_of_requests,index,flag,status);
  if ((flag) && (COMM_REQ_FROM_STATUS(status).inreq!=MPI_REQUEST_NULL))
    {
      if (COMM_REQ_FROM_STATUS(status).type==PNMPIMOD_REQUESTS_RECV)
	{
	  PBCOPYDEF(&(STATUS_STORAGE(status,piggyback_offset,char)),
		    INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));
	  free(INFO_FROM_STATUS(status,*StatusOffsetInRequest,char*));
	}
    }

  return err;  
}

int MPI_Testsome(int count, MPI_Request *array_of_requests, int *outcount, int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err,i;

  err=PMPI_Testsome(count,array_of_requests,outcount,array_of_indices,array_of_statuses);

  for (i=0; i<*outcount; i++)
    {
      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
	    {
	      PBCOPYDEF(&(STATUS_STORAGE_ARRAY(array_of_statuses,piggyback_offset,*TotalStatusExtension,char,count,i)),
			INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
	      free(INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
	    }
	}
    }

  return err;  
}

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag, MPI_Status *array_of_statuses)
{
  int err,i;

  err=PMPI_Testall(count, array_of_requests, flag, array_of_statuses);

  if (flag) 
    {
      for (i=0; i<count; i++)
	{
	  if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).inreq!=MPI_REQUEST_NULL)  
	    {
	      if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses,count,i).type==PNMPIMOD_REQUESTS_RECV)
		{
		  PBCOPYDEF(&(STATUS_STORAGE_ARRAY(array_of_statuses,piggyback_offset,*TotalStatusExtension,char,count,i)),
			    INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
		  free(INFO_FROM_STATUSARRAY(array_of_statuses,*StatusOffsetInRequest,*TotalStatusExtension,char*,count,i));
		}
	    }
	}
    }

  return err;
}


/*--------------------------------------------------------------------------*/
/* The End. */

