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

/* Special wrappers not covered by the generator */

#ifdef EXPERIMENTAL_UNWIND
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif
#include <stdarg.h>

#include "core.h"

/*-------------------------------------------------------------------*/
/* include generated wrappers */

/*int NQJ_Finalize(void);
  int NQJ_Init(int * _pnmpi_arg_0, char * * * _pnmpi_arg_1);*/


extern void *MPIR_ToPointer (int idx);

#include "../wrapper/wrapper.c"
#include "../wrapper/f77symbols.h"
#include "../wrapper/wrapper_f77.c"

/* Fortran wrappers */
#if 0
#include "../wrapper/wrapperf.h"
#include "../wrapper/wrapperf.c"
#endif

#ifdef COMPILE_FOR_FORTRAN
void pmpi_init_(int *ierror); 
#endif

static int init_was_fortran = -1;

/*-------------------------------------------------------------------*/
/* MPI_Init */

static int PNMPI_Common_MPI_Init(int * _pnmpi_arg_0, char * * * _pnmpi_arg_1)
{
  int returnVal;

  pnmpi_PreInit();  /* this will never fail */
  
  if (NOT_ACTIVATED(MPI_Init_MAJOR,MPI_Init_MINOR))
    {
#ifdef COMPILE_FOR_FORTRAN
      if (init_was_fortran)
	pmpi_init_(&returnVal);
      else
#endif
	returnVal=PMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);
    }
  else
    returnVal=Internal_XMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);

  if (returnVal!=MPI_SUCCESS)
    return returnVal;

  DBGLATEINIT();
  STATUSINIT();
  DBGPRINT1("Leaving Init");

  STATUSPRINT1("");
  STATUSPRINT1("\t\t  ---------------------------");
  STATUSPRINT1("\t\t | P^N-MPI Interface         |");
  STATUSPRINT1("\t\t | Martin Schulz, 2005, LLNL |");
  STATUSPRINT1("\t\t  ---------------------------");
  STATUSPRINT1("");

  {
    int i;
    module_servlist_p serv;
    module_globlist_p glob;
    module_arg_t *args;

    STATUSPRINT1("Number of modules: %i",modules.num);
    STATUSPRINT1("Pcontrol Setting:  %i",modules.pcontrol);
    STATUSPRINT1("");
    for (i=0; i<modules.num; i++)
      {
	if (modules.module[i]->registered)
	  {
	    STATUSPRINT1("Module %s: registered as %s (Pctrl %i)",
			 modules.module[i]->name,
			 modules.module[i]->username,
			 modules.module[i]->pcontrol);
	  }
	else
	  {
	    if (modules.module[i]->stack_delimiter)
	      {	    STATUSPRINT1("Stack %s: not registered",
				 modules.module[i]->name);
	      }
	    else
	      {	    STATUSPRINT1("Module %s: not registered (Pctrl %i)",
			 modules.module[i]->name,
			 modules.module[i]->pcontrol);
	      }
	  }

	for (args=modules.module[i]->args; args!=NULL; args=args->next)
	  {
	    STATUSPRINT1("\tArgument: %s = %s",
			 args->name,args->value);
	  }

	for (serv=modules.module[i]->services; serv!=NULL; serv=serv->next)
	  {
	    STATUSPRINT1("\tService: %s (%s)",
			 serv->desc.name,serv->desc.sig);
	  }

	for (glob=modules.module[i]->globals; glob!=NULL; glob=glob->next)
	  {
	    STATUSPRINT1("\tGlobal: %s (%c)",
			 glob->desc.name,glob->desc.sig);
	  }
      }
    STATUSPRINT1("");
  }

  return returnVal;
}

#ifndef AIX

#ifdef COMPILE_FOR_FORTRAN
void mpi_init_(int *ierr)
{
  /* some code in here is taken from MPICH-1 */

  int argc;
  char **argv;

#if 0
  int i, argsize = 1024;
  char *p;
  /*  int  argcSave;           Save the argument count */
  char **argvSave;         /* Save the pointer to the argument vector */
#endif
  
  DBGEARLYINIT();

  DBGPRINT3("Entering Old Fortran MPI_Init at base level");

  if (init_was_fortran==0)
    {
      *ierr=MPI_SUCCESS;
      return;
    }

  init_was_fortran=1;
  
#if 0
  /* argcSave    = */ argc = iargc_() + 1;
  argvSave    = argv = (char **) malloc( argc * sizeof(char *) );

  if (!argv) 
    {
      WARNPRINT("Can't allocate memory for argv table - exiting"); 
      exit(1);
    }

    for (i=0; i<argc; i++) 
      {
        argvSave[i] = argv[i] = (char *)malloc( argsize + 1 );
        if (!argv[i]) 
	  {
	    WARNPRINT("Can't allocate memory for argv[%i] - exiting",i);
	    exit(1);
	  }
        getarg_( &i, argv[i], argsize );

        /* Trim trailing blanks */
        p = argv[i] + argsize - 1;
        while (p > argv[i]) 
	  {
            if (*p != ' ') 
	      {
                p[1] = '\0';
                break;
	      }
            p--;
	  }
      }

#ifdef DBGLEVEL
    DBGPRINT4("ARGUMENT COUNT IS %i\n",argc);
    for (i=0; i<argc; i++)
      {
	DBGPRINT4("ARGUMENT %i IS %s",i,argv[i]);
      }
#endif /* DBGLEVEL */

  *ierr=PNMPI_Common_MPI_Init(&argc,&argv);
#endif

  argc=0;
  argv=NULL;
    
  *ierr=PNMPI_Common_MPI_Init(&argc,&argv);
  return;
}
#endif
#endif


int MPI_Init(int *argc, char ***argv)
{
  DBGEARLYINIT();

  DBGPRINT3("Entering Old MPI_Init at base level");

  if (init_was_fortran==1)
    return MPI_SUCCESS;

  init_was_fortran=0;
  
  return PNMPI_Common_MPI_Init(argc,argv);
}


static int pnmpi_init_done=0;

int NQJ_Init(int * _pnmpi_arg_0, char * * * _pnmpi_arg_1)
{
  int res;
  int start_level;
  
  start_level=pnmpi_level;
  
  if (IS_ACTIVATED(MPI_Init_MAJOR,MPI_Init_MINOR))
    {
       while ((pnmpi_level<pnmpi_max_level) && (modules.module[pnmpi_level]->stack_delimiter==0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Init[pnmpi_level]!=NULL)
	    {
	      DBGPRINT3("Calling a wrapper in MPI_Init at level %i FROM %px",pnmpi_level,&(Internal_XMPI_Init));
	      res=(pnmpi_function_ptrs.pnmpi_int_MPI_Init)[pnmpi_level](_pnmpi_arg_0, _pnmpi_arg_1);
	      DBGPRINT3("Done with wrapper in MPI_Init at level %i - reseting to %i",pnmpi_level,start_level);
	      pnmpi_level=start_level;
	      return res;
            }
          pnmpi_level++;
       }
    }

  if (pnmpi_init_done)
    {
      DBGPRINT3("Duplicated: calling a original MPI in MPI_Init");
      res=MPI_SUCCESS;
    }
  else
    {
      DBGPRINT3("Calling a original MPI in MPI_Init");
#ifdef COMPILE_FOR_FORTRAN
      if (init_was_fortran)
	pmpi_init_(&res);
      else
#endif
	res=PMPI_Init(_pnmpi_arg_0, _pnmpi_arg_1);
      pnmpi_init_done=1;
    }
  DBGPRINT3("Done with original MPI in MPI_Init");
  pnmpi_level=start_level;
  return res;
}

int MPI_Finalize(void)
{
  DBGPRINT3("Entering Old MPI_Finalize at base level - Location = %px",&(MPI_Finalize));

  if (NOT_ACTIVATED(MPI_Finalize_MAJOR,MPI_Finalize_MINOR))
    return PMPI_Finalize();
  else
    {
      int err=Internal_XMPI_Finalize();
      PMPI_Finalize();
      return err;
    }
}

int NQJ_Finalize(void)
{
  int res;
  int start_level;
  
  start_level=pnmpi_level;
  
  if (IS_ACTIVATED(MPI_Finalize_MAJOR,MPI_Finalize_MINOR))
    {
       while ((pnmpi_level<pnmpi_max_level) && (modules.module[pnmpi_level]->stack_delimiter==0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_MPI_Finalize[pnmpi_level]!=NULL)
	    {
	      DBGPRINT3("Calling a wrapper in MPI_Finalize at level %i FROM %px",pnmpi_level,&(Internal_XMPI_Finalize));
	      res=(pnmpi_function_ptrs.pnmpi_int_MPI_Finalize)[pnmpi_level]();
	      DBGPRINT3("Done with wrapper in MPI_Finalize at level %i - reseting to %i",pnmpi_level,start_level);
	      pnmpi_level=start_level;
	      return res;
            }
          pnmpi_level++;
       }
    }

  DBGPRINT3("Calling a original MPI in MPI_Finalize");
  res=MPI_SUCCESS;
  DBGPRINT3("Done with original MPI in MPI_Finalize");
  pnmpi_level=start_level;
  return res;
}

/*-------------------------------------------------------------------*/
/* MPI_Pcontrol */

#define TRANSTYPE doubledouble

typedef struct doubledouble_d
{
  double a,b;
} doubledouble;

typedef struct dummy10_d
{
  TRANSTYPE d[10];
} dummy10_t;

typedef struct dummy100_d
{
  TRANSTYPE d[100];
} dummy100_t;


static int PNMPI_Internal_CallVarg(pnmpi_int_MPI_Pcontrol_t fct, int level, TRANSTYPE* args, int len)
{
  if (args==NULL)
    {
      return fct(level);
    }
  else
    {
      switch (len)
	{
#include "vargcall.c"
	default: 
	  WARNPRINT("Pcontrol arguments too long - reporting error"); 
	  return PNMPI_ERROR;
	}
    }
}


int MPI_Pcontrol(int level, ... )
{  
#ifdef EXPERIMENTAL_UNWIND
  unw_context_t ctx;
  unw_cursor_t ptr;
  unw_word_t sp1,sp2,len;
#endif
  TRANSTYPE *start;
  int leni,mod,modnum,size,type,actlevel;
  int error=MPI_SUCCESS;
  int *mods,*errors;
  va_list va_alist;

  int ret,i;

  DBGPRINT3("Entering Old MPI_Pcontrol at base level");

	/* This mode just delegates the level arg to modules w/pcontrol on. */
	if ((modules.pcontrol == PNMPI_PCONTROL_INT) ||
		((modules.pcontrol == PNMPI_PCONTROL_TYPED) && (modules.pcontrol_typed_level != level)))
	{
		for (i=0; i<pnmpi_max_level; i++)
		{
			if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i]!=NULL) &&
				(modules.module[i]->pcontrol))
			{
				/* yes, we need to call this Pcontrol */
				ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level);
				if (ret!=MPI_SUCCESS) return ret;
			}
		}
		return MPI_SUCCESS;
	}
	
	/* This mode delegates the level arg plus one typed parameter to modules w/pcontrol on. */
	/* in this case: modules.pcontrol_typed_level == level */
	if (modules.pcontrol == PNMPI_PCONTROL_TYPED)
	{
		int arg_int=0;
		long arg_long=0;
		void* arg_ptr=NULL;
		double arg_double=0.0;
		
		va_start(va_alist,level);
		switch (modules.pcontrol_typed_type)
		{
			case PNMPI_PCONTROL_TYPE_INT: 
				arg_int=va_arg(va_alist,int);
				break;
			case PNMPI_PCONTROL_TYPE_LONG: 
				arg_long=va_arg(va_alist,long);
				break;
			case PNMPI_PCONTROL_TYPE_PTR: 
				arg_ptr=va_arg(va_alist,void*);
				break;
			case PNMPI_PCONTROL_TYPE_DOUBLE: 
				arg_double=va_arg(va_alist,double);
				break;
			default:
				WARNPRINT("Unknown pcontrol argument type, defaulting to PNMPI_PCONTROL_TYPED\n");
		}
		va_end(va_alist);
		
		for (i=0; i<pnmpi_max_level; i++)
		{
			if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i]!=NULL) &&
				(modules.module[i]->pcontrol))
			{
				switch (modules.pcontrol_typed_type)
				{
					case PNMPI_PCONTROL_TYPE_INT: 
						ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,arg_int);
						break;
					case PNMPI_PCONTROL_TYPE_LONG: 
						ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,arg_long);
						break;
					case PNMPI_PCONTROL_TYPE_PTR: 
						ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,arg_ptr);
						break;
					case PNMPI_PCONTROL_TYPE_DOUBLE: 
						ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level,arg_double);
						break;
					default:
						ret = pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i](level);
				}
				if (ret!=MPI_SUCCESS) return ret;
			}
		}
		return MPI_SUCCESS;
	}
	
	
  if (modules.pcontrol==PNMPI_PCONTROL_OFF)
    {
      return MPI_SUCCESS;
    }

  if ((modules.pcontrol==PNMPI_PCONTROL_PNMPI) &&
      (level!=PNMPI_PCONTROL_LEVEL))
    {
      return PNMPI_ERROR;
    }

  /* start processing the variable list arguments */

  va_start(va_alist,level);

  if ((modules.pcontrol==PNMPI_PCONTROL_PNMPI) ||
      ((modules.pcontrol==PNMPI_PCONTROL_MIXED) && 
       (level==PNMPI_PCONTROL_LEVEL)))
    {
      /* own format */
      /* int level = PNMPI_PCONTROL_LEVEL
	 int level = Actual level to transmit
	 int type  = PNMPI_PCONTROL_SINGLE or MULTIPLE |
	             PNMPI_PCONTROL_VARG or PTR
	 int mod   = module number (if SINGLE)
	 int modnum / int* mods = module numbers (if MULTIPLE)
	 int *errors (if MULTIPLE and MULTIPLEERROR)
	 int size   = size of all arguments in byte
	 ...       = arguments (if VARG)
	 void *buf = arguments (if PTR) */
      
      actlevel=va_arg(va_alist,int);
      type=va_arg(va_alist,int);
      errors=NULL;
      if (type & PNMPI_PCONTROL_SINGLEorMULTIPLE)
	{
	  /* MULTIPLE */
	  modnum=va_arg(va_alist,int);
	  mods=va_arg(va_alist,int*);
	  mod=mods[0];
	  if (type & PNMPI_PCONTROL_SINGLEorMULTIPLEERROR)
	    {
	      errors=va_arg(va_alist,int*);
	    }
	}
      else
	{
	  /* SINGLE */
	  mod=va_arg(va_alist,int);
	  modnum=1;
	  mods=&mod;
	}
      size=va_arg(va_alist,int);
      if (type & PNMPI_PCONTROL_VARGorPTR)
	{
	  /* PTR */
	  start=(TRANSTYPE*) (va_arg(va_alist,void*));
	}
      else
	{
	  start=(TRANSTYPE*) va_alist;
	}
      leni=(size+sizeof(TRANSTYPE)-1)/sizeof(TRANSTYPE);

      error=MPI_SUCCESS;
      for (i=0; i<modnum; i++)
	{
	  if ((mods[i]<0) || (mods[i]>=pnmpi_max_level))
	    ret=PNMPI_ERROR;
	  else
	    {
	      if (pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[mods[i]]!=NULL)
		{
		  /* yes, we need to call this Pcontrol */
		  ret=PNMPI_Internal_CallVarg(pnmpi_function_ptrs.
					      pnmpi_int_MPI_Pcontrol[mods[i]],
					      actlevel,start,leni);
		}
	      else
		ret=MPI_SUCCESS;
	    }
	  if (ret!=MPI_SUCCESS) error=ret;
	  if (errors) errors[i]=ret;
	}

      va_end(va_alist);

      return error;
    }
  else
    {
#ifndef EXPERIMENTAL_UNWIND
      WARNPRINT("This Pcontrol option is not supported (enable EXPERIMENTAL_UNWIND)\n");
#else
      /* not our format, need to extract arguments */
      
      unw_getcontext(&ctx);  /* check for error */
      unw_init_local(&ptr,&ctx);
      unw_get_reg(&ptr, UNW_REG_SP, &sp1);
      unw_step(&ptr);
      unw_get_reg(&ptr, UNW_REG_SP, &sp2);

      /* Difference between stack frames minus all local vars */
      /* THIS MUST BE ADJUSTED IF THE LOCAL VARIABLES OF THIS
	 ROUTINE ARE CHANGED !!! */

      len=sp2-sp1-sizeof(ctx)-sizeof(ptr)-3*sizeof(sp1)-sizeof(start)-9*sizeof(int)-
	sizeof(va_alist)-2*sizeof(int*);

      unw_step(&ptr);
      unw_get_reg(&ptr, UNW_REG_SP, &sp1);
      len=sp1-sp2-len-sizeof(level);

      start=(TRANSTYPE*) (sp2+sizeof(level));

      leni=(len+sizeof(TRANSTYPE)-1)/sizeof(TRANSTYPE);

      error=MPI_SUCCESS;
      for (i=0; i<pnmpi_max_level; i++)
	{
	  if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i]!=NULL) &&
	      ((modules.pcontrol==PNMPI_PCONTROL_ON) ||
	       (modules.module[i]->pcontrol)))
	    {
	      /* yes, we need to call this Pcontrol */
	      
	      ret=PNMPI_Internal_CallVarg(pnmpi_function_ptrs.
					  pnmpi_int_MPI_Pcontrol[i],
					  level,start,leni);
	      if (ret!=MPI_SUCCESS) error=ret;
	    }
	}

#endif /*else EXPERIMENTAL_UNWIND*/
      va_end(va_alist);

      return error;
    }
}


void mpi_pcontrol_(int *level,int *ierr)
{
  int i,ret;

  DBGPRINT3("Entering Old Fortran MPI_Pcontrol at base level");

  if (modules.pcontrol==PNMPI_PCONTROL_OFF)
    {
      *ierr=MPI_SUCCESS;
      return;
    }
  if ((modules.pcontrol==PNMPI_PCONTROL_PNMPI) ||
      ((modules.pcontrol==PNMPI_PCONTROL_MIXED) && 
       (*level==PNMPI_PCONTROL_LEVEL)))
    {
      /* can't do that in Fortran */
      *ierr=PNMPI_ERROR;
      return;
    }
  
  *ierr=MPI_SUCCESS;
  for (i=0; i<pnmpi_max_level; i++)
    {
      if ((pnmpi_function_ptrs.pnmpi_int_MPI_Pcontrol[i]!=NULL) &&
	  ((modules.pcontrol==PNMPI_PCONTROL_ON) ||
	   (modules.module[i]->pcontrol)))
	{
	  /* yes, we need to call this Pcontrol */
	  ret=PNMPI_Internal_CallVarg(pnmpi_function_ptrs.
					pnmpi_int_MPI_Pcontrol[i],
					*level,NULL,0);
	  if (ret!=MPI_SUCCESS) *ierr=ret;
	}
    }
  return;
}

/*-------------------------------------------------------------------*/
/* special case for FORTRAN timing routines */

double mpi_wtick_(void)
{
  DBGPRINT3("Entering Old Fortran mpi_wtick_ at base level - Location = %px",&(MPI_Wtick));

  if (NOT_ACTIVATED(MPI_Wtick_MAJOR,MPI_Wtick_MINOR))
    return PMPI_Wtick();
  else
    return Internal_XMPI_Wtick();
}

double mpi_wtime_(void)
{
  DBGPRINT3("Entering Old Fortran mpi_wtime_ at base level - Location = %px",&(MPI_Wtime));

  if (NOT_ACTIVATED(MPI_Wtime_MAJOR,MPI_Wtime_MINOR))
    return PMPI_Wtime();
  else
    return Internal_XMPI_Wtime();
}


/*-------------------------------------------------------------------*/
/* The End. */
