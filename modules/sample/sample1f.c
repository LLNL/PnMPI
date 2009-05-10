#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

void mpi_send_(void* buf, int *num, MPI_Datatype *dtype, int *node, 
	      int *tag, MPI_Comm *comm, int *ierr)
{
  printf("WRAPPER 1: Before send\n");
  fflush(stdout);

  *ierr=PMPI_Send(buf,*num,*dtype,*node,*tag,*comm);

  printf("WRAPPER 1: After send\n");
  fflush(stdout);

  return;
}

void mpi_recv_(void* buf, int *num, MPI_Datatype *dtype, int *node,
	      int *tag, MPI_Comm *comm, MPI_Status *status, int *ierr)
{
  printf("WRAPPER 1: Before recv\n");
  fflush(stdout);

  *ierr=PMPI_Recv(buf,*num,*dtype,*node,*tag,*comm,status);

  printf("WRAPPER 1: After recv\n");
  fflush(stdout);  
  
  return;
}

