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

#include "core.h"


{{forallfn fn_name MPI_Pcontrol MPI_Init MPI_Init_thread MPI_Finalize}}
{{ret_type}} {{sub {{fn_name}} '^MPI_' NQJ_}}({{formals}})
{
  {{ret_type}} res;
  int start_level = pnmpi_level;

  if (IS_ACTIVATED({{fn_name}}_ID) && (get_pnmpi_mpi_level() == 0) && (pnmpi_initialization_complete > 0))
    {
       while ((pnmpi_level<pnmpi_max_level) && (modules.module[pnmpi_level]->stack_delimiter==0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_{{fn_name}}[pnmpi_level]!=NULL)
	    {
          WRAP_MPI_CALL_PREFIX
          DBGPRINT3("Calling a wrapper in {{fn_name}} at level %i FROM %px",pnmpi_level,&(Internal_X{{fn_name}}));
          res=(pnmpi_function_ptrs.pnmpi_int_{{fn_name}})[pnmpi_level]({{args}});
          WRAP_MPI_CALL_POSTFIX
	      DBGPRINT3("Done with wrapper in {{fn_name}} at level %i - reseting to %i",pnmpi_level,start_level);
	      pnmpi_level = start_level;
	      return res;
            }
          ++pnmpi_level;
       }
    }

  DBGPRINT3("Calling a original MPI in {{fn_name}}");
  WRAP_MPI_CALL_PREFIX
  res=P{{fn_name}}({{args}});
  WRAP_MPI_CALL_POSTFIX
  DBGPRINT3("Done with original MPI in {{fn_name}}");
  pnmpi_level = start_level;
  return res;
}
{{endforallfn}}


{{fnall fn_name MPI_Init MPI_Init_thread MPI_Pcontrol MPI_Finalize MPI_Wtick MPI_Wtime}}
{
  WRAP_MPI_CALL_PREFIX
  DBGPRINT3("Entering Old {{fn_name}} at base level - Location = %px",&({{fn_name}}));
  WRAP_MPI_CALL_POSTFIX

  if (NOT_ACTIVATED({{fn_name}}_ID))
  { // in case we have no registered function of this type and we call PMPI directly, a subsequent call from the mpi library starts at level1
    int start_level = pnmpi_level;
    pnmpi_level = pnmpi_max_level;
    WRAP_MPI_CALL_PREFIX
    {{ret_type}} ret = P{{fn_name}}({{args}});
    WRAP_MPI_CALL_POSTFIX
    pnmpi_level = start_level;
    return ret;
  }
  else if (get_pnmpi_mpi_level() > 0)
  {
    WRAP_MPI_CALL_PREFIX
    return P{{fn_name}}({{args}});
    WRAP_MPI_CALL_POSTFIX
  }
  else
    {
    /* Store the return address to the application, and the address of the
     * original MPI call, so modules may check the origin of this MPI call. */
    pnmpi_set_return_address();
    pnmpi_set_function_address({{fn_name}});

    WRAP_MPI_CALL_PREFIX
    {{ret_val}}=Internal_X{{fn_name}}({{args}});
    WRAP_MPI_CALL_POSTFIX

    /* Reset the return and function address to the default one. */
    pnmpi_reset_return_address();
    pnmpi_reset_function_address();
  }
}
{{endfnall}}


{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}({{formals}})
{
  {{ret_type}} returnVal;
  ++pnmpi_level;
  WRAP_MPI_CALL_PREFIX
  returnVal=Internal_X{{fn_name}}({{args}});
  WRAP_MPI_CALL_POSTFIX
  --pnmpi_level;
  return returnVal;
}
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}_NewStack({{list "PNMPI_modHandle_t stack" {{formals}}}})
{
  {{ret_type}} returnVal;
  int current = pnmpi_level;
  if (stack>=0)
    pnmpi_level = stack;
  else
    pnmpi_level++;

  WRAP_MPI_CALL_PREFIX
  returnVal=Internal_X{{fn_name}}({{args}});
  WRAP_MPI_CALL_POSTFIX
  pnmpi_level = current;
  return returnVal;
}
{{endforallfn}}
