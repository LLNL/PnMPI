!     Copyright (c) 2008
!     Lawrence Livermore National Security, LLC.
!
!     Produced at the Lawrence Livermore National Laboratory.
!     Written by Martin Schulz, schulzm@llnl.gov.
!     LLNL-CODE-402774,
!     All rights reserved.
!
!     This file is part of P^nMPI.
!
!     Please also read the file "LICENSE" included in this package
!     for Our Notice and GNU Lesser General Public License.
!
!     This program is free software; you can redistribute it and/or
!     modify it under the terms of the GNU General Public License
!     (as published by the Free Software Foundation) version 2.1
!     dated February 1999.
!
!     This program is distributed in the hope that it will be useful,
!     but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY
!     OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
!     terms and conditions of the GNU General Public License for more
!     details.

!     You should have received a copy of the GNU Lesser General Public
!     License along with this program; if not, write to the

!     Free Software Foundation, Inc.,
!     Temple Place, Suite 330,
!     Boston, MA 02111-1307 USA

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
