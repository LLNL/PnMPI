C This file is part of P^nMPI.
C
C Copyright (c)
C  2008-2016 Lawrence Livermore National Laboratories, United States of America
C  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
C  2013-2016 RWTH Aachen University, Federal Republic of Germany
C
C
C P^nMPI is free software; you can redistribute it and/or modify it under the
C terms of the GNU Lesser General Public License as published by the Free
C Software Foundation version 2.1 dated February 1999.
C
C P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
C WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
C A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
C details.
C
C You should have received a copy of the GNU Lesser General Public License
C along with P^nMPI; if not, write to the
C
C   Free Software Foundation, Inc.
C   51 Franklin St, Fifth Floor
C   Boston, MA 02110, USA
C
C
C Written by Martin Schulz, schulzm@llnl.gov.
C
C LLNL-CODE-402774

C This test file will simply pass the own rank in a circle to the next rank and
C can be used to check the common MPI functions Init, Finalize, Send and Recv
C in combination with PnMPI.

      program firstmpi

      include 'mpif.h'

      integer :: mpierror, mpisize, mpirank, target, incoming
      Integer status(MPI_STATUS_SIZE)


      call MPI_Init(mpierror)
      call MPI_Comm_size(MPI_COMM_WORLD, mpisize, mpierror)
      call MPI_Comm_rank(MPI_COMM_WORLD, mpirank, mpierror)

      target = MOD( mpirank + 1 , mpisize )

      call MPI_Send(mpirank,1,MPI_INTEGER,target,42,MPI_COMM_WORLD,
     2              mpierror)
      call MPI_Recv(incoming,1,MPI_INTEGER,MPI_ANY_SOURCE,42,
     2              MPI_COMM_WORLD,status,mpierror)

      print *, "Message from ",status(MPI_SOURCE),"/",incoming,
     2      " to ",mpirank

      call MPI_Finalize(mpierror)

      end program firstmpi
