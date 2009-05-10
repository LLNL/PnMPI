#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(int argc, char **argv)
{
  int rank,size,i,buf[5],err;
  MPI_Status status;

  err=MPI_Init(&argc,&argv);
  if (err!=MPI_SUCCESS)
    {
      printf("ERROR IN INIT: %i\n",err);
      exit(1);
    }

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank==0)
    {
      for (i=1; i<size; i++)
	{
	  MPI_Recv(buf,5,MPI_INT,MPI_ANY_SOURCE,42,MPI_COMM_WORLD,&status);
	  MPI_Send(buf,5,MPI_INT,status.MPI_SOURCE,42,MPI_COMM_WORLD);
	  printf("GOT %i\n",status.MPI_SOURCE);
	}
    }
  else
    {
      MPI_Send(buf,5,MPI_INT,0,42,MPI_COMM_WORLD);
      MPI_Recv(buf,5,MPI_INT,MPI_ANY_SOURCE,42,MPI_COMM_WORLD,&status);
    }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
