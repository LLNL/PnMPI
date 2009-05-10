      program firstmpi

      include 'mpif.h'

      integer :: mpierror, mpisize, mpirank, target, incoming
      Integer status(MPI_STATUS_SIZE)


      call MPI_Init(mpierror)
      call MPI_Comm_size(MPI_COMM_WORLD, mpisize, mpierror)
      call MPI_Comm_rank(MPI_COMM_WORLD, mpirank, mpierror)
      
      target = MOD( mpirank + 1 , mpisize )
C     print *, "Message from ", mpirank, " to ", target 

      call MPI_Send(mpirank,1,MPI_INTEGER,target,42,MPI_COMM_WORLD,
     2              mpierror)
      call MPI_Recv(incoming,1,MPI_INTEGER,MPI_ANY_SOURCE,42,
     2              MPI_COMM_WORLD,status,mpierror)
      
      print *, "Message from ",status(MPI_SOURCE),"/",incoming,
     2      " to ",mpirank

      call MPI_Finalize(mpierror)

      end program firstmpi
