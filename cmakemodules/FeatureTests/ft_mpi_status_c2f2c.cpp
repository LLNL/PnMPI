/*
Copyright (c) 2011
Lawrence Livermore National Security, LLC.
Technische Universitaet Dresden, ZIH

Produced at the Lawrence Livermore National Laboratory
and the Technische Universitaet Dresden.
Written by Martin Schulz, schulzm@llnl.gov,
Tobias Hilbrich tobias.hilbrich@tu-dresden.de,
Joachim Protze joachim.protze@tu-dresden.de,
Mathias Korepkat mathias.korepkat@tu-dresden.de.
LLNL-CODE-402774,
All rights reserved.

This file is part of P^nMPI.

@TODO Pointer to LICENSE
*/

#include <mpi.h>
int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  MPI_Status my_status;
  MPI_Fint my_fort_status[100];
  MPI_Status_c2f(&my_status, my_fort_status);
  MPI_Status_f2c(my_fort_status, &my_status);
  MPI_Finalize();
  return 0;
}
