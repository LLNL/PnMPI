/* automatically generated wrapper header */

{{forallfnpp fnpp_name fn_name MPI_Pcontrol}}
{{retType}} {{fnpp_name}}{{argTypeListVoid}};
{{endforallfnpp}}

{{forallfn fn_name MPI_Pcontrol}}
{{retType}} X{{fn_name}}{{argTypeListVoid}};
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{retType}} X{{fn_name}}_NewStack(PNMPI_modHandle_t stack {{argTypeListOpen}});
{{endforallfn}}


{{forallfn fn_name}}
#define {{fn_name}}_MAJOR ({{fn_num}}/8)
#define {{fn_name}}_MINOR (1<<({{last_num}}%8))
{{endforallfn}}

{{forallfnpp fnpp_name fn_name MPI_Pcontrol}}
#define Internal_X{{fn_name}} {{fnpp_name}}
{{endforallfnpp}}

{{forallfn fn_name MPI_Pcontrol}}
typedef {{retType}} (*pnmpi_int_{{fn_name}}_t){{argTypeList}};{{endforallfn}}

typedef struct pnmpi_functions_d
{
{{forallfn fn_name}}  pnmpi_int_{{fn_name}}_t *pnmpi_int_{{fn_name}};
{{endforallfn}}
} pnmpi_functions_t;


#define INITIALIZE_ALL_FUNCTION_STACKS(mods) \
{{forallfn fn_name}}INITIALIZE_FUNCTION_STACK("{{fn_name}}",{{fn_name}}_MAJOR,{{fn_name}}_MINOR,pnmpi_int_{{fn_name}}_t,pnmpi_int_{{fn_name}},mods);\
{{endforallfn}}

