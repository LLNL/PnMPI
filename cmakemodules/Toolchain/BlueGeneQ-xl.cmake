# Need this first to ensure that we include PnMPI's custom platform files.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakemodules;${CMAKE_MODULE_PATH}")

# the name of the target operating system
set(CMAKE_SYSTEM_NAME BlueGeneQ-dynamic)

# Set search paths to prefer local, admin-installed wrappers for the BG backend compilers
set(BGQ_XL_COMPILER_SEARCH_PATHS
  /usr/local/bin
  /usr/bin
  /opt/ibmcmp/vac/bg/12.1/bin
  /opt/ibmcmp/vacpp/bg/12.1/bin
  /opt/ibmcmp/xlf/bg/14.1/bin)

# XL C Compilers
find_program(CMAKE_C_COMPILER       bgxlc      ${BGQ_XL_COMPILER_SEARCH_PATHS})
find_program(CMAKE_CXX_COMPILER     bgxlc      ${BGQ_XL_COMPILER_SEARCH_PATHS})
find_program(CMAKE_Fortran_COMPILER bgxlf90    ${BGQ_XL_COMPILER_SEARCH_PATHS})

# Make sure MPI_COMPILER wrapper matches the gnu compilers.
# Prefer local machine wrappers to driver wrappers here too.
set(BGQ_MPI_COMPILER_SEARCH_PATHS
  /usr/local/bin
  /usr/bin
  /bgsys/drivers/ppcfloor/comm/gcc/bin)

find_program(MPI_C_COMPILER NAMES mpixlc mpxlc PATHS ${BGQ_MPI_COMPILER_SEARCH_PATHS})
find_program(MPI_CXX_COMPILER NAMES mpixlc mpxlc PATHS ${BGQ_MPI_COMPILER_SEARCH_PATHS})
find_program(MPI_Fortran_COMPILER NAMES mpixlf90 mpxlf90 PATHS ${BGQ_MPI_COMPILER_SEARCH_PATHS})

SET(MPIEXEC_NUMPROC_FLAG "-n" CACHE STRING "Flag used by MPI to specify the number of processes for MPIEXEC; the next option will be the number of processes." FORCE)
SET(MPIEXEC_EXTRA_FLAG "-ppdebug" CACHE STRING "Extraflag used to give the debug queue for srun" FORCE)
