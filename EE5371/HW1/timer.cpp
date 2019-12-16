
/* Example of using an inteval timer on most Unix machines.
									*/
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "problem_functions.h"


/* declare some stuff for the timer.					*/
struct timeval start_time, end_time;

/* Debug on/off */
int DEBUG = 0;

int timer ()
{

	/* Declare some values to be used with the timer.           */
	double start_count, end_count;
	double elapsed_time;        /* measured time in microseconds        */

	/* Declare your other variables used in the program.                */

	long int n = 5000000;
	int max_stride = 500;
	char* A = (char*)malloc(n*8);
	char* B = (char*)malloc(n*8);

	FILE* fp;
	fp = fopen("/Users/Grant/CLionProjects/EE5371/HW1/hw1b_5M2.txt", "ab+");
	if(fp == NULL){
		perror("No such file");
	}
	/* Start the timer by reading its current value.            */
	gettimeofday(&start_time,NULL);

	/* Stuff to be measured */


	/* Measure the elapsed time by subtracting the start value
	   from the current value.                                  */
	gettimeofday(&end_time, NULL);
	start_count = (double) start_time.tv_sec
	              + 1.e-6 * (double) start_time.tv_usec;
	end_count = (double) end_time.tv_sec +
	            1.e-6 * (double) end_time.tv_usec;
	elapsed_time = (end_count - start_count);
	if(DEBUG) {
//		printf("value at start:%d stride: %d \n", A[0], stride);
		printf("The total elapsed time is:  %f seconds\n", elapsed_time);
	}
	if(!DEBUG){
		if(fp != NULL) {
			fprintf(fp, "%f\n", elapsed_time);
		}
		else{
			printf("Failed to open file\n");
		}
	}
	end(fp, A, B);
}


