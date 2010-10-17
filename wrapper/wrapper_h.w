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

