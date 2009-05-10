#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int MPI_Send(void* buf, int num, MPI_Datatype dtype, int node, 
	     int tag, MPI_Comm comm)
{
  int res;

  printf("WRAPPER 1: Before send\n");
  fflush(stdout);

  res=PMPI_Send(buf,num,dtype,node,tag,comm);

  printf("WRAPPER 1: After send\n");
  fflush(stdout);

  return res;
}

int MPI_Recv(void* buf, int num, MPI_Datatype dtype, int node,
             int tag, MPI_Comm comm, MPI_Status *status)
{
  int res;

  printf("WRAPPER 1: Before recv\n");
  fflush(stdout);

  res=PMPI_Recv(buf,num,dtype,node,tag,comm,status);

  printf("WRAPPER 1: After recv\n");
  fflush(stdout);  
  
  return res;
}

