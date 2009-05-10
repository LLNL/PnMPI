#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int MPI_Recv(void* buf, int num, MPI_Datatype dtype, int node,
             int tag, MPI_Comm comm, MPI_Status *status)
{
  int res;

  printf("WRAPPER 3: Before recv\n");
  fflush(stdout);

  res=PMPI_Recv(buf,num,dtype,node,tag,comm,status);

  printf("WRAPPER 3: After recv\n");
  fflush(stdout);  
  
  return res;
}

