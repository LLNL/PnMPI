#include <mpi.h>

#ifndef _PNMPI_MOD_STATUS
#define _PNMPI_MOD_STATUS

/*------------------------------------------------------------*/
/* Note: this module must be loaded into the stack before
   any module using it (i.e., must be instantiated between
   any user and the application). Also, any call to its
   services must occur after calling PMPI_Init and services
   are only available to modules requesting it during
   MPI_Init */

/*------------------------------------------------------------*/
/* Additional storage in requests */

#define PNMPI_MODULE_STATUS "status-storage"


/*..........................................................*/
/* Access macros */

#define STATUS_STORAGE(status,offset,type) STATUS_STORAGE_ARRAY(status,offset,0,type,1,0)

#define STATUS_STORAGE_ARRAY(status,offset,total,type,count,num) (*((type*) (((char*) (status))+count*sizeof(MPI_Status)+num*total+offset)))

#define PNMPIMOD_STATUS_TAG 0x3e1f9


/*..........................................................*/
/* Request additional memory in the status object
   This must be called from within MPI_Init (after
   calling PMPI_Init)

   IN:  size = number of bytes requested
               (if 0, just storage of standard parameters
   OUT: >=0: offset of new storage relative to request pointer
         <0: error message 
*/

typedef int (*PNMPIMOD_Status_RequestStorage_t)(int);
int PNMPIMOD_Status_RequestStorage(int size);


#endif /* _PNMPI_MOD_STATUS */
