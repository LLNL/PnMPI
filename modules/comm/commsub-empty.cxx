#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "commsub.h"

void COMM_ALL_INIT(int argc, char**argv) 
{
}

void COMM_ALL_FINALIZE() 
{
}

void SEND_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag, 
		    MPI_Comm comm, void **ptr, int type) 
{
}

void SEND_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node, int tag, 
			 MPI_Comm comm, void **ptr, int type) 
{
}

void SEND_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag, 
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
		  MPI_Comm comm, int err, void **ptr, void **midptr, int type) 
{
}

void COMM_P2P_ASYNC_MID2(int count, MPI_Request *requests, int flag, void **midptr) 
{
}

void COMM_P2P_ASYNC_COMPLETION(int flag) 
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

