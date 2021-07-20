#ifndef __MPID_GETUS
#define __MPID_GETUS


#ifdef _WIN32
#include <windows.h>
#define GETTICKS(t) QueryPerformanceCounter((LARGE_INTEGER*)t);

#else
/* binding for an assembler subroutine to read processor clocks 
   on x86 CPUs via the rdtsc command */

/* CLOCK is the clock frequency of the processor - it has to be
   set according to the current configuration to the CPU frequency 
   in 1e+7 Hz, i.e. for 450 MHz the setting would be 45 */
#define CLOCK    45 

void gettime(long long *us);
/* this macro returns a number of CPU ticks */
#define GETTICKS(t) gettime(t)


/* Sun cc */
#ifdef __SUNPRO_C

/* get processor ticks */
void getticks(long long *us);
#pragma inline (getticks);

#else 
/* Gnu gcc */
#ifdef X86
#define GETTICKS(t) asm volatile ("push %%esi\n\t" "mov %0, %%esi" : : "r" (t)); \
                      asm volatile ("push %eax\n\t" "push %edx"); \
                      asm volatile ("rdtsc"); \
                      asm volatile ("movl %eax, (%esi)\n\t" "movl %edx, 4(%esi)"); \
                      asm volatile ("pop %edx\n\t" "pop %eax\n\t" "pop %esi"); 
#endif /* X86 */

#endif /* SUNPRO_C */

#define GETUS(t)    GETTICKS(t);

#ifdef X86
/* this macro returns a time stamp as a multiple of 100ns */
#define GETUS(ts)   GETTICKS(ts); \
                     *ts /= CLOCK;
#endif /* X86 */

#endif /* _WIN32 */

#endif /* __MPID_GETUS */
