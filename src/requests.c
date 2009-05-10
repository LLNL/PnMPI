#include "services.h"

/*------------------------------------------------------------*/
/* Declarations */

/*..........................................................*/
/* Size of additional storage across all modules */

int pnmpi_req_storage_size = -1;


/*------------------------------------------------------------*/
/* Public Interface */

/*..........................................................*/
/* Request additional memory in the request object
   This must be called from within MPI_Init

   IN:  size = number of bytes requested
               (if 0, just storage of standard parameters
   OUT: >=0: offset of new storage relative to request pointer
         <0: error message 
*/

/* No need to be threadsafe here, since there can only be
   one activation of MPI_Init anyway */

int PNMPI_Service_RequestStorage(int size)
{
  int ret;

  if (pnmpi_init_running)
    {
      if (pnmpi_req_storage<0)
	{
	  /* first request, activate it */
	  pnmpi_req_storage=0;
	}
      
      ret=sizeof(PNMPI_Request_Map)+pnmpi_req_storage;
      pnmpi_req_storage+=size;
      return ret;
    }
  else
    {
      /* can only be called from within MPI_Init */
      return PNMPI_ERROR_NOT_INIT;
    }
}


/*------------------------------------------------------------*/
/* Internal Interface */


/*------------------------------------------------------------*/
/* The End. */
