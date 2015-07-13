/*
Copyright (c) 2008
Lawrence Livermore National Security, LLC. 

Produced at the Lawrence Livermore National Laboratory. 
Written by Martin Schulz, schulzm@llnl.gov.
LLNL-CODE-402774,
All rights reserved.

This file is part of P^nMPI. 

Please also read the file "LICENSE" included in this package for 
Our Notice and GNU Lesser General Public License.

This program is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
(as published by the Free Software Foundation) version 2.1 
dated February 1999.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY 
OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
terms and conditions of the GNU General Public License for more 
details.

You should have received a copy of the GNU Lesser General Public 
License along with this program; if not, write to the 

Free Software Foundation, Inc., 
59 Temple Place, Suite 330, 
Boston, MA 02111-1307 USA
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "commsub.h"

/*--------------------------------------------------------------------------*/
/* print all message traffic */

int send_cnt=0;
int recv_cnt=0;

void COMM_ALL_INIT(int argc, char**argv) 
{
}

void COMM_ALL_FINALIZE() 
{
}

void SEND_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag, 
		    MPI_Comm comm, void **ptr, int type)
{
  int done; 
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l,s;
  MPI_Datatype t;

  if ((verbosity_level & 0xff)>0)
    {
      printf("Starting to send %i (%i x DT %i) to %i channel %i/%i\n",
	     send_cnt,count,dt,node,tag,comm);
    }
  *((int*) ptr)=send_cnt;
  send_cnt++;

  if ((verbosity_level & 0xff)>1)
    {
      dt_get(buf, count, dt, &ref);
      do
	{
	  PNMPIMOD_Datatype_getItem(&ref,&b,&t,&l,&s,&done)
#ifdef USE_FUNCTIONS
	    ;
#endif
	  printf("\t%i ",l);
	  if (t==MPI_INT) printf("INT   "); 
	  else if (t==MPI_SHORT) printf("SHORT ");
	  else if (t==MPI_LONG) printf("LONG  "); 
	  else if (t==MPI_CHAR) printf("CHAR  "); 
	  else if (t==MPI_DOUBLE) printf("DOUBLE"); 
	  else if (t==MPI_FLOAT) printf("FLOAT "); 
	  else printf("Other");
	  printf(" of size %i at buf %16p / %li\n",s,b,((long) b)-((long) buf));
	  if ((verbosity_level & 0xff)>2)
	    {
	      int _t,_v,_c;
	      printf("\tContents:");
	      for (_t=0; _t<s; _t++)
		{
		  _c=b[_t];
		  _v=(int) _c;
		  printf(" %i",_v);
		}
	      printf("\n");
	    }
	  fflush(stdout);
	}
      while (!done);
      
      dt_del(&ref);
    }
}


void SEND_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node, int tag, 
			 MPI_Comm comm, void **ptr, int type) 
{
}

void SEND_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag, 
		  MPI_Comm comm, int err, void **ptr, void **midptr, int type) 
{
  if ((verbosity_level & 0xff)>0)
    {
      printf("--> Send %i done (%i x DT %i) to %i channel %i/%i\n",
	     *((int*) ptr),count,dt,node,tag,comm);
    }
}

void RECV_P2P_START(void *buf, int count, MPI_Datatype dt, int node, int tag, 
		    MPI_Comm comm, void **ptr, int type) 
{
  if ((verbosity_level & 0xff)>0)
    {
      printf("Starting to receive %i (%i x DT %i)\n",
	     recv_cnt,count,dt);
    }
  *((int*) ptr)=recv_cnt;
  recv_cnt++;
}

void RECV_P2P_ASYNC_MID1(void *buf, int count, MPI_Datatype dt, int node, int tag, 
			 MPI_Comm comm, int err, void **ptr, int type) 
{
}

void RECV_P2P_END(void *buf, int count, MPI_Datatype dt, int node, int tag, 
		  MPI_Comm comm, int err, void **ptr, void **midptr, int type) 
{
  int done; 
  PNMPIMOD_Datatype_Parameters_t ref;
  char *b;
  int l,s;
  MPI_Datatype t;

  if ((verbosity_level & 0xff)>0)
    {
      printf("--> Receive %i done (%i x DT %i) from %i channel %i/%i\n",
	     *((int*) ptr),count,dt,node,tag,comm);
    }

  if ((verbosity_level & 0xff)>1)
    {
      dt_get(buf, count, dt, &ref);
      do
	{
	  PNMPIMOD_Datatype_getItem(&ref,&b,&t,&l,&s,&done)
#ifdef USE_FUNCTIONS
	    ;
#endif
	  printf("\t%i ",l);
	  if (t==MPI_INT) printf("INT   "); 
	  else if (t==MPI_SHORT) printf("SHORT ");
	  else if (t==MPI_LONG) printf("LONG  "); 
	  else if (t==MPI_CHAR) printf("CHAR  "); 
	  else if (t==MPI_DOUBLE) printf("DOUBLE"); 
	  else if (t==MPI_FLOAT) printf("FLOAT "); 
	  else printf("Other");
	  printf(" of size %i at buf %16p / %li\n",s,b,((long) b)-((long) buf));
	  if ((verbosity_level & 0xff)>2)
	    {
	      int _t,_v,_c;
	      printf("\tContents:");
	      for (_t=0; _t<s; _t++)
		{
		  _c=b[_t];
		  _v=(int) _c;
		  printf(" %i",_v);
		}
	      printf("\n");
	    }
	  fflush(stdout);
	}
      while (!done);
      
      dt_del(&ref);
    }
}

void COMM_P2P_ASYNC_MID2(int count, MPI_Request *requests, int flag, void **midptr) 
{
}

void COMM_P2P_ASYNC_COMPLETION(int flag) 
{
}

void COMM_COLL_REDUCE(void *buf, int count, MPI_Datatype dt, MPI_Op ops, int size, void** ptr) 
{
}

void COMM_COLL_START(MPI_Comm comm,int root,int type, void **ptr) 
{
}

void COMM_COLL_END(MPI_Comm comm,int root,int type, void **ptr) 
{
}




