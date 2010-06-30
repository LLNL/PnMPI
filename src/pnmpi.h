/* Public interface for PNMPI */
/* To be used by applications that are aware of PNMPI */

#include <mpi.h>

#include "pnmpimod.h"

#define PNMPI_PCONTROL_LEVEL 3

#define PNMPI_PCONTROL_SINGLE   0x0
#define PNMPI_PCONTROL_MULTIPLE 0x1
#define PNMPI_PCONTROL_VARG     0x0
#define PNMPI_PCONTROL_PTR      0x2

#define PNMPI_PCONTROL_PMPI   0
#define PNMPI_PCONTROL_OFF    1
#define PNMPI_PCONTROL_ON     2
#define PNMPI_PCONTROL_PNMPI  3
#define PNMPI_PCONTROL_MIXED  4
#define PNMPI_PCONTROL_INT    5
#define PNMPI_PCONTROL_TYPED  6

#define PNMPI_PCONTROL_DEFAULT PNMPI_PCONTROL_INT

#define PNMPI_PCONTROL_TYPE_INT 0
#define PNMPI_PCONTROL_TYPE_LONG 1
#define PNMPI_PCONTROL_TYPE_PTR 2
#define PNMPI_PCONTROL_TYPE_DOUBLE 3

#define PNMPI_ERROR -1
