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

/* automatically generated wrapper header */

typedef int (*pnmpi_int_MPI_Pcontrol_t)(int level, ... );

void mpi_init_(int *ierr);
double mpi_wtick_(void);
double mpi_wtime_(void);

{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} {{sub {{fn_name}} '^MPI_' NQJ_}}({{formals}});
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}({{formals}});
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
{{ret_type}} X{{fn_name}}_NewStack({{list "PNMPI_modHandle_t stack" {{formals}}}});
{{endforallfn}}


{{forallfn fn_name}}
#define {{fn_name}}_ID {{fn_num}}
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
#define Internal_X{{fn_name}} {{sub {{fn_name}} '^MPI_' NQJ_}}
{{endforallfn}}

{{forallfn fn_name MPI_Pcontrol}}
typedef {{ret_type}} (*pnmpi_int_{{fn_name}}_t)({{formals}});{{endforallfn}}

typedef struct pnmpi_functions_d
{
{{forallfn fn_name}}  pnmpi_int_{{fn_name}}_t *pnmpi_int_{{fn_name}};
{{endforallfn}}
} pnmpi_functions_t;

#ifdef DBGLEVEL5
typedef struct pnmpi_functions_statscount_d
{
{{forallfn fn_name}} long {{fn_name}};
{{endforallfn}}
} pnmpi_functions_statscount_t;

#define DBGPRINT5_COUNTS_ALL_ALL(buf,num,div,out) \
{{forallfn fn_name}}DBGPRINT5_COUNTS_FCT_ALL(buf,num,div,out,{{fn_name}},"{{fn_name}}");\
{{endforallfn}}
#endif

#ifdef DBGLEVEL6
typedef struct pnmpi_functions_statstiming_d
{
{{forallfn fn_name}}  timing_t {{fn_name}};
{{endforallfn}}
} pnmpi_functions_statstiming_t;


#define DBGPRINT6_TIMING_ALL_ALL(buf,num,div,out) \
{{forallfn fn_name}}DBGPRINT6_TIMING_FCT_ALL(buf,num,div,out,{{fn_name}},"{{fn_name}}");\
{{endforallfn}}
#endif

#define INITIALIZE_ALL_FUNCTION_STACKS(mods) \
{{forallfn fn_name}}INITIALIZE_FUNCTION_STACK("{{fn_name}}", {{fn_name}}_ID, pnmpi_int_{{fn_name}}_t, pnmpi_int_{{fn_name}}, mods, {{fn_name}});\
{{endforallfn}}

/*-------------------------------------------------------------------*/
/* timing and count variables for DBGLEVEL 5 and 6 */

#ifdef DBGLEVEL5
extern pnmpi_functions_statscount_t pnmpi_totalstats_count;
#endif
#ifdef DBGLEVEL6
extern pnmpi_functions_statstiming_t pnmpi_totalstats_timing;
extern timing_t pnmpi_overall_timing;
#endif


