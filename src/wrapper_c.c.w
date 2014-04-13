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

/* automatically generated wrapper code */

#include "core.h"

{{forallfn fn_name MPI_Pcontrol MPI_Init MPI_Finalize}}
{{ret_type}} {{sub {{fn_name}} '^MPI_' NQJ_}}({{formals}})
{
  {{ret_type}} res;
  int start_level;

  start_level=pnmpi_level;

  if (IS_ACTIVATED({{fn_name}}_ID))
    {
       while ((pnmpi_level<pnmpi_max_level) && (modules.module[pnmpi_level]->stack_delimiter==0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_{{fn_name}}[pnmpi_level]!=NULL)
	    {
		  #ifdef DBGLEVEL6
		  timing_t start_timer;
		  #endif
	      DBGPRINT3("Calling a wrapper in {{fn_name}} at level %i FROM %px",pnmpi_level,&(Internal_X{{fn_name}}));
		  #ifdef DBGLEVEL5
		  if (DBGCHECK(DBGLEVEL5))
		    modules.module[pnmpi_level]->statscount.{{fn_name}}++;
		  #endif
		  #ifdef DBGLEVEL6
		  if (DBGCHECK(DBGLEVEL6))
		    start_timer=get_time_ns();
		  #endif
      res=(pnmpi_function_ptrs.pnmpi_int_{{fn_name}})[pnmpi_level]({{args}});
		  #ifdef DBGLEVEL6
		  if (DBGCHECK(DBGLEVEL6))
		    modules.module[pnmpi_level]->statstiming.{{fn_name}}+=get_time_ns()-start_timer;
		  #endif
	      DBGPRINT3("Done with wrapper in {{fn_name}} at level %i - reseting to %i",pnmpi_level,start_level);
	      pnmpi_level=start_level;
	      return res;
            }
          pnmpi_level++;
       }
    }

  DBGPRINT3("Calling a original MPI in {{fn_name}}");
  res=P{{fn_name}}({{args}});
  DBGPRINT3("Done with original MPI in {{fn_name}}");
  pnmpi_level=start_level;
  return res;
}
{{endforallfn}}


{{forallfn fn_name MPI_Init MPI_Pcontrol MPI_Finalize}}
{{ret_type}} {{fn_name}}({{formals}})
{
  DBGPRINT3("Entering Old {{fn_name}} at base level - Location = %px",&({{fn_name}}));

  if (NOT_ACTIVATED({{fn_name}}_ID))
    return P{{fn_name}}({{args}});
  else
    {
	int err;
	#ifdef DBGLEVEL6
	timing_t start_timer;
	#endif
    #ifdef DBGLEVEL5
    if (DBGCHECK(DBGLEVEL5))
	  pnmpi_totalstats_count.{{fn_name}}++;
	#endif
	#ifdef DBGLEVEL6
    if (DBGCHECK(DBGLEVEL6))
      start_timer=get_time_ns();
	#endif
    err=Internal_X{{fn_name}}({{args}});
	#ifdef DBGLEVEL6
    if (DBGCHECK(DBGLEVEL6))
      pnmpi_totalstats_timing.{{fn_name}}=get_time_ns()-start_timer;
	#endif
	return err;
	}
}
{{endforallfn}}


{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}({{formals}})
{
  {{ret_type}} returnVal;
  pnmpi_level++;
  returnVal=Internal_X{{fn_name}}({{args}});
  pnmpi_level--;
  return returnVal;
}
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}_NewStack({{list "PNMPI_modHandle_t stack" {{formals}}}})
{
  {{ret_type}} returnVal;
  int current=pnmpi_level;
  if (stack>=0)
    pnmpi_level=stack;
  else
    pnmpi_level++;

  returnVal=Internal_X{{fn_name}}({{args}});
  pnmpi_level=current;
  return returnVal;
}
{{endforallfn}}
