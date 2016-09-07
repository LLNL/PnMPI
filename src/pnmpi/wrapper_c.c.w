/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2016 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2016 RWTH Aachen University, Federal Republic of Germany
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

#include "core.h"

{{forallfn fn_name MPI_Pcontrol MPI_Init MPI_Init_thread MPI_Finalize}}
{{ret_type}} {{sub {{fn_name}} '^MPI_' NQJ_}}({{formals}})
{
  {{ret_type}} res;
  int start_level, pnmpi_level;

  pnmpi_level=start_level=get_pnmpi_level();

  if (IS_ACTIVATED({{fn_name}}_ID) && (get_pnmpi_mpi_level() == 0) && (pnmpi_initialization_complete > 0))
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
	      set_pnmpi_level( start_level );
	      return res;
            }
          pnmpi_level=inc_pnmpi_level();
       }
    }

  DBGPRINT3("Calling a original MPI in {{fn_name}}");
  res=P{{fn_name}}({{args}});
  DBGPRINT3("Done with original MPI in {{fn_name}}");
  set_pnmpi_level( start_level );
  return res;
}
{{endforallfn}}


{{fnall fn_name MPI_Init MPI_Init_thread MPI_Pcontrol MPI_Finalize MPI_Wtick MPI_Wtime}}
{
  DBGPRINT3("Entering Old {{fn_name}} at base level - Location = %px",&({{fn_name}}));

  if (NOT_ACTIVATED({{fn_name}}_ID))
  { // in case we have no registered function of this type and we call PMPI directly, a subsequent call from the mpi library starts at level1
    int start_level=get_pnmpi_level();
    set_pnmpi_level( pnmpi_max_level );
    {{ret_type}} ret = P{{fn_name}}({{args}});
    set_pnmpi_level( start_level );
    return ret;
  }
  else if (get_pnmpi_mpi_level() > 0)
  {
    return P{{fn_name}}({{args}});
  }
  else
    {
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
    {{ret_val}}=Internal_X{{fn_name}}({{args}});
        #ifdef DBGLEVEL6
    if (DBGCHECK(DBGLEVEL6))
      pnmpi_totalstats_timing.{{fn_name}}=get_time_ns()-start_timer;
        #endif
        }
}
{{endfnall}}


{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}({{formals}})
{
  {{ret_type}} returnVal;
  inc_pnmpi_level();
  returnVal=Internal_X{{fn_name}}({{args}});
  dec_pnmpi_level();
  return returnVal;
}
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}_NewStack({{list "PNMPI_modHandle_t stack" {{formals}}}})
{
  {{ret_type}} returnVal;
  int current=get_pnmpi_level();
  if (stack>=0)
    set_pnmpi_level( stack );
  else
    set_pnmpi_level( current + 1 );

  returnVal=Internal_X{{fn_name}}({{args}});
  set_pnmpi_level( current );
  return returnVal;
}
{{endforallfn}}

