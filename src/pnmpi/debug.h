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

/* debug macros */


/*---------------------------------------------------*/
/* GNU */

#ifdef __GNUC__

#ifdef NOSTATUS
#define STATUSPRINT1(format, args...)
#define STATUSPRINTN(format, args...)
#define STATUSINIT()
#else
#define STATUSPRINT1(format, args...) \
  {                                   \
    if (_print_node == 0)             \
      DOPRINT(format, ##args);        \
  }
#define STATUSPRINTN(format, args...) DOPRINT(format, ##args)
#define STATUSINIT() \
  {                  \
  }
#endif

#ifdef NOWARNINGS
#define WARNRPINT(format, args...)
#else
#define WARNPRINT(format, args...) DOPRINT("%s" format, "WARNING: ", ##args)
#endif

#ifdef DBGLEVEL
#define DBGLEVEL1 0x0001 /* entry and exit prints */
#define DBGLEVEL2 0x0002 /* module load and instantiation */
#define DBGLEVEL3 0x0004 /* entry and exit of layers */
#define DBGLEVEL4 0x0008 /* arguments and parse information */
#endif                   /*DBGLEVEL*/

#define DBGPRINT1(format, args...) DBGPRINT(DBGLEVEL1, format, ##args)
#define DBGPRINT2(format, args...) DBGPRINT(DBGLEVEL2, format, ##args)
#define DBGPRINT3(format, args...) DBGPRINT(DBGLEVEL3, format, ##args)
#define DBGPRINT4(format, args...) DBGPRINT(DBGLEVEL4, format, ##args)

#ifdef DBGLEVEL

extern int _dbg_cur_level;
extern int _dbg_cur_node;
#define DBGPRINT(level, format, args...)                        \
  {                                                             \
    if ((level & _dbg_cur_level) &&                             \
        ((_dbg_cur_node == _print_node) || (_print_node < 0) || \
         (_dbg_cur_node < 0)))                                  \
      DOPRINT("L%04x " format, _dbg_cur_level, ##args);         \
  }
#define DBGCHECK(level) ((level & _dbg_cur_level) != 0)
#define DBGLATEINIT() \
  {                   \
  }
#define DBGEARLYINIT()                            \
  {                                               \
    char *dbglevel, *dbgnode;                     \
    dbglevel = getenv("DBGLEVEL");                \
    if (dbglevel != NULL)                         \
      _dbg_cur_level = strtol(dbglevel, NULL, 0); \
    dbgnode = getenv("DBGNODE");                  \
    if (dbgnode != NULL)                          \
      _dbg_cur_node = strtol(dbgnode, NULL, 0);   \
  }
#else

#define DBGPRINT(level, format, args...)
#define DBGCHECK(level) (0)
#define DBGEARLYINIT()
#define DBGLATEINIT()
#endif

extern int _print_node;
#define PRINTINIT()                              \
  {                                              \
    MPI_Comm_rank(MPI_COMM_WORLD, &_print_node); \
  }
#define DOPRINT(format, args...)                        \
  {                                                     \
    if (_print_node > -2)                               \
      printf("%5i: " format "\n", _print_node, ##args); \
    else                                                \
      printf(format "\n", ##args);                      \
    fflush(stdout);                                     \
  }


/*-----------------------------------------------------------------*/
/* Not GNU */

#else /* i.e. COMP!=GNU */

void dbg_statusprint1(char *format, ...);
void dbg_statusprintn(char *format, ...);
void dbg_warning(char *format, ...);
void dbg_debugprint1(char *format, ...);
void dbg_debugprint2(char *format, ...);
void dbg_debugprint3(char *format, ...);
void dbg_debugprint4(char *format, ...);

#ifdef NOSTATUS
#define STATUSPRINT1 dbg_statusprint1
#define STATUSPRINTN dbg_statusprintn
#define STATUSINIT()
#else
#define STATUSPRINT1 dbg_statusprint1
#define STATUSPRINTN dbg_statusprintn
#define STATUSINIT() // {MPI_Comm_rank(MPI_COMM_WORLD,&_print_node);}
#endif

#ifdef NOWARNINGS
#define WARNPRINT dbg_warning
#else
#define WARNPRINT dbg_warning
#endif

#define DBGLEVEL1 0x0001 /* entry and exit prints */
#define DBGLEVEL2 0x0002 /* module load and instantiation */
#define DBGLEVEL3 0x0004 /* entry and exit of layers */
#define DBGLEVEL4 0x0008 /* arguments and parse information */

#define DBGPRINT1 dbg_debugprint1
#define DBGPRINT2 dbg_debugprint2
#define DBGPRINT3 dbg_debugprint3
#define DBGPRINT4 dbg_debugprint4

#ifdef DBGLEVEL
extern int _dbg_cur_level;
extern int _dbg_cur_node;

#define DBGCHECK(level) ((level & _dbg_cur_level) != 0)
#define DBGLATEINIT() \
  {                   \
  }
#define DBGEARLYINIT()                            \
  {                                               \
    char *dbglevel, *dbgnode;                     \
    dbglevel = getenv("DBGLEVEL");                \
    if (dbglevel != NULL)                         \
      _dbg_cur_level = strtol(dbglevel, NULL, 0); \
    dbgnode = getenv("DBGNODE");                  \
    if (dbgnode != NULL)                          \
      _dbg_cur_node = strtol(dbgnode, NULL, 0);   \
  }
#else /* ifdef DBGLEVEL */
#define DBGCHECK(level) (0)
#define DBGLATEINIT()
#define DBGEARLYINIT()
#endif

extern int _print_node;
#define PRINTINIT()                              \
  {                                              \
    MPI_Comm_rank(MPI_COMM_WORLD, &_print_node); \
  }

#endif /* else of COMP==GNU */
