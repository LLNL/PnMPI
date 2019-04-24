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

#include "requests.h"

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>
#include <status.h>

using namespace std;


/*------------------------------------------------------------*/
/* globals */

#define TABLE_SEGMENT_SIZE 1024


/*.......................................................*/
/* types */

typedef struct Req_Int_d
{
  PNMPIMOD_Requests_Parameters_t param;
  int next_free;
} Req_Int_t;

typedef struct ReqTable_d
{
  char *data;
  Req_Int_t *reqs;
  int freelist;
  int size;
  int storage_stride;
} ReqTable_t;


/*.......................................................*/
/* variables */

static int extra_request_space = 0;

static PNMPIMOD_Status_RequestStorage_t status_add;

static ReqTable_t reqtable;

static int PNMPIMOD_Request_offsetInStatus;
static int *TotalStatusExtension;

static map<MPI_Request, int> requestmap;

static int check_level;
static int req_rank;


/*.......................................................*/
/* macros */

#define CHECK_COPY(status)                                             \
  ((status != MPI_STATUS_IGNORE) && (status != MPI_STATUSES_IGNORE) && \
   (status->MPI_ERROR == PNMPIMOD_STATUS_TAG))

#define ASSOCIATE_REQUEST(request, _type, _buf, _count, _datatype, _node, \
                          _tag, _comm, _persistent)                       \
  if (request != MPI_REQUEST_NULL)                                        \
    {                                                                     \
      Req_Int_t *rp;                                                      \
      if (reqtable.freelist == -1)                                        \
        {                                                                 \
          int err = allocate_new_reqtable();                              \
          if (err != MPI_SUCCESS)                                         \
            return err;                                                   \
        }                                                                 \
      requestmap[request] = reqtable.freelist;                            \
      rp = &(reqtable.reqs[reqtable.freelist]);                           \
      rp->param.type = _type;                                             \
      rp->param.buf = _buf;                                               \
      rp->param.count = _count;                                           \
      rp->param.datatype = _datatype;                                     \
      rp->param.node = _node;                                             \
      rp->param.tag = _tag;                                               \
      rp->param.comm = _comm;                                             \
      rp->param.persistent = _persistent;                                 \
      rp->param.active = 1 - _persistent;                                 \
      rp->param.cancelled = 0;                                            \
      rp->param.inreq = request;                                          \
      reqtable.freelist = rp->next_free;                                  \
    }

#define COPY_REQUEST_STATUSARRAY(err, request, status, count, num)             \
  {                                                                            \
    memcpy(&(STATUS_STORAGE_ARRAY(                                             \
             status, PNMPIMOD_Request_offsetInStatus, *TotalStatusExtension,   \
             PNMPIMOD_Requests_Parameters_t, count, num)),                     \
           &(reqtable.reqs[requestmap[request]].param),                        \
           sizeof(PNMPIMOD_Requests_Parameters_t));                            \
    memcpy(                                                                    \
      &(STATUS_STORAGE_ARRAY(status, PNMPIMOD_Request_offsetInStatus +         \
                                       sizeof(PNMPIMOD_Requests_Parameters_t), \
                             *TotalStatusExtension,                            \
                             PNMPIMOD_Requests_Parameters_t, count, num)),     \
      &(reqtable.data[requestmap[request] * reqtable.storage_stride]),         \
      reqtable.storage_stride);                                                \
  }

#define COPY_REQUEST(err, request, status)                \
  {                                                       \
    COPY_REQUEST_STATUSARRAY(err, request, status, 1, 0); \
  }



#define FREE_REQUEST(err, oldreq, newreq)                                    \
  {                                                                          \
    if ((err == MPI_SUCCESS) && (oldreq != MPI_REQUEST_NULL))                \
      {                                                                      \
        if (reqtable.reqs[requestmap[oldreq]].param.persistent)              \
          {                                                                  \
            reqtable.reqs[requestmap[oldreq]].param.active = 0;              \
          }                                                                  \
        else                                                                 \
          {                                                                  \
            reqtable.reqs[requestmap[oldreq]].next_free = reqtable.freelist; \
            reqtable.freelist = requestmap[oldreq];                          \
            requestmap.erase(oldreq);                                        \
          }                                                                  \
      }                                                                      \
  }


/*------------------------------------------------------------*/
/* External service routines */
/* Comment: this routine is NOT thread safe, since it can
   only be called from MPI_Init - hence there should not be
   more than one thread calling it anyway */

int PNMPIMOD_Requests_RequestStorage(int size)
{
  int ret;

  ret = extra_request_space;
  extra_request_space += size;

  return ret;
}

PNMPIMOD_Requests_Parameters_t *PNMPIMOD_Requests_MapRequest(MPI_Request req)
{
  return &(reqtable.reqs[requestmap[req]].param);
}


/*------------------------------------------------------------*/
/* Internal routines */

int allocate_new_reqtable()
{
  int i;

  /* increase table */

  reqtable.reqs = (Req_Int_t *)realloc(
    reqtable.reqs, sizeof(Req_Int_t) * (reqtable.size + TABLE_SEGMENT_SIZE));
  if (reqtable.reqs == NULL)
    return MPI_ERR_NO_MEM;

  if (reqtable.storage_stride > 0)
    {
      reqtable.data =
        (char *)realloc(reqtable.data, reqtable.storage_stride *
                                         (reqtable.size + TABLE_SEGMENT_SIZE));
      if (reqtable.data == NULL)
        return MPI_ERR_NO_MEM;
      for (i = 0; i < reqtable.size + TABLE_SEGMENT_SIZE; i++)
        reqtable.reqs[i].param.data =
          &(reqtable.data[i * reqtable.storage_stride]);
    }

  /* link the new slots into the freelist */

  for (i = reqtable.size + TABLE_SEGMENT_SIZE - 1; i >= reqtable.size; i--)
    {
      reqtable.reqs[i].next_free = reqtable.freelist;
      reqtable.freelist = i;
    }

  /* increase storage mark */

  reqtable.size += TABLE_SEGMENT_SIZE;

  return PNMPI_SUCCESS;
}


/*------------------------------------------------------------*/
/* Wrapper routines */

/*.......................................................*/
/* Registration */

void PNMPI_RegistrationPoint()
{
  int err;
  PNMPI_Service_descriptor_t service;
  PNMPI_Global_descriptor_t global;


  /* reset variables */

  extra_request_space = 0;


  /* register this module and its services */

  err = PNMPI_Service_RegisterModule(PNMPI_MODULE_REQUEST);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(service.name, "add-storage");
  service.fct = (PNMPI_Service_Fct_t)PNMPIMOD_Requests_RequestStorage;
  sprintf(service.sig, "i");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(service.name, "map-request");
  service.fct = (PNMPI_Service_Fct_t)PNMPIMOD_Requests_MapRequest;
  sprintf(service.sig, "r");
  err = PNMPI_Service_RegisterService(&service);
  if (err != PNMPI_SUCCESS)
    return;

  sprintf(global.name, "status-offset");
  global.addr.i = &PNMPIMOD_Request_offsetInStatus;
  global.sig = 'i';
  err = PNMPI_Service_RegisterGlobal(&global);
  if (err != PNMPI_SUCCESS)
    return;
}


/*.......................................................*/
/* Init */

int MPI_Init(int *argc, char ***argv)
{
  int err;
  PNMPI_modHandle_t handle_status;
  PNMPI_Service_descriptor_t serv;
  PNMPI_Global_descriptor_t global;
  PNMPI_modHandle_t handle_req;
  const char *clevel_s;

  /* are we doing checks at the end? */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_REQUEST, &handle_req);
  if (err != PNMPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetArgument(handle_req, "check", &clevel_s);
  if (err != PNMPI_SUCCESS)
    {
      if (err == PNMPI_NOARG)
        check_level = 0;
      else
        return err;
    }
  else
    {
      check_level = atoi(clevel_s);
    }



  /* all modules have registered their
     needs for extra storage - this number is now fixed */

  /* could add padding to stride of data table */

  reqtable.storage_stride = extra_request_space;

  /* initialize */

  reqtable.data = NULL;
  reqtable.reqs = NULL;
  reqtable.size = 0;
  reqtable.freelist = -1;
  allocate_new_reqtable();
  ASSOCIATE_REQUEST(MPI_REQUEST_NULL, PNMPIMOD_REQUESTS_NULL, NULL, 0, MPI_INT,
                    -1, -1, MPI_COMM_NULL, 1);


  /* call the init routines */

  err = PMPI_Init(argc, argv);
  if (err != MPI_SUCCESS)
    return err;

  err = PMPI_Comm_rank(MPI_COMM_WORLD, &req_rank);
  if (err != MPI_SUCCESS)
    return err;


  /* query the status module */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_STATUS, &handle_status);
  if (err != MPI_SUCCESS)
    return err;

  err =
    PNMPI_Service_GetServiceByName(handle_status, "add-storage", "i", &serv);
  if (err != MPI_SUCCESS)
    return err;
  status_add = (PNMPIMOD_Requests_RequestStorage_t)serv.fct;

  err = PNMPI_Service_GetGlobalByName(handle_status, "total-status-extension",
                                      'i', &global);
  if (err != MPI_SUCCESS)
    return err;
  TotalStatusExtension = (global.addr.i);


  /* request to track requests */

  PNMPIMOD_Request_offsetInStatus = status_add(
    reqtable.storage_stride + sizeof(PNMPIMOD_Requests_Parameters_t));

  return err;
}


/*.......................................................*/
/* finalize */

int MPI_Finalize()
{
  int err;
  int first;
  char type_s[30];
  map<MPI_Request, int>::iterator i;

  first = 1;
  if (check_level)
    {
      for (i = requestmap.begin(); i != requestmap.end(); i++)
        {
          if (first)
            printf("REQYEST CHECK: Node %i had requests that have not been "
                   "cleaned up\n",
                   req_rank);
          first = 0;

          switch (reqtable.reqs[i->second].param.type)
            {
            case PNMPIMOD_REQUESTS_SEND: sprintf(type_s, "Send "); break;
            case PNMPIMOD_REQUESTS_BSEND: sprintf(type_s, "Bsend"); break;
            case PNMPIMOD_REQUESTS_RSEND: sprintf(type_s, "Rsend"); break;
            case PNMPIMOD_REQUESTS_SSEND: sprintf(type_s, "Ssend"); break;
            case PNMPIMOD_REQUESTS_RECV: sprintf(type_s, "Recv "); break;
            default: sprintf(type_s, "Invalid"); break;
            }

          printf("Node %i: %s count %i, node %i, persistent %i\n", req_rank,
                 type_s, reqtable.reqs[i->second].param.count,
                 reqtable.reqs[i->second].param.node,
                 reqtable.reqs[i->second].param.persistent);
        }
      if (first)
        printf("REQUEST CHECK: all requests have been cleaned up on node %i\n",
               req_rank);
    }

  err = PMPI_Finalize();

  if (reqtable.data)
    free(reqtable.data);
  if (reqtable.reqs)
    free(reqtable.reqs);

  return err;
}


/*.......................................................*/
/* Isend */

int MPI_Isend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_SEND, buf, count, datatype,
                    dest, tag, comm, 0);

  return err;
}


/*.......................................................*/
/* Ibsend */

int MPI_Ibsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
               MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_BSEND, buf, count, datatype,
                    dest, tag, comm, 0);
  return err;
}


/*.......................................................*/
/* Issend */

int MPI_Issend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
               MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_SSEND, buf, count, datatype,
                    dest, tag, comm, 0);
  return err;
}


/*.......................................................*/
/* Irsend */

int MPI_Irsend(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
               MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_RSEND, buf, count, datatype,
                    dest, tag, comm, 0);
  return err;
}


/*.......................................................*/
/* Irecv */

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
              MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_RECV, buf, count, datatype,
                    source, tag, comm, 0);
  return err;
}


/*.......................................................*/
/* Wait */

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
  /* removes a request */

  int err;
  MPI_Request req = *request;

  if (CHECK_COPY(status))
    {
      COPY_REQUEST(err, *request, status);
    }

  err = PMPI_Wait(request, status);
  FREE_REQUEST(err, req, *request);
  return err;
}


/*.......................................................*/
/* Test */

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  /* removes a request */

  int err;
  MPI_Request req = *request;

  if (CHECK_COPY(status))
    {
      COPY_REQUEST(err, *request, status);
    }

  err = PMPI_Test(request, flag, status);
  if (*flag)
    FREE_REQUEST(err, req, *request);
  return err;
}


/*.......................................................*/
/* Waitany */

int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index,
                MPI_Status *status)
{
  int err, check, i;
  MPI_Request *ra;

  check = CHECK_COPY(status);
  ra = (MPI_Request *)malloc(count * sizeof(MPI_Request));
  if (ra == NULL)
    return MPI_ERR_NO_MEM;
  for (i = 0; i < count; i++)
    {
      ra[i] = array_of_requests[i];
    }

  err = PMPI_Waitany(count, array_of_requests, index, status);

  if (check)
    {
      COPY_REQUEST(err, ra[*index], status);
    }
  FREE_REQUEST(err, ra[*index], array_of_requests[*index]);

  free(ra);
  return err;
}


/*.......................................................*/
/* Testany */

int MPI_Testany(int count, MPI_Request *array_of_requests, int *index,
                int *flag, MPI_Status *status)
{
  int err, check, i;
  MPI_Request *ra;

  check = CHECK_COPY(status);
  ra = (MPI_Request *)malloc(count * sizeof(MPI_Request));
  if (ra == NULL)
    return MPI_ERR_NO_MEM;
  for (i = 0; i < count; i++)
    {
      ra[i] = array_of_requests[i];
    }

  err = PMPI_Testany(count, array_of_requests, index, flag, status);

  if (*flag)
    {
      if (check)
        {
          COPY_REQUEST(err, ra[*index], status);
        }
      FREE_REQUEST(err, ra[*index], array_of_requests[*index]);
    }

  free(ra);
  return err;
}


/*.......................................................*/
/* Waitall */

int MPI_Waitall(int count, MPI_Request *array_of_requests,
                MPI_Status *array_of_statuses)
{
  int err, check, i;
  MPI_Request *ra;

  check = CHECK_COPY(array_of_statuses);
  ra = (MPI_Request *)malloc(count * sizeof(MPI_Request));
  if (ra == NULL)
    return MPI_ERR_NO_MEM;
  for (i = 0; i < count; i++)
    {
      ra[i] = array_of_requests[i];
    }

  err = PMPI_Waitall(count, array_of_requests, array_of_statuses);

  for (i = 0; i < count; i++)
    {
      if (check)
        {
          COPY_REQUEST_STATUSARRAY(err, ra[i], array_of_statuses, count, i);
        }
      FREE_REQUEST(err, ra[i], array_of_requests[i]);
    }

  free(ra);
  return err;
}


/*.......................................................*/
/* Testall */

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag,
                MPI_Status *array_of_statuses)
{
  int err, check, i;
  MPI_Request *ra;

  check = CHECK_COPY(array_of_statuses);
  ra = (MPI_Request *)malloc(count * sizeof(MPI_Request));
  if (ra == NULL)
    return MPI_ERR_NO_MEM;
  for (i = 0; i < count; i++)
    {
      ra[i] = array_of_requests[i];
    }

  err = PMPI_Testall(count, array_of_requests, flag, array_of_statuses);

  if (*flag)
    {
      for (i = 0; i < count; i++)
        {
          if (check)
            {
              COPY_REQUEST_STATUSARRAY(err, ra[i], array_of_statuses, count, i);
            }
          FREE_REQUEST(err, ra[i], array_of_requests[i]);
        }
    }

  free(ra);
  return err;
}


/*.......................................................*/
/* Waitsome */

int MPI_Waitsome(int count, MPI_Request *array_of_requests, int *outcount,
                 int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err, check, i;
  MPI_Request *ra;

  check = CHECK_COPY(array_of_statuses);
  ra = (MPI_Request *)malloc(count * sizeof(MPI_Request));
  if (ra == NULL)
    return MPI_ERR_NO_MEM;
  for (i = 0; i < count; i++)
    {
      ra[i] = array_of_requests[i];
    }

  err = PMPI_Waitsome(count, array_of_requests, outcount, array_of_indices,
                      array_of_statuses);

  for (i = 0; i < *outcount; i++)
    {
      if (check)
        {
          COPY_REQUEST_STATUSARRAY(err, ra[array_of_indices[i]],
                                   array_of_statuses, count, i);
        }
      FREE_REQUEST(err, ra[array_of_indices[i]],
                   array_of_requests[array_of_indices[i]]);
    }

  free(ra);
  return err;
}


/*.......................................................*/
/* Testsome */

int MPI_Testsome(int incount, MPI_Request *array_of_requests, int *outcount,
                 int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err, check, i;
  MPI_Request *ra;

  check = CHECK_COPY(array_of_statuses);
  ra = (MPI_Request *)malloc(incount * sizeof(MPI_Request));
  if (ra == NULL)
    return MPI_ERR_NO_MEM;
  for (i = 0; i < incount; i++)
    {
      ra[i] = array_of_requests[i];
    }

  err = PMPI_Testsome(incount, array_of_requests, outcount, array_of_indices,
                      array_of_statuses);
  for (i = 0; i < *outcount; i++)
    {
      if (check)
        {
          COPY_REQUEST_STATUSARRAY(err, ra[array_of_indices[i]],
                                   array_of_statuses, incount, i);
        }
      FREE_REQUEST(err, ra[array_of_indices[i]],
                   array_of_requests[array_of_indices[i]]);
    }

  free(ra);
  return err;
}


/*.......................................................*/
/* Cancel */

int MPI_Cancel(MPI_Request *request)
{
  int err;
  MPI_Request req = *request;
  err = PMPI_Cancel(request);
  FREE_REQUEST(err, req, *request);
  return err;
}


/*.......................................................*/
/* Send init */

int MPI_Send_init(void *buf, int count, MPI_Datatype datatype, int dest,
                  int tag, MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Send_init(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_SEND, buf, count, datatype,
                    dest, tag, comm, 1);
  return err;
}


/*.......................................................*/
/* Bsend init */

int MPI_Bsend_init(void *buf, int count, MPI_Datatype datatype, int dest,
                   int tag, MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_BSEND, buf, count, datatype,
                    dest, tag, comm, 1);
  return err;
}


/*.......................................................*/
/* Rsend init */

int MPI_Rsend_init(void *buf, int count, MPI_Datatype datatype, int dest,
                   int tag, MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_RSEND, buf, count, datatype,
                    dest, tag, comm, 1);
  return err;
}


/*.......................................................*/
/* Rsend init */

int MPI_Ssend_init(void *buf, int count, MPI_Datatype datatype, int dest,
                   int tag, MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_SSEND, buf, count, datatype,
                    dest, tag, comm, 1);
  return err;
}


/*.......................................................*/
/* Receive init */

int MPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source,
                  int tag, MPI_Comm comm, MPI_Request *request)
{
  /* creates a request */

  int err;

  err = PMPI_Recv_init(buf, count, datatype, source, tag, comm, request);
  ASSOCIATE_REQUEST(*request, PNMPIMOD_REQUESTS_RECV, buf, count, datatype,
                    source, tag, comm, 1);
  return err;
}


/*.......................................................*/
/* Start */

int MPI_Start(MPI_Request *request)
{
  int err;
  err = PMPI_Start(request);
  if (err == MPI_SUCCESS)
    reqtable.reqs[requestmap[*request]].param.active = 1;
  return err;
}


/*.......................................................*/
/* Startall */

int MPI_Startall(int count, MPI_Request *array_of_requests)
{
  int i, err;
  err = PMPI_Startall(count, array_of_requests);
  if (err == MPI_SUCCESS)
    {
      for (i = 0; i < count; i++)
        {
          reqtable.reqs[requestmap[array_of_requests[i]]].param.active = 1;
        }
    }
  return err;
}

/*.......................................................*/
/* free a persistent request */

int MPI_Request_free(MPI_Request *request)
{
  int err;
  MPI_Request req = *request;
  if ((*request) != MPI_REQUEST_NULL)
    reqtable.reqs[requestmap[(*request)]].param.persistent = 0;
  err = PMPI_Request_free(request);
  FREE_REQUEST(err, req, *request);
  return err;
}


/*------------------------------------------------------------*/
/* The End. */
