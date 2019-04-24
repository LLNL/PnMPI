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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/const.h>
#include <pnmpi/hooks.h>
#include <pnmpi/service.h>

#include "commsub.h"


#define PNMPI_MODULE_COMM "communication-tracker"

PNMPIMOD_Datatype_getReference_t dt_get;
PNMPIMOD_Datatype_delReference_t dt_del;
PNMPIMOD_Datatype_getSize_t dt_size;

int verbosity_level;

PNMPIMOD_Requests_RequestStorage_t req_add;
PNMPIMOD_Requests_MapRequest_t PNMPIMOD_requestmap;

static int offset, *StatusOffsetInRequest;
int *TotalStatusExtension;

#define COMM_REQ_FROM_STATUS(status) \
  REQ_FROM_STATUS(status, (*StatusOffsetInRequest))
#define COMM_INFO_FROM_STATUS(status, type) \
  INFO_FROM_STATUS(status, (*StatusOffsetInRequest), type)

#define COMM_REQ_FROM_STATUSARRAY(status, count, num)    \
  REQ_FROM_STATUSARRAY(status, (*StatusOffsetInRequest), \
                       *TotalStatusExtension, count, num)
#define COMM_INFO_FROM_STATUSARRAY(status, type, count, num) \
  INFO_FROM_STATUSARRAY(status, (*StatusOffsetInRequest),    \
                        *TotalStatusExtension, type, count, num)


/* The MPI-2 standard deprecated a number of routines because MPI-2 provides
 * better versions. The following macros will replace calls to the old functions
 * by calls to the new ones. */
#if MPI_VERSION >= 2
#define PMPI_Type_extent(sendtype, e) PMPI_Type_get_extent(sendtype, NULL, e)
#endif


/*==========================================================================*/
/* Actual Macros */
/* The following can be used as a template to generate new submodules */

#if 0

void COMM_ALL_INIT(int argc, char**argv)
{
}

void COMM_ALL_PREINIT(int argc, char**argv)
{
}

void COMM_ALL_FINALIZE()
{
}

void SEND_P2P_START(
    PNMPI_CONST
    void *buf, int count, MPI_Datatype dt, int node, int tag,
		    MPI_Comm comm, void **ptr, int type)
{
}

void SEND_P2P_ASYNC_MID1(
    PNMPI_CONST
    void *buf, int count, MPI_Datatype dt, int node, int tag,
			 MPI_Comm comm, void **ptr, int type)
{
}

void SEND_P2P_END(
    PNMPI_CONST
    void *buf, int count, MPI_Datatype dt, int node, int tag,
		  MPI_Comm comm, int err, void **ptr, void **midptr, int type)
{
}

void RECV_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag,
		    MPI_Comm comm, void **ptr, int type)
{
}

void RECV_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node, int tag,
			 MPI_Comm comm, int err, void **ptr, int type)
{
}

void RECV_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag,
                  MPI_Comm comm, int err, void **ptr, void **midptr, int type,
                  MPI_Status *statusarray, int numindex, int index)
{
}

void COMM_P2P_ASYNC_MID2(int count, MPI_Request *requests, int flag, void **midptr)
{
}

void COMM_COLL_REDUCE(void *buf, int count, MPI_Datatype dt, MPI_Op ops, int size, void** ptr)
{
}

void COMM_COLL_START(MPI_Comm comm,int root,int type, void **ptr)
{
}

void COMM_COLL_END(MPI_Comm comm,int root,int type, void **ptr)
{
}


#endif


/*==========================================================================*/
/* Implementation */

int pnmpimod_comm_collective_support = PNMPIMOD_COMM_DEFAULT;


/*--------------------------------------------------------------------------*/
/* module local support routine */

void pnmpimod_comm_set_collectivesupport(int param)
{
  pnmpimod_comm_collective_support = param;
}


/*--------------------------------------------------------------------------*/
/* Setup */

/*.......................................................*/
/* Registration */

void PNMPI_RegistrationPoint()
{
  /* register this module and its services */
  char name[1000];
  sprintf(name, "%s:%s", PNMPI_MODULE_COMM, provide_submod_name());
  PNMPI_Service_RegisterModule(name);
}


/*.......................................................*/
/* Init */


int MPI_Init(int *argc, char ***argv)
{
  int err, defer_err;
  PNMPI_modHandle_t handle_dt, handle_req, handle_status, handle_comm;
  PNMPI_Service_descriptor_t serv;
  PNMPI_Global_descriptor_t global;
  const char *vlevel_s;
  char name[1000];

  /* query the datatype module */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_DATATYPE, &handle_dt);
  if (err != PNMPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetServiceByName(handle_dt, "getDatatypeReference",
                                       "Pimp", &serv);
  if (err != PNMPI_SUCCESS)
    return err;
  dt_get = (PNMPIMOD_Datatype_getReference_t)((void *)serv.fct);

  err = PNMPI_Service_GetServiceByName(handle_dt, "delDatatypeReference", "p",
                                       &serv);
  if (err != PNMPI_SUCCESS)
    return err;
  dt_del = (PNMPIMOD_Datatype_delReference_t)((void *)serv.fct);

  err =
    PNMPI_Service_GetServiceByName(handle_dt, "getDatatypeSize", "mp", &serv);
  if (err != PNMPI_SUCCESS)
    return err;
  dt_size = (PNMPIMOD_Datatype_getSize_t)((void *)serv.fct);



  /* query the request module */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_REQUEST, &handle_req);
  if (err != PNMPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetServiceByName(handle_req, "add-storage", "i", &serv);
  if (err != PNMPI_SUCCESS)
    return err;
  req_add = (PNMPIMOD_Requests_RequestStorage_t)((void *)serv.fct);

  err = PNMPI_Service_GetServiceByName(handle_req, "map-request", "r", &serv);
  if (err != PNMPI_SUCCESS)
    return err;
  PNMPIMOD_requestmap = (PNMPIMOD_Requests_MapRequest_t)((void *)serv.fct);

  err =
    PNMPI_Service_GetGlobalByName(handle_req, "status-offset", 'i', &global);
  if (err != PNMPI_SUCCESS)
    return err;
  StatusOffsetInRequest = (global.addr.i);


  /* query the status module */

  err = PNMPI_Service_GetModuleByName(PNMPI_MODULE_STATUS, &handle_status);
  if (err != PNMPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetGlobalByName(handle_status, "total-status-extension",
                                      'i', &global);
  if (err != PNMPI_SUCCESS)
    return err;
  TotalStatusExtension = (global.addr.i);


  /* query own module */

  sprintf(name, "%s:%s", PNMPI_MODULE_COMM, provide_submod_name());

  err = PNMPI_Service_GetModuleByName(name, &handle_comm);
  if (err != PNMPI_SUCCESS)
    return err;

  err = PNMPI_Service_GetArgument(handle_comm, "verbose", &vlevel_s);
  if (err != PNMPI_SUCCESS)
    {
      if (err == PNMPI_NOARG)
        verbosity_level = 0;
      else
        return err;
    }
  else
    {
      verbosity_level = atoi(vlevel_s);
    }


  /* request to track requests */

  offset = req_add(sizeof(void *));

  COMM_ALL_PREINIT(*argc, *argv);
  defer_err = PMPI_Init(argc, argv);

  if (defer_err == MPI_SUCCESS)
    {
      COMM_ALL_INIT(*argc, *argv);
    }

  return defer_err;
}


/*.......................................................*/
/* Finalize */


int MPI_Finalize(void)
{
  COMM_ALL_FINALIZE();
  return PMPI_Finalize();
}


/*--------------------------------------------------------------------------*/
/* Point to Point */

/*........................................................................*/
/* Sync Sends */

int MPI_Send(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
             int tag, MPI_Comm comm)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Send(buf, count, datatype, dest, tag, comm);
  SEND_P2P_END(buf, count, datatype, dest, tag, comm, err, &ptr, NULL,
               PNMPIMOD_COMM_P2P);
  return err;
}

int MPI_Bsend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Bsend(buf, count, datatype, dest, tag, comm);
  SEND_P2P_END(buf, count, datatype, dest, tag, comm, err, &ptr, NULL,
               PNMPIMOD_COMM_P2P);
  return err;
}

int MPI_Rsend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Rsend(buf, count, datatype, dest, tag, comm);
  SEND_P2P_END(buf, count, datatype, dest, tag, comm, err, &ptr, NULL,
               PNMPIMOD_COMM_P2P);
  return err;
}

int MPI_Ssend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Ssend(buf, count, datatype, dest, tag, comm);
  SEND_P2P_END(buf, count, datatype, dest, tag, comm, err, &ptr, NULL,
               PNMPIMOD_COMM_P2P);
  return err;
}


/*........................................................................*/
/* Async Sends */

int MPI_Isend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_ASYNC_P2P);
  err = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
  SEND_P2P_ASYNC_MID1(buf, count, datatype, dest, tag, comm, &ptr,
                      PNMPIMOD_COMM_ASYNC_P2P);
  REQ_STORAGE(*request, PNMPIMOD_requestmap, offset, void *, ptr);
  return err;
}

int MPI_Ibsend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype,
               int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_ASYNC_P2P);
  err = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
  SEND_P2P_ASYNC_MID1(buf, count, datatype, dest, tag, comm, &ptr,
                      PNMPIMOD_COMM_ASYNC_P2P);
  REQ_STORAGE(*request, PNMPIMOD_requestmap, offset, void *, ptr);
  return err;
}

int MPI_Irsend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype,
               int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_ASYNC_P2P);
  err = PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
  SEND_P2P_ASYNC_MID1(buf, count, datatype, dest, tag, comm, &ptr,
                      PNMPIMOD_COMM_ASYNC_P2P);
  REQ_STORAGE(*request, PNMPIMOD_requestmap, offset, void *, ptr);
  return err;
}

int MPI_Issend(PNMPI_CONST void *buf, int count, MPI_Datatype datatype,
               int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  int err;
  void *ptr;
  SEND_P2P_START(buf, count, datatype, dest, tag, comm, &ptr,
                 PNMPIMOD_COMM_ASYNC_P2P);
  err = PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
  SEND_P2P_ASYNC_MID1(buf, count, datatype, dest, tag, comm, &ptr,
                      PNMPIMOD_COMM_ASYNC_P2P);
  REQ_STORAGE(*request, PNMPIMOD_requestmap, offset, void *, ptr);
  return err;
}


/*........................................................................*/
/* Sync. Receive */

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
             MPI_Comm comm, MPI_Status *status)
{
  int err;
  void *ptr;
  RECV_P2P_START(buf, count, datatype, source, tag, comm, &ptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Recv(buf, count, datatype, source, tag, comm, status);
  PMPI_Get_count(status, datatype, &count);
  RECV_P2P_END(buf, count, datatype, status->MPI_SOURCE, status->MPI_TAG, comm,
               status->MPI_ERROR, &ptr, NULL, PNMPIMOD_COMM_P2P, status, 1, -1);
  return err;
}


/*........................................................................*/
/* Async. Receive */

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
              MPI_Comm comm, MPI_Request *request)
{
  int err;
  void *ptr;
  RECV_P2P_START(buf, count, datatype, source, tag, comm, &ptr,
                 PNMPIMOD_COMM_ASYNC_P2P);
  err = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
  RECV_P2P_ASYNC_MID1(buf, count, datatype, source, tag, comm, err, &ptr,
                      PNMPIMOD_COMM_ASYNC_P2P);
  REQ_STORAGE(*request, PNMPIMOD_requestmap, offset, void *, ptr);
  return err;
}


/*........................................................................*/
/* Send/Recv */

int MPI_Sendrecv(PNMPI_CONST void *sendbuf, int sendcount,
                 MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf,
                 int recvcount, MPI_Datatype recvtype, int source, int recvtag,
                 MPI_Comm comm, MPI_Status *status)
{
  int err;
  void *sptr = NULL, *rptr = NULL;

  SEND_P2P_START(sendbuf, sendcount, sendtype, dest, sendtag, comm, &sptr,
                 PNMPIMOD_COMM_P2P);
  RECV_P2P_START(recvbuf, recvcount, recvtype, source, recvtag, comm, &rptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf,
                      recvcount, recvtype, source, recvtag, comm, status);
  SEND_P2P_END(sendbuf, sendcount, sendtype, dest, sendtag, comm, err, &sptr,
               NULL, PNMPIMOD_COMM_P2P);
  PMPI_Get_count(status, recvtype, &recvcount);
  RECV_P2P_END(recvbuf, recvcount, recvtype, status->MPI_SOURCE,
               status->MPI_TAG, comm, status->MPI_ERROR, &rptr, NULL,
               PNMPIMOD_COMM_P2P, status, 1, 0);
  return err;
}

int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest,
                         int sendtag, int source, int recvtag, MPI_Comm comm,
                         MPI_Status *status)
{
  int err;
  void *sptr = NULL, *rptr = NULL;

  SEND_P2P_START(buf, count, datatype, dest, sendtag, comm, &sptr,
                 PNMPIMOD_COMM_P2P);
  RECV_P2P_START(buf, count, datatype, source, recvtag, comm, &rptr,
                 PNMPIMOD_COMM_P2P);
  err = PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source,
                              recvtag, comm, status);
  SEND_P2P_END(buf, count, datatype, dest, sendtag, comm, err, &sptr, NULL,
               PNMPIMOD_COMM_P2P);
  PMPI_Get_count(status, datatype, &count);
  RECV_P2P_END(buf, count, datatype, status->MPI_SOURCE, status->MPI_TAG, comm,
               status->MPI_ERROR, &rptr, NULL, PNMPIMOD_COMM_P2P, status, 1, 0);
  return err;
}


/*........................................................................*/
/* Waits */

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
  int err;
  void *mid2ptr;

  COMM_P2P_ASYNC_MID2(1, request, PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_ONE,
                      &mid2ptr);

  err = PMPI_Wait(request, status);

  if (COMM_REQ_FROM_STATUS(status).inreq != MPI_REQUEST_NULL)
    {
      if (COMM_REQ_FROM_STATUS(status).type == PNMPIMOD_REQUESTS_RECV)
        {
          int count;
          PMPI_Get_count(status, COMM_REQ_FROM_STATUS(status).datatype, &count);
          RECV_P2P_END(COMM_REQ_FROM_STATUS(status).buf, count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       status->MPI_SOURCE, status->MPI_TAG,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P, status, 1, 0);
        }
      else
        {
          SEND_P2P_END(COMM_REQ_FROM_STATUS(status).buf,
                       COMM_REQ_FROM_STATUS(status).count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       COMM_REQ_FROM_STATUS(status).node,
                       COMM_REQ_FROM_STATUS(status).tag,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P);
        }
    }
  COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_ONE);

  return err;
}

int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index,
                MPI_Status *status)
{
  int err;
  void *mid2ptr;

  COMM_P2P_ASYNC_MID2(count, array_of_requests,
                      PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_ANY, &mid2ptr);

  err = PMPI_Waitany(count, array_of_requests, index, status);
  if (COMM_REQ_FROM_STATUS(status).inreq != MPI_REQUEST_NULL)
    {
      if (COMM_REQ_FROM_STATUS(status).type == PNMPIMOD_REQUESTS_RECV)
        {
          int count;
          PMPI_Get_count(status, COMM_REQ_FROM_STATUS(status).datatype, &count);
          RECV_P2P_END(COMM_REQ_FROM_STATUS(status).buf, count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       status->MPI_SOURCE, status->MPI_TAG,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P, status, 1, 0);
        }
      else
        {
          SEND_P2P_END(COMM_REQ_FROM_STATUS(status).buf,
                       COMM_REQ_FROM_STATUS(status).count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       COMM_REQ_FROM_STATUS(status).node,
                       COMM_REQ_FROM_STATUS(status).tag,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P);
        }
    }
  COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_ANY);
  return err;
}

int MPI_Waitsome(int count, MPI_Request *array_of_requests, int *outcount,
                 int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err, i;
  void *mid2ptr;

  COMM_P2P_ASYNC_MID2(count, array_of_requests,
                      PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_SOME, &mid2ptr);

  err = PMPI_Waitsome(count, array_of_requests, outcount, array_of_indices,
                      array_of_statuses);
  for (i = 0; i < *outcount; i++)
    if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).inreq !=
        MPI_REQUEST_NULL)
      {
        if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).type ==
            PNMPIMOD_REQUESTS_RECV)
          {
            int c;
            PMPI_Get_count(
              &(array_of_statuses[i]),
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).datatype,
              &c);

            RECV_P2P_END(
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).buf, c,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).datatype,
              array_of_statuses[i].MPI_SOURCE, array_of_statuses[i].MPI_TAG,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).comm,
              array_of_statuses[i].MPI_ERROR,
              &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *, count,
                                           i)),
              &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P, array_of_statuses, count, i);
          }
        else
          {
            SEND_P2P_END(
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).buf,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).count,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).datatype,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).node,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).tag,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).comm,
              array_of_statuses[i].MPI_ERROR,
              &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *, count,
                                           i)),
              &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P);
          }
      }
  COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_SOME);
  return err;
}

int MPI_Waitall(int count, MPI_Request *array_of_requests,
                MPI_Status *array_of_statuses)
{
  int err, i;
  void *mid2ptr;

  if (count > 0)
    {
      COMM_P2P_ASYNC_MID2(count, array_of_requests,
                          PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_ALL, &mid2ptr);

      err = PMPI_Waitall(count, array_of_requests, array_of_statuses);

      for (i = 0; i < count; i++)
        if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).inreq !=
            MPI_REQUEST_NULL)
          {
            if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).type ==
                PNMPIMOD_REQUESTS_RECV)
              {
                int c;
                PMPI_Get_count(
                  &(array_of_statuses[i]),
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                    .datatype,
                  &c);
                RECV_P2P_END(
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).buf, c,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                    .datatype,
                  array_of_statuses[i].MPI_SOURCE, array_of_statuses[i].MPI_TAG,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).comm,
                  array_of_statuses[i].MPI_ERROR,
                  &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *, count,
                                               i)),
                  &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P, array_of_statuses, count,
                  i);
              }
            else
              {
                SEND_P2P_END(
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).buf,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).count,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                    .datatype,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).node,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).tag,
                  COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).comm,
                  array_of_statuses[i].MPI_ERROR,
                  &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *, count,
                                               i)),
                  &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P);
              }
          }
      COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_WAIT | PNMPIMOD_COMM_ALL);
    }
  else
    {
      err = PMPI_Waitall(count, array_of_requests, array_of_statuses);
    }
  return err;
}


/*........................................................................*/
/* Tests */

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  int err;
  void *mid2ptr;

  COMM_P2P_ASYNC_MID2(1, request, PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_ONE,
                      &mid2ptr);

  err = PMPI_Test(request, flag, status);
  if ((*flag) && (COMM_REQ_FROM_STATUS(status).inreq != MPI_REQUEST_NULL))
    {
      if (COMM_REQ_FROM_STATUS(status).type == PNMPIMOD_REQUESTS_RECV)
        {
          int count;
          PMPI_Get_count(status, COMM_REQ_FROM_STATUS(status).datatype, &count);
          RECV_P2P_END(COMM_REQ_FROM_STATUS(status).buf, count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       status->MPI_SOURCE, status->MPI_TAG,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P, status, 1, 0);
        }
      else
        {
          SEND_P2P_END(COMM_REQ_FROM_STATUS(status).buf,
                       COMM_REQ_FROM_STATUS(status).count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       COMM_REQ_FROM_STATUS(status).node,
                       COMM_REQ_FROM_STATUS(status).tag,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P);
        }
    }
  COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_ONE);
  return err;
}

int MPI_Testany(int count, MPI_Request *array_of_requests, int *index,
                int *flag, MPI_Status *status)
{
  int err;
  void *mid2ptr;

  COMM_P2P_ASYNC_MID2(count, array_of_requests,
                      PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_ANY, &mid2ptr);

  err = PMPI_Testany(count, array_of_requests, index, flag, status);
  if ((*flag) && (COMM_REQ_FROM_STATUS(status).inreq != MPI_REQUEST_NULL))
    {
      if (COMM_REQ_FROM_STATUS(status).type == PNMPIMOD_REQUESTS_RECV)
        {
          int count;
          PMPI_Get_count(status, COMM_REQ_FROM_STATUS(status).datatype, &count);
          RECV_P2P_END(COMM_REQ_FROM_STATUS(status).buf, count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       status->MPI_SOURCE, status->MPI_TAG,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P, status, 1, 0);
        }
      else
        {
          SEND_P2P_END(COMM_REQ_FROM_STATUS(status).buf,
                       COMM_REQ_FROM_STATUS(status).count,
                       COMM_REQ_FROM_STATUS(status).datatype,
                       COMM_REQ_FROM_STATUS(status).node,
                       COMM_REQ_FROM_STATUS(status).tag,
                       COMM_REQ_FROM_STATUS(status).comm, status->MPI_ERROR,
                       &(COMM_INFO_FROM_STATUS(status, void *)), &mid2ptr,
                       PNMPIMOD_COMM_ASYNC_P2P);
        }
    }
  COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_ANY);
  return err;
}

int MPI_Testsome(int count, MPI_Request *array_of_requests, int *outcount,
                 int *array_of_indices, MPI_Status *array_of_statuses)
{
  int err, i;
  void *mid2ptr;

  COMM_P2P_ASYNC_MID2(count, array_of_requests,
                      PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_SOME, &mid2ptr);

  err = PMPI_Testsome(count, array_of_requests, outcount, array_of_indices,
                      array_of_statuses);
  for (i = 0; i < *outcount; i++)
    if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).inreq !=
        MPI_REQUEST_NULL)
      {
        if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).type ==
            PNMPIMOD_REQUESTS_RECV)
          {
            int c;
            PMPI_Get_count(
              &(array_of_statuses[i]),
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).datatype,
              &c);
            RECV_P2P_END(
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).buf, c,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).datatype,
              array_of_statuses[i].MPI_SOURCE, array_of_statuses[i].MPI_TAG,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).comm,
              array_of_statuses[i].MPI_ERROR,
              &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *, count,
                                           i)),
              &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P, array_of_statuses, count, i);
          }
        else
          {
            SEND_P2P_END(
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).buf,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).count,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).datatype,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).node,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).tag,
              COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).comm,
              array_of_statuses[i].MPI_ERROR,
              &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *, count,
                                           i)),
              &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P);
          }
      }
  COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_SOME);
  return err;
}

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag,
                MPI_Status *array_of_statuses)
{
  int err, i;
  void *mid2ptr;

  if (count > 0)
    {
      COMM_P2P_ASYNC_MID2(count, array_of_requests,
                          PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_ALL, &mid2ptr);

      err = PMPI_Testall(count, array_of_requests, flag, array_of_statuses);
      if (*flag)
        {
          for (i = 0; i < count; i++)
            if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i).inreq !=
                MPI_REQUEST_NULL)
              {
                if (COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                      .type == PNMPIMOD_REQUESTS_RECV)
                  {
                    int c;
                    PMPI_Get_count(
                      &(array_of_statuses[i]),
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .datatype,
                      &c);
                    RECV_P2P_END(
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .buf,
                      c, COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                           .datatype,
                      array_of_statuses[i].MPI_SOURCE,
                      array_of_statuses[i].MPI_TAG,
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .comm,
                      array_of_statuses[i].MPI_ERROR,
                      &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *,
                                                   count, i)),
                      &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P, array_of_statuses,
                      count, i);
                  }
                else
                  {
                    SEND_P2P_END(
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .buf,
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .count,
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .datatype,
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .node,
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .tag,
                      COMM_REQ_FROM_STATUSARRAY(array_of_statuses, count, i)
                        .comm,
                      array_of_statuses[i].MPI_ERROR,
                      &(COMM_INFO_FROM_STATUSARRAY(array_of_statuses, void *,
                                                   count, i)),
                      &mid2ptr, PNMPIMOD_COMM_ASYNC_P2P);
                  }
              }
        }
      COMM_P2P_ASYNC_COMPLETION(PNMPIMOD_COMM_TEST | PNMPIMOD_COMM_ALL);
    }
  else
    {
      err = PMPI_Testall(count, array_of_requests, flag, array_of_statuses);
    }
  return err;
}


/*--------------------------------------------------------------------------*/
/* Collectives */

/*........................................................................*/
/* Barrier */

int MPI_Barrier(MPI_Comm comm)
{
  int err;
  void *cptr;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, -1,
                    PNMPIMOD_COMM_BARRIER | PNMPIMOD_COMM_COLL_BARRIER, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    {
      err = PMPI_Barrier(comm);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1, PNMPIMOD_COMM_BARRIER | PNMPIMOD_COMM_COLL_BARRIER,
                  &cptr);

  return err;
}



/*........................................................................*/
/* Bcast */

int MPI_Bcast(void *buf, int count, MPI_Datatype datatype, int root,
              MPI_Comm comm)
{
  int err;
  void *cptr;
  void **sptr = NULL, *rptr = NULL;
  int r, s, i;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, root, PNMPIMOD_COMM_BCAST | PNMPIMOD_COMM_COLL_FANOUT,
                    &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      if (r == root)
        sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (r == root)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(buf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                             comm, &(sptr[i]),
                             PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_BCAST);
            }
        }
      RECV_P2P_START(buf, count, datatype, root, PNMPIMOD_COMM_COLLTAG, comm,
                     &rptr, PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_BCAST);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Bcast(buf, count, datatype, root, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (r == root)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(buf, count, datatype, i, PNMPIMOD_COMM_COLLTAG, comm,
                           err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_BCAST);
            }
        }
      RECV_P2P_END(buf, count, datatype, root, PNMPIMOD_COMM_COLLTAG, comm, err,
                   &rptr, NULL, PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_BCAST,
                   NULL, 0, -1);

      if ((sptr != NULL) && (r == root))
        free(sptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, root, PNMPIMOD_COMM_BCAST | PNMPIMOD_COMM_COLL_FANOUT,
                  &cptr);

  return err;
}



/*........................................................................*/
/* Scatter */

int MPI_Scatter(PNMPI_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
                MPI_Comm comm)
{
  int err;
  void *cptr;
  void **sptr = NULL, *rptr = NULL;
  int r, s, i;
  MPI_Aint e = 0;


  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, root,
                    PNMPIMOD_COMM_SCATTER | PNMPIMOD_COMM_COLL_FANOUT, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      if (r == root)
        sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_START(sendbuf, sendcount, sendtype, i,
                                 PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                                 PNMPIMOD_COMM_COLL_FANOUT |
                                   PNMPIMOD_COMM_SCATTER);
                }
            }
          RECV_P2P_START(recvbuf, recvcount, recvtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &rptr,
                         PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTER);
        }
      else
        {
          PMPI_Type_extent(sendtype, &e);

          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_START(((void *)(((char *)sendbuf) + i * e)),
                                 sendcount, sendtype, i, PNMPIMOD_COMM_COLLTAG,
                                 comm, &(sptr[i]), PNMPIMOD_COMM_COLL_FANOUT |
                                                     PNMPIMOD_COMM_SCATTER);
                }
            }
          RECV_P2P_START(recvbuf, recvcount, recvtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &rptr,
                         PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTER);
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount,
                       recvtype, root, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_END(sendbuf, sendcount, sendtype, i,
                               PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]),
                               NULL, PNMPIMOD_COMM_COLL_FANOUT |
                                       PNMPIMOD_COMM_SCATTER);
                }
            }
          RECV_P2P_END(recvbuf, recvcount, recvtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &rptr, NULL,
                       PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTER, NULL,
                       0, -1);
        }
      else
        {
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_END(((void *)(((char *)sendbuf) + e * i)), sendcount,
                               sendtype, i, PNMPIMOD_COMM_COLLTAG, comm, err,
                               &(sptr[i]), NULL, PNMPIMOD_COMM_COLL_FANOUT |
                                                   PNMPIMOD_COMM_SCATTER);
                }
            }
          RECV_P2P_END(recvbuf, recvcount, recvtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &rptr, NULL,
                       PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTER, NULL,
                       0, -1);
        }

      if ((sptr != NULL) && (r == root))
        free(sptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, root, PNMPIMOD_COMM_SCATTER | PNMPIMOD_COMM_COLL_FANOUT,
                  &cptr);

  return err;
}



/*........................................................................*/
/* Scatterv */

int MPI_Scatterv(PNMPI_CONST void *sendbuf, PNMPI_CONST int *sendcounts,
                 PNMPI_CONST int *displs, MPI_Datatype sendtype, void *recvbuf,
                 int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
  int err;
  void *cptr;
  void **sptr = NULL, *rptr = NULL;
  int r, s, i;
  MPI_Aint e = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, root,
                    PNMPIMOD_COMM_SCATTERV | PNMPIMOD_COMM_COLL_FANOUT, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      if (r == root)
        sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_START(sendbuf, sendcounts[i], sendtype, i,
                                 PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                                 PNMPIMOD_COMM_COLL_FANOUT |
                                   PNMPIMOD_COMM_SCATTERV);
                }
            }
          RECV_P2P_START(recvbuf, recvcount, recvtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &rptr,
                         PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTERV);
        }
      else
        {
          PMPI_Type_extent(sendtype, &e);

          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_START(((void *)(((char *)sendbuf) + e * displs[i])),
                                 sendcounts[i], sendtype, i,
                                 PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                                 PNMPIMOD_COMM_COLL_FANOUT |
                                   PNMPIMOD_COMM_SCATTERV);
                }
            }
          RECV_P2P_START(recvbuf, recvcount, recvtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &rptr,
                         PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTERV);
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf,
                        recvcount, recvtype, root, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_END(
                    ((void *)(((char *)sendbuf) + displs[i])), sendcounts[i],
                    sendtype, i, PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]),
                    NULL, PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTERV);
                }
            }
          RECV_P2P_END(recvbuf, recvcount, recvtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &rptr, NULL,
                       PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTERV, NULL,
                       0, -1);
        }
      else
        {
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  SEND_P2P_END(((void *)(((char *)sendbuf) + e * displs[i])),
                               sendcounts[i], sendtype, i,
                               PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]),
                               NULL, PNMPIMOD_COMM_COLL_FANOUT |
                                       PNMPIMOD_COMM_SCATTERV);
                }
            }
          RECV_P2P_END(recvbuf, recvcount, recvtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &rptr, NULL,
                       PNMPIMOD_COMM_COLL_FANOUT | PNMPIMOD_COMM_SCATTERV, NULL,
                       0, -1);
        }
      if ((sptr != NULL) && (r == root))
        free(sptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, root,
                  PNMPIMOD_COMM_SCATTERV | PNMPIMOD_COMM_COLL_FANOUT, &cptr);

  return err;
}


/*........................................................................*/
/* Gather */

int MPI_Gather(PNMPI_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
               MPI_Comm comm)
{
  int err;
  void *cptr;
  void **rptr = NULL, *sptr = NULL;
  int r, s, i;
  MPI_Aint e = 0;


  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, root, PNMPIMOD_COMM_GATHER | PNMPIMOD_COMM_COLL_FANIN,
                    &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      if (r == root)
        rptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          SEND_P2P_START(sendbuf, sendcount, sendtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &(sptr),
                         PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHER);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_START(recvbuf, recvcount, recvtype, i,
                                 PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                                 PNMPIMOD_COMM_COLL_FANIN |
                                   PNMPIMOD_COMM_GATHER);
                }
            }
        }
      else
        {
          PMPI_Type_extent(recvtype, &e);

          SEND_P2P_START(sendbuf, sendcount, sendtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &(sptr),
                         PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHER);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_START(((void *)(((char *)recvbuf) + i * e)),
                                 recvcount, recvtype, i, PNMPIMOD_COMM_COLLTAG,
                                 comm, &(rptr[i]), PNMPIMOD_COMM_COLL_FANIN |
                                                     PNMPIMOD_COMM_GATHER);
                }
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount,
                      recvtype, root, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          SEND_P2P_END(sendbuf, sendcount, sendtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr), NULL,
                       PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHER);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_END(recvbuf, recvcount, recvtype, i,
                               PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]),
                               NULL,
                               PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHER,
                               NULL, 0, -1);
                }
            }
        }
      else
        {
          SEND_P2P_END(sendbuf, sendcount, sendtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr), NULL,
                       PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHER);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_END(((void *)(((char *)recvbuf) + i * e)), recvcount,
                               recvtype, i, PNMPIMOD_COMM_COLLTAG, comm, err,
                               &(rptr[i]), NULL,
                               PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHER,
                               NULL, 0, -1);
                }
            }
        }

      if ((rptr != NULL) && (r == root))
        free(rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, root, PNMPIMOD_COMM_GATHER | PNMPIMOD_COMM_COLL_FANIN,
                  &cptr);

  return err;
}



/*........................................................................*/
/* Gatherv */

int MPI_Gatherv(PNMPI_CONST void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, PNMPI_CONST int *recvcounts,
                PNMPI_CONST int *displs, MPI_Datatype recvtype, int root,
                MPI_Comm comm)
{
  int err;
  void *cptr;
  void **rptr = NULL, *sptr = NULL;
  int r, s, i;
  MPI_Aint e = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, root,
                    PNMPIMOD_COMM_GATHERV | PNMPIMOD_COMM_COLL_FANIN, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      if (r == root)
        rptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          SEND_P2P_START(sendbuf, sendcount, sendtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &(sptr),
                         PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHERV);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_START(recvbuf, recvcounts[i], recvtype, i,
                                 PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                                 PNMPIMOD_COMM_COLL_FANIN |
                                   PNMPIMOD_COMM_GATHERV);
                }
            }
        }
      else
        {
          PMPI_Type_extent(recvtype, &e);

          SEND_P2P_START(sendbuf, sendcount, sendtype, root,
                         PNMPIMOD_COMM_COLLTAG, comm, &(sptr),
                         PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHERV);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_START(((void *)(((char *)recvbuf) + e * displs[i])),
                                 recvcounts[i], recvtype, i,
                                 PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                                 PNMPIMOD_COMM_COLL_FANIN |
                                   PNMPIMOD_COMM_GATHERV);
                }
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts,
                       displs, recvtype, root, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          SEND_P2P_END(sendbuf, sendcount, sendtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr), NULL,
                       PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHERV);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_END(recvbuf, recvcounts[i], recvtype, i,
                               PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]),
                               NULL,
                               PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHERV,
                               NULL, 0, -1);
                }
            }
        }
      else
        {
          SEND_P2P_END(sendbuf, sendcount, sendtype, root,
                       PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr), NULL,
                       PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHERV);
          if (r == root)
            {
              for (i = 0; i < s; i++)
                {
                  RECV_P2P_END(((void *)(((char *)recvbuf) + e * displs[i])),
                               recvcounts[i], recvtype, i,
                               PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]),
                               NULL,
                               PNMPIMOD_COMM_COLL_FANIN | PNMPIMOD_COMM_GATHERV,
                               NULL, 0, -1);
                }
            }
        }

      if ((rptr != NULL) && (r == root))
        free(rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, root, PNMPIMOD_COMM_GATHERV | PNMPIMOD_COMM_COLL_FANIN,
                  &cptr);

  return err;
}



/*........................................................................*/
/* Allgather */

int MPI_Allgather(PNMPI_CONST void *sendbuf, int sendcount,
                  MPI_Datatype sendtype, void *recvbuf, int recvcount,
                  MPI_Datatype recvtype, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, s, i;
  void **sptr = NULL, **rptr = NULL;
  MPI_Aint e = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(
      comm, -1, PNMPIMOD_COMM_ALLGATHER | PNMPIMOD_COMM_COLL_ALL2ALL, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      rptr = (void **)malloc(sizeof(void *) * s);
      sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, sendcount, sendtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALLGATHER);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, recvcount, recvtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALLGATHER);
            }
        }
      else
        {
          PMPI_Type_extent(recvtype, &e);

          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, sendcount, sendtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALLGATHER);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(((void *)(((char *)recvbuf) + i * e)), recvcount,
                             recvtype, i, PNMPIMOD_COMM_COLLTAG, comm,
                             &(rptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL |
                                           PNMPIMOD_COMM_ALLGATHER);
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    {
      err = PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount,
                           recvtype, comm);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(sendbuf, sendcount, sendtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL |
                             PNMPIMOD_COMM_ALLGATHER);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(recvbuf, recvcount, recvtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALLGATHER,
                           NULL, 0, -1);
            }
        }
      else
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(sendbuf, sendcount, sendtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL |
                             PNMPIMOD_COMM_ALLGATHER);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(((void *)(((char *)recvbuf) + i * e)), recvcount,
                           recvtype, i, PNMPIMOD_COMM_COLLTAG, comm, err,
                           &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALLGATHER,
                           NULL, 0, -1);
            }
        }

      if (sptr != NULL)
        free(sptr);
      if (rptr != NULL)
        free(rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1,
                  PNMPIMOD_COMM_ALLGATHER | PNMPIMOD_COMM_COLL_ALL2ALL, &cptr);

  return err;
}



/*........................................................................*/
/* Allgatherv */

int MPI_Allgatherv(PNMPI_CONST void *sendbuf, int sendcount,
                   MPI_Datatype sendtype, void *recvbuf,
                   PNMPI_CONST int *recvcounts, PNMPI_CONST int *displs,
                   MPI_Datatype recvtype, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, s, i;
  void **sptr = NULL, **rptr = NULL;
  MPI_Aint e = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(
      comm, -1, PNMPIMOD_COMM_ALLGATHERV | PNMPIMOD_COMM_COLL_ALL2ALL, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      rptr = (void **)malloc(sizeof(void *) * s);
      sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, sendcount, sendtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALLGATHERV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, recvcounts[i], recvtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALLGATHERV);
            }
        }
      else
        {
          PMPI_Type_extent(recvtype, &e);

          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, sendcount, sendtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALLGATHERV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(((void *)(((char *)recvbuf) + e * displs[i])),
                             recvcounts[i], recvtype, i, PNMPIMOD_COMM_COLLTAG,
                             comm, &(rptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL |
                                                 PNMPIMOD_COMM_ALLGATHERV);
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts,
                          displs, recvtype, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(sendbuf, sendcount, sendtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL |
                             PNMPIMOD_COMM_ALLGATHERV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(recvbuf, recvcounts[i], recvtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL |
                             PNMPIMOD_COMM_ALLGATHERV,
                           NULL, 0, -1);
            }
        }
      else
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(sendbuf, sendcount, sendtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL |
                             PNMPIMOD_COMM_ALLGATHERV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(
                ((void *)(((char *)recvbuf) + e * displs[i])), recvcounts[i],
                recvtype, i, PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]), NULL,
                PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALLGATHERV, NULL, 0,
                -1);
            }
        }

      if (sptr != NULL)
        free(sptr);
      if (rptr != NULL)
        free(rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1,
                  PNMPIMOD_COMM_ALLGATHERV | PNMPIMOD_COMM_COLL_ALL2ALL, &cptr);

  return err;
}



/*........................................................................*/
/* Alltoall */

int MPI_Alltoall(PNMPI_CONST void *sendbuf, int sendcount,
                 MPI_Datatype sendtype, void *recvbuf, int recvcount,
                 MPI_Datatype recvtype, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, s, i;
  void **sptr = NULL, **rptr = NULL;
  MPI_Aint re = 0, se = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, -1,
                    PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      rptr = (void **)malloc(sizeof(void *) * s);
      sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(
                sendbuf, sendcount, sendtype, i, PNMPIMOD_COMM_COLLTAG, comm,
                &(sptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(
                recvbuf, recvcount, recvtype, i, PNMPIMOD_COMM_COLLTAG, comm,
                &(rptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL);
            }
        }
      else
        {
          PMPI_Type_extent(sendtype, &se);
          PMPI_Type_extent(recvtype, &re);

          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(((void *)(((char *)sendbuf) + i * se)), sendcount,
                             sendtype, i, PNMPIMOD_COMM_COLLTAG, comm,
                             &(sptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL |
                                           PNMPIMOD_COMM_ALL2ALL);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(((void *)(((char *)recvbuf) + i * re)), recvcount,
                             recvtype, i, PNMPIMOD_COMM_COLLTAG, comm,
                             &(rptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL |
                                           PNMPIMOD_COMM_ALL2ALL);
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount,
                        recvtype, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(sendbuf, sendcount, sendtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(recvbuf, recvcount, recvtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL,
                           NULL, 0, -1);
            }
        }
      else
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(((void *)(((char *)sendbuf) + i * se)), sendcount,
                           sendtype, i, PNMPIMOD_COMM_COLLTAG, comm, err,
                           &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(((void *)(((char *)recvbuf) + i * re)), recvcount,
                           recvtype, i, PNMPIMOD_COMM_COLLTAG, comm, err,
                           &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL,
                           NULL, 0, -1);
            }
        }

      if (sptr != NULL)
        free(sptr);
      if (rptr != NULL)
        free(rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1, PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALL,
                  &cptr);

  return err;
}



/*........................................................................*/
/* alltoallv */
// MPI_Alltoallv(const void *, const int *, const int *, MPI_Datatype={int},
// void *, const int *, const int *, MPI_Datatype={int}, MPI_Comm={int}
int MPI_Alltoallv(PNMPI_CONST void *sendbuf, PNMPI_CONST int *sendcounts,
                  PNMPI_CONST int *sdispls, MPI_Datatype sendtype,
                  void *recvbuf, PNMPI_CONST int *recvcounts,
                  PNMPI_CONST int *rdispls, MPI_Datatype recvtype,
                  MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, s, i;
  void **sptr = NULL, **rptr = NULL;
  MPI_Aint re = 0, se = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, -1,
                    PNMPIMOD_COMM_ALL2ALLV | PNMPIMOD_COMM_COLL_ALL2ALL, &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      rptr = (void **)malloc(sizeof(void *) * s);
      sptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, sendcounts[i], sendtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALL2ALLV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, recvcounts[i], recvtype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_ALL2ALLV);
            }
        }
      else
        {
          PMPI_Type_extent(sendtype, &se);
          PMPI_Type_extent(recvtype, &re);

          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(((void *)(((char *)sendbuf) + se * sdispls[i])),
                             sendcounts[i], sendtype, i, PNMPIMOD_COMM_COLLTAG,
                             comm, &(sptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL |
                                                 PNMPIMOD_COMM_ALL2ALLV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(((void *)(((char *)recvbuf) + re * rdispls[i])),
                             recvcounts[i], recvtype, i, PNMPIMOD_COMM_COLLTAG,
                             comm, &(rptr[i]), PNMPIMOD_COMM_COLL_ALL2ALL |
                                                 PNMPIMOD_COMM_ALL2ALLV);
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf,
                         recvcounts, rdispls, recvtype, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(sendbuf, sendcounts[i], sendtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALLV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(recvbuf, recvcounts[i], recvtype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALLV,
                           NULL, 0, -1);
            }
        }
      else
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(((void *)(((char *)sendbuf) + se * sdispls[i])),
                           sendcounts[i], sendtype, i, PNMPIMOD_COMM_COLLTAG,
                           comm, err, &(sptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALLV);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(((void *)(((char *)recvbuf) + re * rdispls[i])),
                           recvcounts[i], recvtype, i, PNMPIMOD_COMM_COLLTAG,
                           comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALLV,
                           NULL, 0, -1);
            }
        }

      if (sptr != NULL)
        free(sptr);
      if (rptr != NULL)
        free(rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1, PNMPIMOD_COMM_COLL_ALL2ALL | PNMPIMOD_COMM_ALL2ALLV,
                  &cptr);

  return err;
}



/*........................................................................*/
/* Reduce */

int MPI_Reduce(PNMPI_CONST void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, i, s;
  void **rptr = NULL, *sptr = NULL;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, root,
                    PNMPIMOD_COMM_REDUCE | PNMPIMOD_COMM_COLL_FANIN |
                      PNMPIMOD_COMM_COLL_REDUCE,
                    &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      rptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      SEND_P2P_START(sendbuf, count, datatype, root, PNMPIMOD_COMM_COLLTAG,
                     comm, &sptr,
                     PNMPIMOD_COMM_REDUCE | PNMPIMOD_COMM_COLL_FANIN |
                       PNMPIMOD_COMM_COLL_REDUCE);

      if (r == root)
        {
          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                             comm, &(rptr[i]),
                             PNMPIMOD_COMM_REDUCE | PNMPIMOD_COMM_COLL_FANIN |
                               PNMPIMOD_COMM_COLL_REDUCE);
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      SEND_P2P_END(sendbuf, count, datatype, root, PNMPIMOD_COMM_COLLTAG, comm,
                   err, &sptr, NULL,
                   PNMPIMOD_COMM_REDUCE | PNMPIMOD_COMM_COLL_FANIN |
                     PNMPIMOD_COMM_COLL_REDUCE);

      if (r == root)
        {
          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(recvbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                           comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_REDUCE | PNMPIMOD_COMM_COLL_FANIN |
                             PNMPIMOD_COMM_COLL_REDUCE,
                           NULL, 0, -1);
            }
        }
      if (rptr != NULL)
        free(rptr);
    }

  if (r == root)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
        COMM_COLL_REDUCE(recvbuf, count, datatype, op, s, rptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, root, PNMPIMOD_COMM_REDUCE | PNMPIMOD_COMM_COLL_FANIN |
                                PNMPIMOD_COMM_COLL_REDUCE,
                  &cptr);

  return err;
}


/*........................................................................*/
/* allreduce */

int MPI_Allreduce(PNMPI_CONST void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, i, s;
  void **rptr = NULL, **sptr = NULL;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, -1,
                    PNMPIMOD_COMM_ALLREDUCE | PNMPIMOD_COMM_COLL_ALL2ALL |
                      PNMPIMOD_COMM_COLL_REDUCE,
                    &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      sptr = (void **)malloc(sizeof(void *) * s);
      rptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      for (i = 0; i < s; i++)
        {
          SEND_P2P_START(sendbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                         comm, &(sptr[i]),
                         PNMPIMOD_COMM_ALLREDUCE | PNMPIMOD_COMM_COLL_ALL2ALL |
                           PNMPIMOD_COMM_COLL_REDUCE);
        }

      for (i = 0; i < s; i++)
        {
          RECV_P2P_START(recvbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                         comm, &(rptr[i]),
                         PNMPIMOD_COMM_ALLREDUCE | PNMPIMOD_COMM_COLL_ALL2ALL |
                           PNMPIMOD_COMM_COLL_REDUCE);
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      for (i = 0; i < s; i++)
        {
          SEND_P2P_END(sendbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG, comm,
                       err, &(sptr[i]), NULL,
                       PNMPIMOD_COMM_ALLREDUCE | PNMPIMOD_COMM_COLL_ALL2ALL |
                         PNMPIMOD_COMM_COLL_REDUCE);
        }

      for (i = 0; i < s; i++)
        {
          RECV_P2P_END(recvbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG, comm,
                       err, &(rptr[i]), NULL,
                       PNMPIMOD_COMM_ALLREDUCE | PNMPIMOD_COMM_COLL_ALL2ALL |
                         PNMPIMOD_COMM_COLL_REDUCE,
                       NULL, 0, -1);
        }

      if (rptr != NULL)
        free(rptr);
      if (sptr != NULL)
        free(sptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    COMM_COLL_REDUCE(recvbuf, count, datatype, op, s, rptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1,
                  PNMPIMOD_COMM_ALLREDUCE | PNMPIMOD_COMM_COLL_ALL2ALL |
                    PNMPIMOD_COMM_COLL_REDUCE,
                  &cptr);

  return err;
}



/*........................................................................*/
/* reduce/scatter */

int MPI_Reduce_scatter(PNMPI_CONST void *sendbuf, void *recvbuf,
                       PNMPI_CONST int *recvcounts, MPI_Datatype datatype,
                       MPI_Op op, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, i, s, sum;
  void **rptr = NULL, **sptr = NULL;
  MPI_Aint e = 0;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, -1,
                    PNMPIMOD_COMM_REDUCESCATTER | PNMPIMOD_COMM_COLL_ALL2ALL |
                      PNMPIMOD_COMM_COLL_REDUCE,
                    &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      sptr = (void **)malloc(sizeof(void *) * s);
      rptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, recvcounts[i], datatype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_REDUCESCATTER |
                               PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_COLL_REDUCE);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, recvcounts[i], datatype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                             PNMPIMOD_COMM_REDUCESCATTER |
                               PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_COLL_REDUCE);
            }
        }
      else
        {
          PMPI_Type_extent(datatype, &e);

          sum = 0;
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(((void *)(((char *)sendbuf) + sum * e)),
                             recvcounts[i], datatype, i, PNMPIMOD_COMM_COLLTAG,
                             comm, &(sptr[i]), PNMPIMOD_COMM_REDUCESCATTER |
                                                 PNMPIMOD_COMM_COLL_ALL2ALL |
                                                 PNMPIMOD_COMM_COLL_REDUCE);
              sum += recvcounts[i];
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, recvcounts[i], datatype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                             PNMPIMOD_COMM_REDUCESCATTER |
                               PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_COLL_REDUCE);
            }
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_IGNOREDATA)
        {
          for (i = 0; i < s; i++)
            {
              SEND_P2P_START(sendbuf, recvcounts[i], datatype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(sptr[i]),
                             PNMPIMOD_COMM_REDUCESCATTER |
                               PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_COLL_REDUCE);
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_START(recvbuf, recvcounts[i], datatype, i,
                             PNMPIMOD_COMM_COLLTAG, comm, &(rptr[i]),
                             PNMPIMOD_COMM_REDUCESCATTER |
                               PNMPIMOD_COMM_COLL_ALL2ALL |
                               PNMPIMOD_COMM_COLL_REDUCE);
            }
        }
      else
        {
          sum = 0;
          for (i = 0; i < s; i++)
            {
              SEND_P2P_END(
                ((void *)(((char *)sendbuf) + sum * e)), recvcounts[i],
                datatype, i, PNMPIMOD_COMM_COLLTAG, comm, err, &(sptr[i]), NULL,
                PNMPIMOD_COMM_REDUCESCATTER | PNMPIMOD_COMM_COLL_ALL2ALL |
                  PNMPIMOD_COMM_COLL_REDUCE);
              sum += recvcounts[i];
            }

          for (i = 0; i < s; i++)
            {
              RECV_P2P_END(recvbuf, recvcounts[i], datatype, i,
                           PNMPIMOD_COMM_COLLTAG, comm, err, &(rptr[i]), NULL,
                           PNMPIMOD_COMM_REDUCESCATTER |
                             PNMPIMOD_COMM_COLL_ALL2ALL |
                             PNMPIMOD_COMM_COLL_REDUCE,
                           NULL, 0, -1);
            }
        }

      if (rptr != NULL)
        free(rptr);
      if (sptr != NULL)
        free(sptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    COMM_COLL_REDUCE(recvbuf, recvcounts[i], datatype, op, s, rptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1,
                  PNMPIMOD_COMM_REDUCESCATTER | PNMPIMOD_COMM_COLL_ALL2ALL |
                    PNMPIMOD_COMM_COLL_REDUCE,
                  &cptr);

  return err;
}



/*........................................................................*/
/* scan */

int MPI_Scan(PNMPI_CONST void *sendbuf, void *recvbuf, int count,
             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
  int err;
  void *cptr;
  int r, i, s;
  void **rptr = NULL, **sptr = NULL;

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_START(comm, -1, PNMPIMOD_COMM_SCAN | PNMPIMOD_COMM_COLL_OTHER |
                                PNMPIMOD_COMM_COLL_REDUCE,
                    &cptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      PMPI_Comm_rank(comm, &r);
      PMPI_Comm_size(comm, &s);
      sptr = (void **)malloc(sizeof(void *) * s);
      rptr = (void **)malloc(sizeof(void *) * s);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      for (i = r; i < s; i++)
        {
          SEND_P2P_START(sendbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                         comm, &(sptr[i]),
                         PNMPIMOD_COMM_SCAN | PNMPIMOD_COMM_COLL_OTHER |
                           PNMPIMOD_COMM_COLL_REDUCE);
        }

      for (i = 0; i <= r; i++)
        {
          RECV_P2P_START(recvbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG,
                         comm, &(rptr[i]),
                         PNMPIMOD_COMM_SCAN | PNMPIMOD_COMM_COLL_OTHER |
                           PNMPIMOD_COMM_COLL_REDUCE);
        }
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_REPLACECOLLECTIVE)
    err = MPI_SUCCESS;
  else
    err = PMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    {
      for (i = r; i < s; i++)
        {
          SEND_P2P_END(sendbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG, comm,
                       err, &(sptr[i]), NULL,
                       PNMPIMOD_COMM_SCAN | PNMPIMOD_COMM_COLL_OTHER |
                         PNMPIMOD_COMM_COLL_REDUCE);
        }

      for (i = 0; i <= r; i++)
        {
          RECV_P2P_END(recvbuf, count, datatype, i, PNMPIMOD_COMM_COLLTAG, comm,
                       err, &(rptr[i]), NULL,
                       PNMPIMOD_COMM_SCAN | PNMPIMOD_COMM_COLL_OTHER |
                         PNMPIMOD_COMM_COLL_REDUCE,
                       NULL, 0, -1);
        }

      if (rptr != NULL)
        free(rptr);
      if (sptr != NULL)
        free(sptr);
    }

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_DISSOLVECOLLECTIVE)
    COMM_COLL_REDUCE(recvbuf, count, datatype, op, r + 1, rptr);

  if (pnmpimod_comm_collective_support & PNMPIMOD_COMM_KEEPCOLLECTIVE)
    COMM_COLL_END(comm, -1, PNMPIMOD_COMM_SCAN | PNMPIMOD_COMM_COLL_OTHER |
                              PNMPIMOD_COMM_COLL_REDUCE,
                  &cptr);

  return err;
}



/*--------------------------------------------------------------------------*/
/* The End. */
