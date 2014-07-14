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
#include <stdio.h>
int main(int argc, char **argv)
{
  void *fPtr = PMPI_Type_create_indexed_block;
  return 0;
}
