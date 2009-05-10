#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int MPI_Send(void* buf, int num, MPI_Datatype dtype, int node, 
	     int tag, MPI_Comm comm)
{
  int res;

  printf("WRAPPER 2: Before send\n");
  fflush(stdout);

  res=PMPI_Send(buf,num,dtype,node,tag,comm);

  printf("WRAPPER 2: After send\n");
  fflush(stdout);

  return res;
}

