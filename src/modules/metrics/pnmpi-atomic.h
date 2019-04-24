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

#ifndef METRICS_ATOMIC_H
#define METRICS_ATOMIC_H


#include "config.h"


/* For both the timing and the counter module we need atomic operations,
 * otherwise counters may be corrupted for MPI applications with concurrent
 * calls (MPI threading level MPI_THREAD_MULTIPLE).
 *
 * We'll try to use C11 atomics or GCCs builtin __atomic or __sync to get atomic
 * operations. If no atomics are supported by the compiler, we'll set the
 * METRIC_NO_ATOMIC macro, so the modules can handle this. */

#if defined(C11_ATOMICS_FOUND)

#include <stdatomic.h>

#define metric_atomic_keyword _Atomic
#define metric_atomic_inc(var) ++var;
#define metric_atomic_dec(var) --var;
#define metric_atomic_add(var, value) var += value;

#elif defined(BUILTINATOMIC_FOUND)

#define metric_atomic_keyword
#define metric_atomic_inc(var) __atomic_add_fetch(&(var), 1, __ATOMIC_SEQ_CST)
#define metric_atomic_dec(var) __atomic_sub_fetch(&(var), 1, __ATOMIC_SEQ_CST)
#define metric_atomic_add(var, value) \
  __atomic_fetch_add(&(var), value, __ATOMIC_SEQ_CST)

#elif defined(BUILTINSYNC_FOUND)

#define metric_atomic_keyword
#define metric_atomic_inc(var) __sync_add_and_fetch(&(var), 1)
#define metric_atomic_dec(var) __sync_sub_and_fetch(&(var), 1)
#define metric_atomic_add(var, value) __sync_fetch_and_add(&(var), value)

#else

#define METRIC_NO_ATOMIC
#define metric_atomic_keyword volatile
#define metric_atomic_inc(var) ++var;
#define metric_atomic_dec(var) --var;
#define metric_atomic_add(var, value) var += value;

#endif


#endif
