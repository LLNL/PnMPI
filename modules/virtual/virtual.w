/* -*- c -*-
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <pnmpimod.h>

MPI_Comm Reserve_Smaller_Comm = MPI_COMM_WORLD;

#define MACRO_MPI_Comm(_c) {if (_c==MPI_COMM_WORLD) _c=partition_comm;}

int mypartition,rank,size,numpartitions;
MPI_Comm partition_comm;

/* automatically generated wrapper code */
{{fnall fn_name MPI_Init MPI_Pcontrol}} {
  {{apply_to_type MPI_Comm MACRO_MPI_Comm}}
  {{callfn}}
}{{endfnall}}


#define MAXSIZE 40

/*--------------------------------------------------------------------------*/
/* Setup */

#define PNMPI_MODULE_VIRTUAL "virtualization"


/*.......................................................*/
/* Registration */

int PNMPI_RegistrationPoint()
{
  int err;

  /* register this module and its services */

  err=PNMPI_Service_RegisterModule(PNMPI_MODULE_VIRTUAL);
  if (err!=PNMPI_SUCCESS)
    return MPI_ERROR_PNMPI;

  return err;
}


/*.......................................................*/
/* Init */

int  MPI_Init(int * argc, char *** argv)
{
  int err,id,i,j;
  char jobs[100];
  char outname[MAXSIZE];
  char *jobnames;
  int *jobids;
  PNMPI_modHandle_t handle;

  /* query own module */

  err=PNMPI_Service_GetModuleByName(PNMPI_MODULE_VIRTUAL,&handle);
  if (err!=PNMPI_SUCCESS)
    return err;

  err=PNMPI_Service_GetArgument(handle,"jobs",(const char**) &jobs);
  if (err!=PNMPI_SUCCESS)
    {
      if (err==PNMPI_NOARG)
	sprintf(jobs,"name");
      else
	return err;
    }

  /* run init */

  err=PMPI_Init(argc, argv);

  /* get comm world */

  if (err==MPI_SUCCESS)
    {
      err=PMPI_Comm_rank(MPI_COMM_WORLD,&rank);
    }
  if (err==MPI_SUCCESS)
    {
      err=PMPI_Comm_size(MPI_COMM_WORLD,&size);
    }

  /* Communication name argument */

  if (err==MPI_SUCCESS)
    {
      strncpy((char*)outname,(char*)(*argv)[0],MAXSIZE);
      outname[MAXSIZE-1]=(char) 0;
      jobnames=(char*)malloc(MAXSIZE*size);
      if (jobnames==NULL) return -1;
      jobids=(int*)malloc(MAXSIZE*size);
      if (jobnames==NULL) return -1;

      err=PMPI_Allgather(outname,MAXSIZE,MPI_CHAR,jobnames,MAXSIZE,MPI_CHAR,MPI_COMM_WORLD);
    }

  if (rank == 0)
    {
      for (i=0; i<size; i++)
	{
	  printf("Jobname for %i is %s\n",i,&(jobnames[i*MAXSIZE]));
	}
    }

  if (err==MPI_SUCCESS)
    {
      id=0;
      for (i=0; i<size; i++)
	{
	  for (j=0; ((j<i) && (j>=0)); j++)
	    {
	      if (strcmp(&(jobnames[j*MAXSIZE]),&(jobnames[i*MAXSIZE]))==0)
		{
		  jobids[i]=jobids[j];
		  j=-1000;
		}
	    }
	  if (j>=0)
	    {
	      jobids[i]=id;
	      id++;
	    }
	}
      numpartitions=id;
      mypartition=jobids[rank];

      printf("Rank %i is part of partition %i / name is %s\n",rank,mypartition,(*argv)[0]);

      free(jobnames);
      free(jobids);

      err=PMPI_Comm_split(MPI_COMM_WORLD,mypartition,rank,&partition_comm);
    }
  return err;
}

