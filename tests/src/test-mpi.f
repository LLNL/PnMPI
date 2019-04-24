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

      integer :: ierror
      call MPI_INIT(ierror)
      call MPI_FINALIZE(ierror)

      end program firstmpi


C Note: There is no special test for preloading PnMPI by environment variables,
C       as different MPI implementations handle environment variables in
C       different ways and the setting the variables for the whole test
C       environment interferes with additional tools like AddressSanitizer, as
C       it would be preloaded for mpiexec, too. However, preloading is
C       indirectly tested by the PnMPIze tests.
C
C
C CONFIGS: dynamic static
C
C COMPILE_INCLUDES: @MPI_Fortran_INCLUDE_PATH@
C COMPILE_FLAGS: @MPI_Fortran_COMPILE_FLAGS@
C LINK_FLAGS: @MPI_Fortran_LINK_FLAGS@
C LINK: @MPI_Fortran_LIBRARIES@
C
C RUN: @MPIEXEC@ @MPIEXEC_NUMPROC_FLAG@ 1
C RUN:   @MPIEXEC_PREFLAGS@ @BINARY@ @MPIEXEC_POSTFLAGS@
C PASS: No modules loaded.
C
C
C LINK-dynamic: pnmpif @MPI_Fortran_LIBRARIES@
C
C LINK-static: pnmpif_static @MPI_Fortran_LIBRARIES@ dl m
