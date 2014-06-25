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
  int provided;
  const char *buf = "hello world!";
  /*We call PMPI here since this appears to be the deeper test case*/
  MPI_Init(&argc, &argv);
  MPI_Send(buf, sizeof(buf), MPI_CHAR, 0, 42, MPI_COMM_SELF);
  return 0;
}
