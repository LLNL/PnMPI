!     This file is part of P^nMPI.
!
!     Copyright (c)
!      2008-2016 Lawrence Livermore National Laboratories, United States of
!                 America
!      2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of
!                 Germany
!      2013-2016 RWTH Aachen University, Federal Republic of Germany
!
!
!     P^nMPI is free software; you can redistribute it and/or modify it under
!     the terms of the GNU Lesser General Public License as published by the
!     Free Software Foundation version 2.1 dated February 1999.
!
!     P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
!     WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!     FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
!     more details.
!
!     You should have received a copy of the GNU Lesser General Public License
!     along with P^nMPI; if not, write to the
!
!       Free Software Foundation, Inc.
!       51 Franklin St, Fifth Floor
!       Boston, MA 02110, USA
!
!
!     Written by Martin Schulz, schulzm@llnl.gov.
!
!     LLNL-CODE-402774

program firstmpi

  include 'mpif.h'

  integer :: mpierror, mpisize, mpirank, target, incoming
  Integer status(MPI_STATUS_SIZE)


  call MPI_Init(mpierror)
  call MPI_Comm_size(MPI_COMM_WORLD, mpisize, mpierror)
  call MPI_Comm_rank(MPI_COMM_WORLD, mpirank, mpierror)

  target = MOD( mpirank + 1 , mpisize )
!     print *, "Message from ", mpirank, " to ", target

  call MPI_Send(mpirank,1,MPI_INTEGER,target,42,MPI_COMM_WORLD, mpierror)
  call MPI_Recv(incoming,1,MPI_INTEGER,MPI_ANY_SOURCE,42,MPI_COMM_WORLD,status,mpierror)

  print *, "Message from ",status(MPI_SOURCE),"/",incoming, " to ",mpirank

  call MPI_Finalize(mpierror)

end program firstmpi
