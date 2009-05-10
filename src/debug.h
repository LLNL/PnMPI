/* debug macros */


/*---------------------------------------------------*/
/* GNU */

#ifdef __GNUC__

#ifdef NOSTATUS
#define STATUSPRINT1(format,args...)
#define STATUSPRINTN(format,args...)
#define STATUSINIT()
#else
extern int _status_node;
#define STATUSPRINT1(format,args...) { if (_status_node==0) DOPRINT(format,##args); }
#define STATUSPRINTN(format,args...) DOPRINT(format,##args)
#define STATUSINIT() {MPI_Comm_rank(MPI_COMM_WORLD,&_status_node);}
#endif

#ifdef NOWARNINGS
#define WARNRPINT(format,args...)
#else
#define WARNPRINT(format,args...) DOPRINT("%s"format,"WARNING: ",##args)
#endif

#ifdef NOASSERTS
#define DBGASSERT(cnd,format,args...)
#else
#define DBGASSERT(cnd,format,args...) { if (!(cnd)) { DOPRINT("ASSERTION VIOLATED (%s/%i): "format,   \
							   __FILE__,__LINE__,##args); exit(1); } }
#endif


#define DBGLEVEL1  0x0001  /* entry and exit prints */
#define DBGLEVEL2  0x0002  /* module load and instantiation */
#define DBGLEVEL3  0x0004  /* entry and exit of layers */
#define DBGLEVEL4  0x0008  /* arguments and parse information */

#define DBGPRINT1(format,args...) DBGPRINT(DBGLEVEL1,format,##args)
#define DBGPRINT2(format,args...) DBGPRINT(DBGLEVEL2,format,##args)
#define DBGPRINT3(format,args...) DBGPRINT(DBGLEVEL3,format,##args)
#define DBGPRINT4(format,args...) DBGPRINT(DBGLEVEL4,format,##args)

#ifdef DBGLEVEL

extern int _dbg_node;
extern int _dbg_cur_level;
extern int _dbg_cur_node;
#define DBGPRINT(level,format,args...) \
         { if ((level&_dbg_cur_level) && ((_dbg_cur_node==_dbg_node)||(_dbg_node<0)||(_dbg_cur_node<0))) \
              DOPRINT("L%04x "format,_dbg_cur_level,##args); }
#define DBGLATEINIT()                       \
{                                           \
  MPI_Comm_rank(MPI_COMM_WORLD,&_dbg_node); \
}
#define DBGEARLYINIT()                      \
{                                           \
  char *dbglevel,*dbgnode;                  \
  dbglevel=getenv("DBGLEVEL");              \
  if (dbglevel!=NULL)                       \
    _dbg_cur_level=strtol(dbglevel,NULL,0); \
  dbgnode=getenv("DBGNODE");                \
  if (dbgnode!=NULL)                        \
    _dbg_cur_node=strtol(dbgnode,NULL,0);   \
}
#else

#define DBGPRINT(level,format,args...)
#define DBGEARLYNIT()
#define DBGLATEINIT()
#endif

#define DOPRINT(format,args...)                       \
 {                                                    \
   if (_dbg_node>-2)                                  \
     printf("%5i: "format"\n",_dbg_node,## args);     \
   else                                               \
     printf(format"\n",## args);                      \
   fflush(stdout);                                    \
 }


/*-----------------------------------------------------------------*/
/* Not GNU */

#else /* i.e. COMP!=GNU */

#ifdef NOSTATUS
#define STATUSPRINT1 dbg_statusprint1
#define STATUSPRINTN dbg_statusprintn
#define STATUSINIT()
#else
extern int _status_node;
#define STATUSPRINT1 dbg_statusprint1
#define STATUSPRINTN dbg_statusprintn
#define STATUSINIT() {MPI_Comm_rank(MPI_COMM_WORLD,&_status_node);}
#endif

#ifdef NOWARNINGS
#define WARNPRINT dbg_warning
#else
#define WARNPRINT dbg_warning
#endif

#ifdef NOASSERTS
#define DBGASSERT dbg_assert
#else
#define DBGASSERT dbg_assert
#endif


#define DBGLEVEL1  0x0001  /* entry and exit prints */
#define DBGLEVEL2  0x0002  /* module load and instantiation */
#define DBGLEVEL3  0x0004  /* entry and exit of layers */
#define DBGLEVEL4  0x0008

#define DBGPRINT1 dbg_debugprint1
#define DBGPRINT2 dbg_debugprint2
#define DBGPRINT3 dbg_debugprint3
#define DBGPRINT4 dbg_debugprint4

#ifdef DBGLEVEL
extern int _dbg_node;
extern int _dbg_cur_level;
extern int _dbg_cur_node;

#define DBGLATEINIT()                       \
{                                           \
  MPI_Comm_rank(MPI_COMM_WORLD,&_dbg_node); \
}
#define DBGEARLYINIT()                      \
{                                           \
  char *dbglevel,*dbgnode;                  \
  dbglevel=getenv("DBGLEVEL");              \
  if (dbglevel!=NULL)                       \
    _dbg_cur_level=strtol(dbglevel,NULL,0); \
  dbgnode=getenv("DBGNODE");                \
  if (dbgnode!=NULL)                        \
    _dbg_cur_node=strtol(dbgnode,NULL,0);   \
}
#else /* ifdef DBGLEVEL */
#define DBGLATEINIT()
#define DBGEARLYINIT()
#endif


#endif /* else of COMP==GNU */

