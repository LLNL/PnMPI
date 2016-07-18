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

#include <stdio.h>

#include <mpi.h>
#include <pnmpimod.h>


/* If C11 is available and atomics are supported, use them for the counters,
 * because these provide the most simple to use interface. */
#if (__STDC_VERSION__ >= 201112L) && (!__STDC_NO_ATOMICS__)

#include <stdatomic.h>

#define pnmpi_counter _Atomic unsigned long int
#define pnmpi_counter_init(x) ATOMIC_VAR_INIT(x)
#define pnmpi_counter_inc(x) x++


/* If no thread safe counter is available, print a warning and use default C
 * types. */
#else

#define pnmpi_counter unsigned long int
#define pnmpi_counter_init(x) x
#define pnmpi_counter_inc(x) x++

#endif


/** \brief Struct of counters.
 *
 * \details This struct stores the counters for all MPI calls. If atomic
 *  counters could have been found, they will be used.
 */
struct counter
{
  {{forallfn fn_name MPI_Finalize}}
  pnmpi_counter {{fn_name}};
  {{endforallfn}}
} counters;


/** \brief Helper function to print counter struct.
 *
 * \details This function will print all functions called at least once with
 *  their counter.
 *
 *
 * \param counter Counter struct to be printed.
 */
void print_counters(struct counter *c)
{
  {{forallfn fn_name MPI_Finalize}}
    if (c->{{fn_name}} > 0)
      printf("  %8zu %s\n", c->{{fn_name}}, "{{fn_name}}");
  {{endforallfn}}
}


/** \brief Helper function to initialize counter struct.
 *
 * \details This function will set all counters in \p counter to zero.
 *
 *
 * \param counter Counter struct to be initialized.
 */
void init_counters(struct counter *c)
{
  {{forallfn fn_name MPI_Finalize}}
    c->{{fn_name}} = pnmpi_counter_init(0);
  {{endforallfn}}
}


/** \brief PnMPI registration point.
 *
 * \details This function sets all counters to zero.
 *
 *
 * \return \ref PNMPI_SUCCESS will be returned for every call.
 */
int PNMPI_RegistrationPoint()
{
  init_counters(&counters);

  return PNMPI_SUCCESS;
}


/* Generate wrapper functions for all MPI calls to increment the counter on
 * every call. MPI_Finalize will be ignored, because it will be used to print
 * the statistics below. */

{{fnall fn_name MPI_Finalize}}
  pnmpi_counter_inc(counters.{{fn_name}});

  {{callfn}}
{{endfnall}}


/** \brief Print the statistics.
 *
 * \details This function will print the statistics to stdout for each rank and
 *  a sum of all ranks.
 *
 * \note In feature releases this should be done by the app_shutdown hook to
 *  track duplicated MPI_Finalize calls.
 *
 *
 * \return The return value of PMPI_Finalize will be pass through.
 */
int MPI_Finalize()
{
  int rank, size;
  PMPI_Comm_rank(MPI_COMM_WORLD, &rank);
  PMPI_Comm_size(MPI_COMM_WORLD, &size);

  /* Iterate over all ranks and tell one rank after another to print his
   * statistics. The per-rank statistics will be reduced to rank 0 to get the
   * total stats. */
  struct counter tmp;
  init_counters(&tmp);

  int n = 0;
  while (n < size)
    {
      if (rank == n)
        {
          if (rank == 0)
            printf("\n\nStats:\n");

          printf("Rank %d:\n", rank);
          print_counters(&counters);
          fflush(stdout);
        }

      n++;

      /* Wait until all ranks have finished processing rank n.
       *
       * This solution was inspired by: http://stackoverflow.com/a/5310506 */
      PMPI_Barrier(MPI_COMM_WORLD);
    }


  /* Reduce statistics of all ranks to rank 0. */
  {{forallfn fn_name MPI_Finalize}}
    PMPI_Reduce(&(counters.{{fn_name}}), &(tmp.{{fn_name}}), 1,
                MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  {{endforallfn}}

  if (rank == 0) {
    printf("Total:\n");
    print_counters(&tmp);
    fflush(stdout);
  }


  /* Call original MPI_Finalize. */
  return PMPI_Finalize();
}
