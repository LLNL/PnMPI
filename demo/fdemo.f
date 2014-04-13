C     Copyright (c) 2008
C     Lawrence Livermore National Security, LLC.
C
C     Produced at the Lawrence Livermore National Laboratory.
C     Written by Martin Schulz, schulzm@llnl.gov.
C     LLNL-CODE-402774,
C     All rights reserved.
C
C     This file is part of P^nMPI.
C
C     Please also read the file "LICENSE" included in this package
C     for Our Notice and GNU Lesser General Public License.
C
C     This program is free software; you can redistribute it and/or
C     modify it under the terms of the GNU General Public License
C     (as published by the Free Software Foundation) version 2.1
C     dated February 1999.
C
C     This program is distributed in the hope that it will be useful,
C     but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY
C     OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
C     terms and conditions of the GNU General Public License for more
C     details.

C     You should have received a copy of the GNU Lesser General Public
C     License along with this program; if not, write to the

C     Free Software Foundation, Inc.,
C     Temple Place, Suite 330,
C     Boston, MA 02111-1307 USA

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
