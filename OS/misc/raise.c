//
// Created by Grant Udstrand on 11/26/18.
//
#include <signal.h>
#include <stdlib.h>

long fact;
long prev_fact;
long i;

void SIGhandler (int sig) {
	printf("\n OVERFLOW. Closest answer is %ld! = %ld\n", i-1, prev_fact);
	exit(0);
}
void main ()
{
	printf("Factorial Computation:\n\n");
	signal(SIGUSR1, SIGhandler); /* install SIGUSR1 handler */
	for (prev_fact = i = 1; ; i++, prev_fact = fact) {
		fact = prev_fact * i; /* computing factorial */
		if (fact < 0) /* if the results wraps around */
		raise(SIGUSR1); /* we have overflow, print it */
		else if (i % 3 == 0) /* otherwise, print the value */
		printf(" %ld! = %ld\n", i, fact);
	}
}