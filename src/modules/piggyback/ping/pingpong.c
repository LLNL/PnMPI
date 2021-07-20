/* pingpong - measure effective bandwidth and latency */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "mpi.h"
#include "getus.h"

#define MAXSIZE (512*1024)
#define MINSIZE (1)
#define INCSIZE (2)
#define INCOP   *=
#define REPEAT  2000

/*#define MAXSIZE (17*1024)
#define MINSIZE (16156)
#define INCSIZE (4)
#define INCOP   +=
#define REPEAT  10*/

/* #define MAXSIZE (1024)
   #define MINSIZE (32)
   #define INCSIZE (1)
   #define INCOP   +=
   #define REPEAT  1000
*/

#define CALIBRATION_LOOPS 100

#define TAG_PING 1
#define TAG_PONG 2

/* define DETAIL if you want to create histogramms by measuring
   the latency of each single ping-pong transfer */
#ifndef PSC
/*#define DETAIL*/
#endif

#ifdef PSC
#define linux
#endif

#if defined(linux) || defined(__GNUC__)
#define longlong_t long long
#endif

char *buffer;
char *exename;
int min_size, max_size, inc_size, repeats;
int myrank, mysize;
static FILE* fpGlobal = NULL;
static FILE* fpDetail = NULL;
struct timeval mytime;


void ping (int to, int from);
void pong (int to);

void ping_wc (int to, int from);
void pong_wc (int to);

#ifdef _WIN32
void gettime(long long *us) {
    /* fixme: insert a real implementation */
    *us = -1;
}
#else
void gettime(long long *us) {
        gettimeofday(&mytime, NULL); \
        *us =  ((mytime.tv_sec) * 10000000 + (mytime.tv_usec) * 10);
}
#endif


int main(int argc, char **argv) {
    int first;
    char fname[128];
    char hname[256];
    
    gethostname(hname,256);
    printf("RUNNING ON HOST %s\n\n",hname);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &mysize);
    
    if (myrank == 0) {
	strcpy(fname, argv[0]);
	strcat(fname,".global");
	fpGlobal = fopen(fname,"w");	
    }
    exename = argv[0];
    
    if(mysize % 2 != 0) {
	printf ("pingpong must be used with an even number of processes.\n");
	MPI_Finalize();	
	exit (1);
    }

    /* set run parameters */
    if (argc != 4) {
	printf ("usage: pingpong min_size max_size repeats\n");
	printf ("using default values for this run\n");
	
	min_size = MINSIZE;
	max_size = MAXSIZE;
	inc_size = INCSIZE;
	repeats  = REPEAT;
    } else {
	min_size = atoi( argv[1] );
	max_size = atoi( argv[2] );
	inc_size = INCSIZE;
	repeats  = atoi( argv[3] );
    }
    buffer = (char *)malloc (max_size);
    
    /* find ping and pong processes */
    if ( myrank < mysize/2 ) {
	if (myrank % 2 == 0)
	    ping(  myrank + mysize/2, myrank);
	else
	    pong( myrank + mysize/2 );
    } else {
	first = (mysize/2) % 2;
	if (myrank % 2 == first)
	    pong( myrank - mysize/2 );
	else 
	    ping( myrank - mysize/2, myrank );
    }

    /* find ping and pong processes */
    if ( myrank < mysize/2 ) {
	if (myrank % 2 == 0)
	    ping_wc(  myrank + mysize/2, myrank);
	else
	    pong_wc( myrank + mysize/2 );
    } else {
	first = (mysize/2) % 2;
	if (myrank % 2 == first)
	    pong_wc( myrank - mysize/2 );
	else 
	    ping_wc( myrank - mysize/2, myrank );
    }
  
    if (myrank == 0) {
	fclose(fpGlobal);
    }

    free (buffer);
    MPI_Finalize();

    return 0;
}


void ping_wc( int to, int from ) {

    MPI_Status status;

    double starttime, totaltime;
    double getticks_overhead;

#ifdef DETAIL
    longlong_t hr_start, hr_end;
    longlong_t calibration = 0;
    longlong_t *timings;
#endif

    char fname[128];
    char bytes[128];
    int i, j;
    int firstrun = 1;
    

#ifdef DETAIL
    fprintf (stderr, "Calibrating...");
    for (i = 0; i < CALIBRATION_LOOPS; i++) {
	GETTICKS(&hr_start);
	GETTICKS(&hr_end);
	calibration += hr_end - hr_start;
    }
    getticks_overhead = ((double)calibration)/(CALIBRATION_LOOPS);
    fprintf (stderr, "gethrtime() overhead is %6.3f\n", getticks_overhead);

    timings = (longlong_t *)malloc (repeats*sizeof(longlong_t));
#endif

    printf("pingpong from %d to %d\n\n", from, to);
    fprintf(fpGlobal, "# msgsize[byte]  repeats  bandwidth[MB/s]  latency[us]\n");
    fflush(stdout);
  
    for( i = min_size; i <= max_size; i INCOP inc_size) {
	if ((!firstrun) && (i == 0)) {
	    i++;
	    if (i > max_size)
		break;
	}
	firstrun = 0;

	MPI_Barrier(MPI_COMM_WORLD);

#ifdef DETAIL
	strcpy (fname, exename);
	sprintf (bytes, "_id%d_%db.detail", myrank, i);
	strcat (fname, bytes);
	fpDetail = fopen (fname,"w");
	fprintf (fpDetail, "# ticks for ping-pong, msgsize = %d byte\n# getticks_overhead = %6.3f \n", 
		i, getticks_overhead);
#endif

	starttime = MPI_Wtime();
	for( j = 0; j < repeats; j++) {
#ifdef DETAIL
	    GETTICKS(&hr_start);
#endif
	    MPI_Send( buffer, i, MPI_CHAR, to, TAG_PING, MPI_COMM_WORLD);
	    MPI_Recv( buffer, i, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
#ifdef DETAIL
	    GETTICKS(&hr_end);
	    timings[j] = hr_end - hr_start;
#endif
	}
	totaltime = MPI_Wtime() - starttime;

	printf("RESULTS WILDCARD size = %7d,  bandwidth = %7.3f MB/s latency = %7.3f usec.,\n",
	       i, ((double)(2*i*repeats))/(totaltime*1024*1024), (totaltime*1e+6)/(repeats*2));
	fprintf(fpGlobal,"%7d \t %7d \t %7.3f \t %7.3f\n", i, repeats,
		((double)(2*i*repeats)/(totaltime*1024*1024)), (totaltime*1e+6)/(repeats*2));
	fflush(stdout); fflush(fpGlobal); 
#ifdef DETAIL
	for( j = 0; j < repeats; j++) {
	    fprintf (fpDetail, "%d \t %6.3f \n", j, (double)timings[j]);
	}
	fclose(fpDetail);
#endif
    }
} 


void pong_wc( int to ) {
    MPI_Status status;
    int i, j;
    int firstrun = 1;

    for( i = min_size; i <= max_size; i INCOP inc_size) {
	if ((!firstrun) && (i == 0)) {
	    i++;
	    if (i > max_size)
		break;
	}
	firstrun = 0;
	MPI_Barrier(MPI_COMM_WORLD);
      
	for(j = 0; j < repeats; j++) {
	    MPI_Recv( buffer, i, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	    MPI_Send( buffer, i, MPI_CHAR, to, TAG_PONG, MPI_COMM_WORLD);
	}
    }  
}

void ping( int to, int from ) {

    MPI_Status status;

    double starttime, totaltime;
    double getticks_overhead;

#ifdef DETAIL
    longlong_t hr_start, hr_end;
    longlong_t calibration = 0;
    longlong_t *timings;
#endif

    char fname[128];
    char bytes[128];
    int i, j;
    int firstrun = 1;
    

#ifdef DETAIL
    fprintf (stderr, "Calibrating...");
    for (i = 0; i < CALIBRATION_LOOPS; i++) {
	GETTICKS(&hr_start);
	GETTICKS(&hr_end);
	calibration += hr_end - hr_start;
    }
    getticks_overhead = ((double)calibration)/(CALIBRATION_LOOPS);
    fprintf (stderr, "gethrtime() overhead is %6.3f\n", getticks_overhead);

    timings = (longlong_t *)malloc (repeats*sizeof(longlong_t));
#endif

    printf("pingpong from %d to %d\n\n", from, to);
    fprintf(fpGlobal, "# msgsize[byte]  repeats  bandwidth[MB/s]  latency[us]\n");
    fflush(stdout);
  
    for( i = min_size; i <= max_size; i INCOP inc_size) {
	if ((!firstrun) && (i == 0)) {
	    i++;
	    if (i > max_size)
		break;
	}
	firstrun = 0;

	MPI_Barrier(MPI_COMM_WORLD);

#ifdef DETAIL
	strcpy (fname, exename);
	sprintf (bytes, "_id%d_%db.detail", myrank, i);
	strcat (fname, bytes);
	fpDetail = fopen (fname,"w");
	fprintf (fpDetail, "# ticks for ping-pong, msgsize = %d byte\n# getticks_overhead = %6.3f \n", 
		i, getticks_overhead);
#endif

	starttime = MPI_Wtime();
	for( j = 0; j < repeats; j++) {
#ifdef DETAIL
	    GETTICKS(&hr_start);
#endif
	    MPI_Send( buffer, i, MPI_CHAR, to, TAG_PING, MPI_COMM_WORLD);
	    MPI_Recv( buffer, i, MPI_CHAR, to, TAG_PONG, MPI_COMM_WORLD, &status);
#ifdef DETAIL
	    GETTICKS(&hr_end);
	    timings[j] = hr_end - hr_start;
#endif
	}
	totaltime = MPI_Wtime() - starttime;

	printf("RESULTS NOWILDCARD size = %7d,  bandwidth = %7.3f MB/s latency = %7.3f usec.,\n",
	       i, ((double)(2*i*repeats))/(totaltime*1024*1024), (totaltime*1e+6)/(repeats*2));
	fprintf(fpGlobal,"%7d \t %7d \t %7.3f \t %7.3f\n", i, repeats,
		((double)(2*i*repeats)/(totaltime*1024*1024)), (totaltime*1e+6)/(repeats*2));
	fflush(stdout); fflush(fpGlobal); 
#ifdef DETAIL
	for( j = 0; j < repeats; j++) {
	    fprintf (fpDetail, "%d \t %6.3f \n", j, (double)timings[j]);
	}
	fclose(fpDetail);
#endif
    }
} 


void pong( int to ) {
    MPI_Status status;
    int i, j;
    int firstrun = 1;

    for( i = min_size; i <= max_size; i INCOP inc_size) {
	if ((!firstrun) && (i == 0)) {
	    i++;
	    if (i > max_size)
		break;
	}
	firstrun = 0;
	MPI_Barrier(MPI_COMM_WORLD);
      
	for(j = 0; j < repeats; j++) {
	    MPI_Recv( buffer, i, MPI_CHAR, to, TAG_PING, MPI_COMM_WORLD, &status);
	    MPI_Send( buffer, i, MPI_CHAR, to, TAG_PONG, MPI_COMM_WORLD);
	}
    }  
}
