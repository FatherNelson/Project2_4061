/******************************************************************************/
/* "HINT" -- Hierarchical INTegration.                                        */
/* Copyright (C) 1994 by Iowa State University Research Foundation, Inc.      */
/*                                                                            */
/* Files needed for use:                                                      */
/*     * hint.c             ---- Driver source                                */
/*     * hkernel.c          ---- Kernel source                                */
/*     * hint.h             ---- General include file                         */
/*     * typedefs.h         ---- Include file for DSIZE and ISIZE             */
/*     * README             ---- These are the rules. Follow them!!!          */
/******************************************************************************/
#define DSIZE double
#define ISIZE int
#include       <stdio.h>
#include       <stdlib.h>
#include       <math.h>
#include       <time.h>

/******************************************************************************/
/*   Vendor Specific defines and includes                                     */
/******************************************************************************/

#if defined(NCUBE2S)       /* Correction for 25 MHz clock on Ncube 2s         */
#define NTIME 0.8
#else
#define NTIME 1.0
#endif

#if defined(MP1) || defined(MP2)
#include       <mpl.h>     /* MasPar specific includes                        */
#include       <mp_time.h> /* MasPar timer prototypes and data declarations   */
#else
#include       <sys/time.h>/* Unix timer prototypes and data declarations     */
#endif

#if defined(SGI)           /* SGI specific includes for shared memory support.*/
#include       <ulocks.h>  
#include       <task.h>
#endif

#if defined(KSR)           /* Kendall Square Research includes for shared mem */
#include       <ksr/numarg.h> 
#endif

#if defined(SUNMOS) || defined(PARAGON)
#include       <nx.h>      /* Intel Paragon specific includes                 */
#define USE_DCLOCK         /* Use the Intel timer routine.                    */
#endif

#ifdef FORTRAN             /* SGI Fortran compiler forces trailing underscore */
#define Hint hint_
#endif


/******************************************************************************/
/*      Macro Declarations                                                    */
/******************************************************************************/
#define MIN(x,y) (((x) > (y))? (y): (x))
#define MAX(x,y) (((x) > (y))? (x): (y))

/******************************************************************************/
/*      Adjustable Defines                                                    */
/*      These may be adjusted according to the HINT rules.                    */
/******************************************************************************/
#define ADVANCE    1.2589  /* Multiplier. We use roughly 1 decibel step size. */
                           /* Closer to 1.0 takes longer to run, but might    */
                           /* produce slightly higher net QUIPS.              */
#define NCHUNK     4       /* Number of chunks for scatter decomposition      */
                           /* Larger numbers increase time to first result    */
                           /* (latency) but sample domain more evenly.        */
#define NSAMP      200     /* Maximum number of QUIPS measurements            */
                           /* Increase if needed, e.g. if ADVANCE is smaller  */
#define NTRIAL     3       /* Normal number of times to run a trial           */
                           /* Increase if computer is prone to interruption   */
#define PATIENCE   999     /* Number of times to rerun a bogus trial          */
#define RUNTM      0.5     /* Target time, seconds. Reduce for high-res timer.*/
                           /* Should be much larger than timer resolution.    */
#define STOPRT     0.1     /* Ratio of current to peak QUIPS to stop at       */
                           /* Smaller numbers will beat on virtual memory.    */
#define STOPTM     100     /* Longest time acceptable, seconds.  Most systems */
                           /* run out of decent-speed memory well before this */

/******************************************************************************/
/*      Non - Adjustable Defines                                              */
/******************************************************************************/
#define HI         0       /* Index of high values                            */
#define LO         1       /* Index of low values                             */
#define TRUE       (1==1)  /* Self explanatory                                */
#define FALSE      (1==0)  /* Self explanatory                                */
#define MSIZE      int     /* Integral type for allocating memory             */
#define NMIN       2       /* Minimum number of subintervals                  */

/******************************************************************************/
/*      Type Defines                                                          */
/******************************************************************************/
#include "typedefs.h"      /* Sets data type DSIZE and index type ISIZE       */

/******************************************************************************/
/*      Type Declarations                                                     */
/******************************************************************************/
typedef struct {
    DSIZE   ahi,           /* Upper bound on rectangle areas                  */
            alo,           /* Lower bound on rectangle areas                  */
            dx,            /* Interval widths                                 */
            flh,           /* Function values of left coordinates, high       */
            fll,           /* Function values of left coordinates, low        */
            frh,           /* Function values of right coordinates, high      */
            frl,           /* Function values of right coordinates, low       */
            xl,            /* Left x-coordinates of subintervals              */
            xr;            /* Right x-coordinates of subintervals             */
    }RECT;

typedef struct {
    double  t,             /* Time for a given run                            */
            qp;            /* QUIPS for a given run                           */
    ISIZE   n;             /* Subintervals for a given run                    */
    }Speed;

typedef ISIZE ERROR;


/******************************************************************************/
/*      Prototypes                                                            */
/******************************************************************************/
double  When();

double
Run(int laps,
    DSIZE *gamut, DSIZE scx, DSIZE scy, DSIZE dmax, ISIZE memry, ERROR *eflag);

#if defined(MP1) || defined(MP2)
DSIZE
Hint(DSIZE *scx, DSIZE *scy, DSIZE *xmax, ISIZE *mcnt, plural RECT *rect,
                          plural DSIZE *errs, plural ISIZE *ixes, ERROR *eflag);

#elif defined(SGI) || defined(KSR) || defined(CONVEX)
void
Hint(DSIZE scx, DSIZE scy, ISIZE mcnt, int iproc, double gs[2],
               RECT *rect, DSIZE *errs, ISIZE *ixes, ERROR *eflag);

#else
DSIZE
Hint(DSIZE *scx, DSIZE *scy, DSIZE *dmax, ISIZE *mcnt, RECT *rect,
                           DSIZE *errs, ISIZE *ixes, ERROR *eflag);
#endif



