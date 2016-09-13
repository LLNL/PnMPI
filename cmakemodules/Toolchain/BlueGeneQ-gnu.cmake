# Need this first to ensure that we include PnMPI's custom platform files.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakemodules;${CMAKE_MODULE_PATH}")

# the name of the target operating system
set(CMAKE_SYSTEM_NAME BlueGeneQ-dynamic)

set(TOOLCHAIN_LOCATION /bgsys/drivers/ppcfloor/gnu-linux/bin)
set(TOOLCHAIN_PREFIX powerpc64-bgq-linux-)

set(CMAKE_C_COMPILER       ${TOOLCHAIN_LOCATION}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER     ${TOOLCHAIN_LOCATION}/${TOOLCHAIN_PREFIX}g++)
set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_LOCATION}/${TOOLCHAIN_PREFIX}gfortran)

# Make sure MPI_COMPILER wrapper matches the gnu compilers.
# Prefer local machine wrappers to driver wrappers here too.
set(BGQ_MPI_COMPILER_SEARCH_PATHS
  /usr/local/bin
  /usr/bin
  /bgsys/drivers/ppcfloor/comm/gcc/bin)

find_program(MPI_C_COMPILER NAMES mpicc PATHS ${BGQ_MPI_COMPILER_SEARCH_PATHS})
find_program(MPI_CXX_COMPILER NAMES mpicxx mpic++ PATHS ${BGQ_MPI_COMPILER_SEARCH_PATHS})
find_program(MPI_Fortran_COMPILER NAMES mpif90 mpif77 PATHS ${BGQ_MPI_COMPILER_SEARCH_PATHS})

SET(MPIEXEC_NUMPROC_FLAG "-n" CACHE STRING "Flag used by MPI to specify the number of processes for MPIEXEC; the next option will be the number of processes." FORCE)
SET(MPIEXEC_EXTRA_FLAG "-ppdebug" CACHE STRING "Extraflag used to give the debug queue for srun" FORCE)
