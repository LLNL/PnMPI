#include <mpi.h>

#ifndef _PNMPI_MOD_TIMELAPSE
#define _PNMPI_MOD_TIMELAPSE

/*------------------------------------------------------------*/
/* Note: this module must be loaded into the stack before
   any module using it (i.e., must be instantiated between
   any user and the application). Also, any call to its
   services must occur after calling PMPI_Init and services
   are only available to modules requesting it during
   MPI_Init */

/*------------------------------------------------------------*/
/* Additional storage in requests */

#define PNMPI_MODULE_TIMELAPSE "timelapse"


/*..........................................................*/
/* Start at stop timer */

typedef int (*PNMPIMOD_Timelapse_Switch_t)(int);
int PNMPIMOD_Timelapse_Switch(int onoff);


#endif /* _PNMPI_MOD_STATUS */
