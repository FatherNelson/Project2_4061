//
// Created by Grant Udstrand on 2019-09-22.
//

//
// Created by Grant Udstrand on 2019-09-22.
//

//#include "problem_functions.h"

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void test(){
	long unsigned int max = 250000000;
	double* new_arr = new double [max];
	for (int i = 0; i < max; i++){
		new_arr[i] = new_arr[i] + new_arr[i];
	}
}


/* Example of using an inteval timer on most Unix machines.
									*/


/* declare some stuff for the timer.					*/
struct timeval start_time, end_time;

int timer ()
{

	/* Declare some values to be used with the timer.           */
	double start_count, end_count;
	double elapsed_time;        /* measured time in microseconds        */

	/* Declare your other variables used in the program.                */

	FILE* fp;
	fp = fopen("/Users/Grant/CLionProjects/EE5371/HW1/hw1d.txt", "ab+");
	if(fp == NULL){
		perror("No such file");
	}
	/* Start the timer by reading its current value.            */
	gettimeofday(&start_time,NULL);

	/* Stuff to be measured */

	printf("got here\n");

	test();

	printf("got here\n");

	/* Measure the elapsed time by subtracting the start value
	   from the current value.                                  */
	gettimeofday(&end_time, NULL);
	start_count = (double) start_time.tv_sec
	              + 1.e-6 * (double) start_time.tv_usec;
	end_count = (double) end_time.tv_sec +
	            1.e-6 * (double) end_time.tv_usec;
	elapsed_time = (end_count - start_count);
	if(fp != NULL) {
		fprintf(fp, "%f\n", elapsed_time);
	}
	fclose(fp);
}


