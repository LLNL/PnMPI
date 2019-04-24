C This file is part of P^nMPI.
C
C Copyright (c)
C  2008-2019 Lawrence Livermore National Laboratories, United States of America
C  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
C  2013-2019 RWTH Aachen University, Federal Republic of Germany
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

      program firstmpi

      include 'mpif.h'

      integer :: ierror, size, rank, buffer, i
      Integer status(MPI_STATUS_SIZE)


      call MPI_INIT(ierror)
      call MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierror)
      call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierror)

      if (size .lt. 2) then
        stop "At least 2 ranks are required for this test."
      end if

C     All ranks send their rank to rank 0 which then answers the sending rank.
      if (rank .eq. 0) then
        do i = 1,size-1
          call MPI_RECV(buffer, 1, MPI_INTEGER, i, 42, MPI_COMM_WORLD,
     &                  status, ierror)
          call MPI_SEND(buffer, 1, MPI_INTEGER, i, 42, MPI_COMM_WORLD,
     &                  ierror)

          print *, "Got ", buffer, " from rank ", status(MPI_SOURCE)
        end do
      else
        call MPI_SEND(rank, 1, MPI_INTEGER, 0, 42, MPI_COMM_WORLD,
     &                ierror)
        call MPI_RECV(buffer, 1, MPI_INTEGER, 0, 42, MPI_COMM_WORLD,
     &                status, ierror)

        print *, "Got ", buffer, " from rank ", status(MPI_SOURCE)
      end if

      call MPI_FINALIZE(ierror)

      end program firstmpi


C COMPILE_INCLUDES: @MPI_Fortran_INCLUDE_PATH@
C COMPILE_FLAGS: @MPI_Fortran_COMPILE_FLAGS@
C LINK: pnmpif @MPI_Fortran_LIBRARIES@
C LINK_FLAGS: @MPI_Fortran_LINK_FLAGS@
C
C RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 2
C RUN:     @MPIEXEC_PREFLAGS@ @BINARY@ @MPIEXEC_POSTFLAGS@
C RUN:   | sort -n
C
C PASS: Got.*1.*from rank.*0.*Got.*1.*from rank.*1
