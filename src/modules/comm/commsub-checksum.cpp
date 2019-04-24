/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2019 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2019 RWTH Aachen University, Federal Republic of Germany
 *
 *
 * P^nMPI is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation version 2.1 dated February 1999.
 *
 * P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with P^nMPI; if not, write to the
 *
 *   Free Software Foundation, Inc.
 *   51 Franklin St, Fifth Floor
 *   Boston, MA 02110, USA
 *
 *
 * Written by Martin Schulz, schulzm@llnl.gov.
 *
 * LLNL-CODE-402774
 */

#include "commsub.h"

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include <pnmpi/const.h>


#define CSD


/*--------------------------------------------------------------------------*/
/* apply checksum to all messages */

long get_checksum(void *buf, int count, MPI_Datatype dt)
{
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s, r;
  MPI_Datatype t;
  long checksum;

  checksum = 0;

  if ((abs(verbosity_level) & 0xff) >= 5)
    {
      printf("In Checksum -( %i)---------------------------\n", count);
    }

  dt_get(buf, count, dt, &ref);
  do
    {
      PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
        ;
#endif
      r = sizeof(checksum);
      while (r <= l * s)
        {
          if ((abs(verbosity_level) & 0xff) >= 5)
            {
              printf("In Checksum %x %i %i %i / val %li\n", b, r, l, s,
                     *((long *)b));
            }
          checksum ^= *((long *)b);
          b += sizeof(checksum);
          r += sizeof(checksum);
        }
    }
  while (!done);

  dt_del(&ref);

  return checksum;
}


void COMM_ALL_INIT(int argc, char **argv)
{
}

void COMM_ALL_FINALIZE()
{
}

void SEND_P2P_START(PNMPI_CONST void *buf, int count, MPI_Datatype dt, int node,
                    int tag, MPI_Comm comm, void **ptr, int type)
{
}

void SEND_P2P_ASYNC_MID1(PNMPI_CONST void *buf, int count, MPI_Datatype dt,
                         int node, int tag, MPI_Comm comm, void **ptr, int type)
{
  long checksum;
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s, i;
  MPI_Datatype t;

  if (comm != MPI_COMM_NULL)
    {
      checksum = get_checksum(buf, count, dt);
      if ((abs(verbosity_level) & 0xff) >= 3)
        {
          printf("Sending checksum %li (dt=%i)\n", checksum, dt);
          dt_get(buf, count, dt, &ref);
          do
            {
              PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
                ;
#endif
              printf("\t%i ", l);
              switch (t)
                {
                case MPI_INT: printf("INT   "); break;
                case MPI_SHORT: printf("SHORT "); break;
                case MPI_LONG: printf("LONG  "); break;
                case MPI_CHAR: printf("CHAR  "); break;
                case MPI_DOUBLE: printf("DOUBLE"); break;
                case MPI_FLOAT: printf("FLOAT "); break;
                default: printf("Other");
                }
              printf(" of size %i at buf %16p / %li == ", s, b,
                     ((long)b) - ((long)buf));
              for (i = 0; i < l; i++)
                {
                  switch (t)
                    {
                    case MPI_INT: printf(" %i", ((int *)b)[i]); break;
                    case MPI_SHORT: printf(" %i", ((short *)b)[i]); break;
                    case MPI_LONG: printf(" %li", ((long *)b)[i]); break;
                    case MPI_CHAR: printf(" %c", ((char *)b)[i]); break;
                    case MPI_DOUBLE: printf(" %f", ((double *)b)[i]); break;
                    case MPI_FLOAT: printf(" %f", ((float *)b)[i]); break;
                    }
                }
              printf("\n");
              fflush(stdout);
            }
          while (!done);
          dt_del(&ref);
        }

      PMPI_Send(&checksum, 1, MPI_LONG, node, tag, comm);
      if ((abs(verbosity_level) & 0xff) >= 4)
        {
          PMPI_Send(buf, count, dt, node, tag, comm);
        }
    }
}

void SEND_P2P_END(PNMPI_CONST void *buf, int count, MPI_Datatype dt, int node,
                  int tag, MPI_Comm comm, int err, void **ptr, void **midptr,
                  int type)
{
  if (type != PNMPIMOD_COMM_ASYNC_P2P)
    {
      SEND_P2P_ASYNC_MID1(buf, count, dt, node, tag, comm, ptr, type);
    }
}

void RECV_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag,
                    MPI_Comm comm, void **ptr, int type)
{
}

void RECV_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node,
                         int tag, MPI_Comm comm, int err, void **ptr, int type)
{
}

void RECV_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag,
                  MPI_Comm comm, int err, void **ptr, void **midptr, int type)
{
  long checksum, incoming;
  MPI_Status status;
  int done;
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l, s, i;
  MPI_Datatype t;


  if (comm != MPI_COMM_NULL)
    {
      checksum = get_checksum(buf, count, dt);
      PMPI_Recv(&incoming, 1, MPI_LONG, node, tag, comm, &status);

      if ((checksum == incoming) || ((type && PNMPIMOD_COMM_COLL_REDUCE) != 0))
        {
          if ((abs(verbosity_level) & 0xff) >= 2)
            {
              printf("Checksum OK\n");
            }
        }
      else
        {
          printf("Checksum Error - %li instead of %li (dt=%i)\n", checksum,
                 incoming, dt);
          if ((abs(verbosity_level) & 0xff) >= 3)
            {
              printf("CSE - Got the following\n");
              dt_get(buf, count, dt, &ref);
              do
                {
                  PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
                    ;
#endif
                  printf("\t%i ", l);
                  switch (t)
                    {
                    case MPI_INT: printf("INT   "); break;
                    case MPI_SHORT: printf("SHORT "); break;
                    case MPI_LONG: printf("LONG  "); break;
                    case MPI_CHAR: printf("CHAR  "); break;
                    case MPI_DOUBLE: printf("DOUBLE"); break;
                    case MPI_FLOAT: printf("FLOAT "); break;
                    default: printf("Other");
                    }
                  printf(" of size %i at buf %16p / %li == ", s, b,
                         ((long)b) - ((long)buf));
                  for (i = 0; i < l; i++)
                    {
                      switch (t)
                        {
                        case MPI_INT: printf(" %i", ((int *)b)[i]); break;
                        case MPI_SHORT: printf(" %i", ((short *)b)[i]); break;
                        case MPI_LONG: printf(" %li", ((long *)b)[i]); break;
                        case MPI_CHAR: printf(" %c", ((char *)b)[i]); break;
                        case MPI_DOUBLE: printf(" %f", ((double *)b)[i]); break;
                        case MPI_FLOAT: printf(" %f", ((float *)b)[i]); break;
                        }
                    }
                  printf("\n");
                  fflush(stdout);
                }
              while (!done);
              dt_del(&ref);
            }
        }

      if ((abs(verbosity_level) & 0xff) >= 4)
        {
          PMPI_Recv(buf, count, dt, node, tag, comm, &status);

          if (checksum != incoming)
            {
              printf("CSE: Message should have been\n");
              dt_get(buf, count, dt, &ref);
              do
                {
                  PNMPIMOD_Datatype_getItem(&ref, &b, &t, &l, &s, &done)
#ifdef USE_FUNCTIONS
                    ;
#endif
                  printf("\t%i ", l);
                  switch (t)
                    {
                    case MPI_INT: printf("INT   "); break;
                    case MPI_SHORT: printf("SHORT "); break;
                    case MPI_LONG: printf("LONG  "); break;
                    case MPI_CHAR: printf("CHAR  "); break;
                    case MPI_DOUBLE: printf("DOUBLE"); break;
                    case MPI_FLOAT: printf("FLOAT "); break;
                    default: printf("Other");
                    }
                  printf(" of size %i at buf %16p / %li == ", s, b,
                         ((long)b) - ((long)buf));
                  for (i = 0; i < l; i++)
                    {
                      switch (t)
                        {
                        case MPI_INT: printf(" %i", ((int *)b)[i]); break;
                        case MPI_SHORT: printf(" %i", ((short *)b)[i]); break;
                        case MPI_LONG: printf(" %li", ((long *)b)[i]); break;
                        case MPI_CHAR: printf(" %c", ((char *)b)[i]); break;
                        case MPI_DOUBLE: printf(" %f", ((double *)b)[i]); break;
                        case MPI_FLOAT: printf(" %f", ((float *)b)[i]); break;
                        }
                    }
                  printf("\n");
                  fflush(stdout);
                }
              while (!done);
              dt_del(&ref);
            }
        }
      if (checksum != incoming)
        {
          if (verbosity_level == 1)
            {
              while (0)
                ;
            }
          if (verbosity_level == -1)
            {
              *((int *)0) = 0;
            }
        }
    }
}


void COMM_P2P_ASYNC_MID2(int count, MPI_Request *requests, int flag,
                         void **midptr)
{
}

void COMM_P2P_ASYNC_COMPLETION(int flag)
{
}

void COMM_COLL_REDUCE(void *buf, int count, MPI_Datatype dt, MPI_Op ops,
                      int size, void **ptr)
{
}

void COMM_COLL_START(MPI_Comm comm, int root, int type, void **ptr)
{
}

void COMM_COLL_END(MPI_Comm comm, int root, int type, void **ptr)
{
}
