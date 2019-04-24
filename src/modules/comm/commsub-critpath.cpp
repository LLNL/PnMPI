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

#include <sys/time.h>

#include <graphlib.h>
#include <libunwind.h>
#include <mpi.h>
#include <pnmpi/const.h>


#define GLOBALTIME(t, n) ((n + 1) * 10000 + t)
#define EXTRACTNODE(id) ((id / 10000) - 1)

#define XSPACE 300
#define YSPACE 50

#define NODESTACKDEPTH 1000


/*--------------------------------------------------------------------*/
/* Global variables */

int critpath_myid;
int critpath_tasks;

int local_timestamp;

graphlib_graph_p lgr;
graphlib_color_t *cp_nodecolor;

int critpath;

int after_barrier, barrier_number, last_barrier = 0;

graphlib_node_t **cp_nodestacks;
int *cp_nodestacks_sp;


/*--------------------------------------------------------------------*/
/* timing */

#define WAITTHRESHHOLD 0.000001 /* 1 us */

typedef double mytime_t;

mytime_t timing_start;


void mytime_Start()
{
}

mytime_t mytime_getTimeStamp()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((double)tv.tv_sec) + (((double)tv.tv_usec) / 1000000.0);
}

double mytime_convertToSec(mytime_t ti)
{
  return ti;
}

mytime_t mytime_timeDiff(mytime_t start, mytime_t end)
{
  return end - start;
}


/*--------------------------------------------------------------------*/

void cp_setnode(int node, int ts, graphlib_width_t w, int crit)
{
  graphlib_nodeattr_t attr;

  graphlib_setDefNodeAttr(&attr);

  if (node < 0)
    {
      if (ts < 0)
        {
          attr.x = (critpath_tasks + 5) * YSPACE;
          attr.y = 0;
        }
      else
        {
          /* barrier node */

          attr.x = (XSPACE * (critpath_tasks - 1)) / 2;
          attr.y = ts * YSPACE;
        }
      if (w < 0)
        attr.width = 0;
      else
        attr.width = w;

      if (crit)
        attr.color = GRC_WHITE;
      else
        attr.color = GRC_WHITE;

      if (last_barrier)
        graphlib_addNodeNoCheck(lgr, 0, &attr);
      else
        graphlib_addNodeNoCheck(lgr, barrier_number, &attr);
    }
  else
    {
      attr.x = node * XSPACE;
      attr.y = ts * YSPACE;
      if (w < 0)
        attr.width = 0;
      else
        attr.width = w;
      if (crit == 1000)
        attr.color = GRC_RED;
      else
        attr.color = GRC_LIGHTGRAY;

      graphlib_addNodeNoCheck(lgr, GLOBALTIME(ts, node), &attr);
    }
}

/*--------------------------------------------------------------------*/

void cp_setedge(int n1, int t1, int n2, int t2, int crit)
{
  graphlib_edgeattr_t attr;
  graphlib_node_t id1, id2;

  graphlib_setDefEdgeAttr(&attr);

  if (n1 < 0)
    {
      if (t1 < 0)
        id1 = 0;
      else
        id1 = barrier_number - 1;
    }
  else
    id1 = GLOBALTIME(t1, n1);

  if (n2 < 0)
    {
      if (last_barrier)
        id2 = 0;
      else
        id2 = barrier_number;
    }
  else
    id2 = GLOBALTIME(t2, n2);

  attr.width = -1;
  if (crit < 0)
    attr.color = GRC_BLACK;
  else if (crit == 0)
    attr.color = GRC_GREEN;
  else
    attr.color = GRC_RED;

  graphlib_addDirectedEdgeNoCheck(lgr, id1, id2, &attr);
}


/*--------------------------------------------------------------------*/
/* Init */

void COMM_ALL_INIT(int argc, char **argv)
{
  PMPI_Comm_size(MPI_COMM_WORLD, &critpath_tasks);
  PMPI_Comm_rank(MPI_COMM_WORLD, &critpath_myid);

  local_timestamp = 0;
  critpath = 1;
  after_barrier = 0;
  barrier_number = 1;

  graphlib_newGraph(&lgr);

  barrier_number = 0;
  //  cp_setnode(-1,-1,-1,0);

  barrier_number = 1;
  cp_setnode(-1, local_timestamp, -1, critpath);
  after_barrier = 1;
  barrier_number++;
}

void COMM_ALL_FINALIZE()
{
  char name[200], *dir;
  int bar;

  last_barrier = 1;
  MPI_Barrier(MPI_COMM_WORLD);

  dir = getenv("TRACEDIR");
  if (dir != NULL)
    sprintf(name, "%s/graph-%03i", dir, critpath_myid);
  else
    sprintf(name, "graph-%03i", critpath_myid);
  graphlib_saveGraph(name, lgr);
}

void SEND_P2P_START(PNMPI_CONST void *buf, int count, MPI_Datatype dt, int node,
                    int tag, MPI_Comm comm, void **ptr, int type)
{
}

void SEND_P2P_ASYNC_MID1(PNMPI_CONST void *buf, int count, MPI_Datatype dt,
                         int node, int tag, MPI_Comm comm, void **ptr, int type)
{
  int old_timestamp;
  int pbdata[2];

  /* build graph */

  old_timestamp = local_timestamp;
  local_timestamp++;

  /* build graph */

  cp_setnode(critpath_myid, local_timestamp, -1, critpath);

  if (after_barrier)
    cp_setedge(-1, old_timestamp, critpath_myid, local_timestamp, critpath);
  else
    cp_setedge(critpath_myid, old_timestamp, critpath_myid, local_timestamp,
               critpath);

  after_barrier = 0;

  pbdata[0] = (local_timestamp * 2) | (critpath & 1);
  pbdata[1] = -1;

  /* set piggyback data */

  PMPI_Send(&pbdata, 1, MPI_INT, node, tag, comm);
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
  if (type != PNMPIMOD_COMM_ASYNC_P2P)
    {
      timing_start = mytime_getTimeStamp();
    }
}

void COMM_P2P_ASYNC_MID2(int count, MPI_Request *requests, int flag,
                         void **midptr)
{
  timing_start = mytime_getTimeStamp();
}


void RECV_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node,
                         int tag, MPI_Comm comm, int err, void **ptr, int type)
{
}

void COMM_P2P_ASYNC_COMPLETION(int flag)
{
}

void RECV_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag,
                  MPI_Comm comm, int err, void **ptr, void **midptr, int type)
{
  int remote_timestamp, old_timestamp, ncritpath, rcritpath, redge, ledge;
  int pbdata[2];
  MPI_Status status;
  mytime_t timing_diff;

  timing_diff =
    mytime_convertToSec(mytime_timeDiff(timing_start, mytime_getTimeStamp()));

  /* get piggyback data */

  PMPI_Recv(pbdata, 2, MPI_INT, node, tag, comm, &status);


  /* create graph */

  remote_timestamp = (pbdata[0] & 0xFFFFFFFE) / 2;
  rcritpath = (pbdata[0] & 1);

  if (timing_diff <= WAITTHRESHHOLD)
    {
      ncritpath = critpath;
      redge = 0;
      ledge = critpath;
    }
  else
    {
      ncritpath = rcritpath;
      ledge = 0;
      redge = rcritpath;
    }

  old_timestamp = local_timestamp;
  if (remote_timestamp >= local_timestamp)
    local_timestamp = remote_timestamp + 1;
  else
    local_timestamp++;

  cp_setnode(critpath_myid, local_timestamp, timing_diff, ncritpath);

  if (after_barrier)
    {
      if ((ledge) || (verbosity_level))
        cp_setedge(-1, old_timestamp, critpath_myid, local_timestamp, ledge);
    }
  else
    {
      if ((ledge) || (verbosity_level))
        cp_setedge(critpath_myid, old_timestamp, critpath_myid, local_timestamp,
                   ledge);
    }

  if ((redge) || (verbosity_level))
    {
      cp_setnode(node, remote_timestamp, -1, rcritpath);
      cp_setedge(node, remote_timestamp, critpath_myid, local_timestamp, redge);
    }

  critpath = ncritpath;

  after_barrier = 0;
}

void COMM_COLL_REDUCE(void *buf, int count, MPI_Datatype dt, MPI_Op ops,
                      int size, void **ptr)
{
}

void COMM_COLL_START(MPI_Comm comm, int root, int type, void **ptr)
{
  timing_start = mytime_getTimeStamp();
}

void COMM_COLL_END(MPI_Comm comm, int root, int type, void **ptr)
{
  int ts, edge;
  struct minnode
  {
    double d;
    int n;
  } loc, glob;
  mytime_t timing_diff;

  timing_diff =
    mytime_convertToSec(mytime_timeDiff(timing_start, mytime_getTimeStamp()));

  critpath = 1;

  if (after_barrier)
    {
      ts = local_timestamp + 1;
      cp_setnode(-1, ts, -1, critpath);
      cp_setedge(-1, local_timestamp, -1, ts, 1);
    }
  else
    {
      /* find timestamps */

      PMPI_Allreduce(&local_timestamp, &ts, 1, MPI_INT, MPI_MAX, comm);
      ts++;

      /* find the minimal wait node */

      loc.d = timing_diff;
      loc.n = critpath_myid;
      PMPI_Allreduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MINLOC, comm);

      /* set barrier node */

      cp_setnode(-1, ts, -1, critpath);
      if (glob.n == critpath_myid)
        edge = 1;
      else
        edge = 0;

      if (edge)
        cp_setedge(critpath_myid, local_timestamp, -1, ts, edge);
    }

  /* set new timestamp and critpath */

  local_timestamp = ts;
  after_barrier = 1;
  barrier_number++;
}
