//
// Created by Grant Udstrand on 11/26/18.
//
#include <signal.h>
#include <stdlib.h>
//volatile int found= 0;
//
//void child_handler(){
//	found++;
//	char* buf[8];
//	buf[0]= found;
//	write(1, found, 8);
//}
//void waitor(){
//	sigset_t set;
//	signal(SIGINT, child_handler);
//	while(found < 4){
//		pause();
//	}
//	printf("I am done loser\n");
//}

/// global declarations if any
int check;
void child_handler (int signo) {
	printf("Child Ended\n");
	check = 1;
}
void waitor () {
	// block until SIGCHLD is received
	while (check == 0){
		pause();
	}
// reset state for next wait
	check = 0;
}
// This function needs to be called from
// main before any fork call
// This needs to be done initially else
// we may miss a signal between fork and wait call
void init(){
	// set up signal handler for SIGCHLD
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_handler = child_handler;
	sigaction (SIGINT, &act, NULL);
}


int main(){
	init();
	waitor();
}