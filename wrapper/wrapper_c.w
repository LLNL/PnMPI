/* automatically generated wrapper code */

{{forallfnpp fnpp_name fn_name MPI_Pcontrol MPI_Init MPI_Finalize}}
{{retType}} {{fnpp_name}}{{argTypeListVoid}}
{
  {{retType}} res;
  int start_level;
  
  start_level=pnmpi_level;
  
  if (IS_ACTIVATED({{fn_name}}_MAJOR,{{fn_name}}_MINOR))
    {
       while ((pnmpi_level<pnmpi_max_level) && (modules.module[pnmpi_level]->stack_delimiter==0))
        {
          if (pnmpi_function_ptrs.pnmpi_int_{{fn_name}}[pnmpi_level]!=NULL)
	    {
	      DBGPRINT3("Calling a wrapper in {{fn_name}} at level %i FROM %px",pnmpi_level,&(Internal_X{{fn_name}}));
	      res=(pnmpi_function_ptrs.pnmpi_int_{{fn_name}})[pnmpi_level]{{argList}};
	      DBGPRINT3("Done with wrapper in {{fn_name}} at level %i - reseting to %i",pnmpi_level,start_level);
	      pnmpi_level=start_level;
	      return res;
            }
          pnmpi_level++;
       }
    }

  DBGPRINT3("Calling a original MPI in {{fn_name}}");
  res=P{{fn_name}}{{argList}};
  DBGPRINT3("Done with original MPI in {{fn_name}}");
  pnmpi_level=start_level;
  return res;
}
{{endforallfnpp}}


{{forallfn fn_name MPI_Init MPI_Pcontrol MPI_Finalize}}
{{retType}} {{fn_name}}{{argTypeListVoid}}
{
  DBGPRINT3("Entering Old {{fn_name}} at base level - Location = %px",&({{fn_name}}));

  if (NOT_ACTIVATED({{fn_name}}_MAJOR,{{fn_name}}_MINOR))
    return P{{fn_name}}{{argList}};
  else
    return Internal_X{{fn_name}}{{argList}};
}
{{endforallfn}}


{{forallfn fn_name MPI_Pcontrol}}
{{retType}} X{{fn_name}}{{argTypeListVoid}}
{
  {{retType}} returnVal;
  pnmpi_level++;
  returnVal=Internal_X{{fn_name}}{{argList}};
  pnmpi_level--;
  return returnVal;
}
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{retType}} X{{fn_name}}_NewStack(PNMPI_modHandle_t stack {{argTypeListOpen}})
{
  {{retType}} returnVal;
  int current=pnmpi_level;
  if (stack>=0)
    pnmpi_level=stack;
  else
    pnmpi_level++;

  returnVal=Internal_X{{fn_name}}{{argList}};
  pnmpi_level=current;
  return returnVal;
}
{{endforallfn}}


