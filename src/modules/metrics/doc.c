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

/* This file is for documentation purposes only any will not be compiled into
 * any object! */


/** \class module_metrics_counter
 *
 * \brief Module to count how often MPI functions got called.
 *
 * \details This PnMPI specific module counts the MPI call invocations. Add the
 *  module at the top of your config file to count how often each rank invoked
 *  which MPI call or in front of a specific module to count how often
 *  invocations reached this module.
 */


/** \class module_metrics_timing
 *
 * \brief Module to measure the time MPI calls take.
 *
 * \details This PnMPI specific module measures the time MPI call invocations
 *  take. It has two different operation modes:
 *
 *  \li \b simple: Add it in front of a module and it will measure the time of
 *      all following modules (including the real MPI call).
 *      \n
 *      \par Example:
 *      \code
 *      module metrics-timing
 *      module sample1
 *      \endcode
 *
 *  \li \b advanced: Add the timing module before and after the modules you want
 *      to measure and it will only measure the time of these, but not the
 *      following modules. \n
 *      \n
 *      \par Example:
 *      \code
 *      module metrics-timing
 *      module sample1
 *      module sample2
 *      module metrics-timing
 *
 *      module empty
 *      \endcode
 *
 *      \note Measuring `MPI_Pcontrol` is available in advanced mode, only. To
 *       measure `MPI_Pcontrol` calls both `metrics-timing` invocations need to
 *       be pcontrol enabled:
 *       \par Example:
 *       \code
 *       module metrics-timing
 *       pcontrol on # May be ignored if metrics-timing is the first module.
 *       module sample1
 *       module metrics-timing
 *       pcontrol on
 *       \endcode
 */
