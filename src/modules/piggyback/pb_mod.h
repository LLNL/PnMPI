#include <mpi.h>
#include <status.h>

#ifndef _PNMPI_MOD_PIGGYBACK
#define _PNMPI_MOD_PIGGYBACK

#define PNMPI_MODULE_PB "piggyback"


/*..........................................................*/
/* Request additional memory in the status object
   This must be called from within MPI_Init (after
   calling PMPI_Init)

   IN:  size = number of bytes requested
               (if 0, just storage of standard parameters
   OUT: >=0: offset of new storage relative to request pointer
         <0: error message 
*/


typedef int (*PNMPIMOD_Piggyback_t)(int,char*);
typedef int (*PNMPIMOD_Piggyback_Size_t)(int);

#endif /* _PNMPI_MOD_STATUS */
