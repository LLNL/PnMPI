      PROGRAM basic
      IMPLICIT NONE
      INCLUDE "mpif.h"
      INTEGER req, prov, ierr
    !We specifically call PMPI here since MVAPICH1 (some version) lacks a P call in its Fortran interface apparently
      CALL PMPI_INIT_THREAD (MPI_THREAD_SINGLE, prov, ierr)
      END
