/* automatically generated wrapper code */

{{forallfn fn_name MPI_Init MPI_Pcontrol MPI_Finalize}}
{{retType}} {{fn_name}}{{argTypeList}}
{
  int _stack=-1;
  {{MACRO_MPI_Comm}}
  return P{{fn_name}}_NewStack(_stack {{argListOpen}});
}
{{endforallfn}}

