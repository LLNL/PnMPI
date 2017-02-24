/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2017 Lawrence Livermore National Laboratories, United States of America
 *  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2017 RWTH Aachen University, Federal Republic of Germany
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

/* \brief Count MPI calls.
 *
 * \details This module counts how often each MPI call was invoked. Before MPI
 *  finalizes, statistics will be printed for each rank and a sum of the made
 *  calls by all ranks.
 */


#include <stdio.h>

#include <mpi.h>
#include <pnmpi/debug_io.h>
#include <pnmpi/hooks.h>
#include <xmpi.h>

#include "atomic.h"


/* If there is no atomic support, we'll limit the threading level of this module
 * to MPI_THREAD_SERIALIZED, so it is safe to use with threaded applications
 * (but they may become slower!). */
#if defined(METRIC_NO_ATOMIC)
int PNMPI_SupportedThreadingLevel = MPI_THREAD_SERIALIZED;
#endif


/** \brief Struct of counters.
 *
 * \details This struct stores the counters for all MPI calls.
 */
static struct counter
{
  {{forallfn fn_name MPI_Finalize}}
  metric_atomic_keyword size_t {{fn_name}};
  {{endforallfn}}
} counters;


/** \brief Helper function to print counter struct.
 *
 * \details This function will print all functions called at least once with
 *  their counter.
 *
 *
 * \param c \ref counter struct to be printed.
 */
static void print_counters(struct counter *c)
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
 * \param c \ref counter struct to be initialized.
 */
static void init_counters(struct counter *c)
{
  {{forallfn fn_name MPI_Finalize}}
  c->{{fn_name}} = metric_atomic_init(0);
  {{endforallfn}}
}


/** \brief PnMPI registration point.
 *
 * \details This function sets all counters to zero.
 */
void PNMPI_RegistrationPoint()
{
  init_counters(&counters);
}


/* Generate wrapper functions for all MPI calls to increment the counter on
 * every call. MPI_Finalize will be ignored, because it will be used to print
 * the statistics below. */

{{fnall fn_name MPI_Finalize MPI_Pcontrol}}
  metric_atomic_inc(counters.{{fn_name}});

  X{{fn_name}}({{args}});
{{endfnall}}


/* MPI_Pcontrol needs special handling, as it doesn't call a PMPI function and
 * PnMPI does not implement the required XMPI call. Instead PnMPI will act as a
 * multiplexer for MPI_Pcontrol, so all we have to do is increment the counter
 * and return. */

int MPI_Pcontrol(const int level, ...)
{
  metric_atomic_inc(counters.MPI_Pcontrol);

  return MPI_SUCCESS;
}


/** \brief Print the statistics.
 *
 * \details This function will print the statistics to stdout for each rank and
 *  a sum of all ranks.
 *
 * \todo In feature releases this should be done by the PNMPI_AppShutdown hook
 *  to track duplicated MPI_Finalize calls.
 *
 *
 * \return The return value of PMPI_Finalize will be pass through.
 */
int MPI_Finalize()
{
  /* Call original MPI_Finalize. */
  int ret = XMPI_Finalize();


  /* Wait until all ranks have flushed their buffers to avoid rank output
   * between the statistics output. */
  fflush(stdout);
  fflush(stderr);
  if (PMPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS)
    pnmpi_error("PMPI_Barrier failed.\n");


  int rank, size;

  if (PMPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
    pnmpi_error("PMPI_Comm_rank failed.\n");
  if (PMPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
    pnmpi_error("PMPI_Comm_size failed.\n");

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
      if (PMPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS)
        pnmpi_error("PMPI_Barrier failed.\n");
    }


  /* Reduce statistics of all ranks to rank 0. */
  {{forallfn fn_name MPI_Finalize}}
    if (PMPI_Reduce(&(counters.{{fn_name}}), &(tmp.{{fn_name}}), 1,
                    MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD)
        != MPI_SUCCESS)
      pnmpi_error("PMPI_Reduce failed for %s counter.\n", "{{fn_name}}");
  {{endforallfn}}

  if (rank == 0) {
    printf("Total:\n");
    print_counters(&tmp);
    fflush(stdout);
  }


  return ret;
}
