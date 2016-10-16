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

#include "core.h"
#include <stdarg.h>

#ifdef DBGLEVEL
int _dbg_cur_level = DBGLEVEL;
int _dbg_cur_node = -1;
#endif

int _print_node = -2;


/*-----------------------------------------------------------------*/
/* Not GNU */

#ifndef __GNUC__

void dbg_statusprint1(char *format, ...);
void dbg_statusprintn(char *format, ...);
void dbg_warning(char *format, ...);
void dbg_debugprint1(char *format, ...);
void dbg_debugprint2(char *format, ...);
void dbg_debugprint3(char *format, ...);
void dbg_debugprint4(char *format, ...);

#ifdef NOSTATUS
void dbg_statusprint1(char *format, ...)
{
}
void dbg_statusprintn(char *format, ...)
{
}
#else

void dbg_statusprint1(char *format, ...)
{
  va_list va_alist;

  if (_print_node == 0)
    {
      va_start(va_alist, format);
      fprintf(stdout, "%5i: ", _print_node);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }

  if (_print_node == -2)
    {
      va_start(va_alist, format);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}

void dbg_statusprintn(char *format, ...)
{
  va_list va_alist;

  if (_print_node > -2)
    {
      va_start(va_alist, format);
      fprintf(stdout, "%5i: ", _print_node);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
  else
    {
      va_start(va_alist, format);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}
#endif /* else of NOSTATUS */

#ifdef NOWARNINGS
void dbg_warning(char *format, ...)
{
}
#else
void dbg_warning(char *format, ...)
{
  va_list va_alist;

  va_start(va_alist, format);
  fprintf(stdout, "WARNING: ");
  vfprintf(stdout, format, va_alist);
  fprintf(stdout, "\n");
  va_end(va_alist);
}
#endif /* else of NOWARNINGS */

#ifndef DBGLEVEL
void dbg_debugprint1(char *format, ...)
{
}
void dbg_debugprint2(char *format, ...)
{
}
void dbg_debugprint3(char *format, ...)
{
}
void dbg_debugprint4(char *format, ...)
{
}
void dbg_debugprint5(char *format, ...)
{
}
void dbg_debugprint6(char *format, ...)
{
}
#else
void dbg_debugprint1(char *format, ...)
{
  va_list va_alist;

  if (((DBGLEVEL1)&_dbg_cur_level) &&
      ((_dbg_cur_node == _print_node) || (_print_node < 0) ||
       (_dbg_cur_node < 0)))
    {
      va_start(va_alist, format);
      fprintf(stdout, "L%04x ", format, _dbg_cur_level);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}

void dbg_debugprint2(char *format, ...)
{
  va_list va_alist;

  if (((DBGLEVEL2)&_dbg_cur_level) &&
      ((_dbg_cur_node == _print_node) || (_print_node < 0) ||
       (_dbg_cur_node < 0)))
    {
      va_start(va_alist, format);
      fprintf(stdout, "L%04x ", format, _dbg_cur_level);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}

void dbg_debugprint3(char *format, ...)
{
  va_list va_alist;

  if (((DBGLEVEL3)&_dbg_cur_level) &&
      ((_dbg_cur_node == _print_node) || (_print_node < 0) ||
       (_dbg_cur_node < 0)))
    {
      va_start(va_alist, format);
      fprintf(stdout, "L%04x ", format, _dbg_cur_level);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}

void dbg_debugprint4(char *format, ...)
{
  va_list va_alist;

  if (((DBGLEVEL4)&_dbg_cur_level) &&
      ((_dbg_cur_node == _print_node) || (_print_node < 0) ||
       (_dbg_cur_node < 0)))
    {
      va_start(va_alist, format);
      fprintf(stdout, "L%04x ", format, _dbg_cur_level);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}

#ifdef DBGLEVEL5
void dbg_debugprint5(char *format, ...)
{
  va_list va_alist;

  if (((DBGLEVEL5)&_dbg_cur_level) &&
      ((_dbg_cur_node == _print_node) || (_print_node < 0) ||
       (_dbg_cur_node < 0)))
    {
      va_start(va_alist, format);
      fprintf(stdout, "L%04x ", format, _dbg_cur_level);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}
#else
void dbg_debugprint5(char *format, ...)
{
}
#endif

#ifdef DBGLEVEL6
void dbg_debugprint6(char *format, ...)
{
  va_list va_alist;

  if (((DBGLEVEL6)&_dbg_cur_level) &&
      ((_dbg_cur_node == _print_node) || (_print_node < 0) ||
       (_dbg_cur_node < 0)))
    {
      va_start(va_alist, format);
      fprintf(stdout, "L%04x ", format, _dbg_cur_level);
      vfprintf(stdout, format, va_alist);
      fprintf(stdout, "\n");
      va_end(va_alist);
    }
}
#else
void dbg_debugprint6(char *format, ...)
{
}
#endif

#endif /* else of not def DBGLEVEL */

#endif /* __GNUC__*/
