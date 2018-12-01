//
// Created by Grant Udstrand on 11/27/18.
//

//
// Created by Grant Udstrand on 11/27/18.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <signal.h>


int x,y; //Bogus ints
int handler = 0;

pthread_mutex_t thread = PTHREAD_MUTEX_INITIALIZER;
struct mymsg_t {
	long mtype; // used for tag selection
	char data[50];
	pid_t pid;
// just bunch of contig. bytes
} msg;

void handler(){
	printf("Handler was called. \n");

	handler = 1;
}

int buffer;// identifies the mailbox from the child to the parent.

int main(){
	struct mymsg_t m1 = {15, "hello", getpid()};
	struct mymsg_t m2 = {20, "goodbye", getpid()};
	sigset_t actionset; // Create a new signal set.
	sigfillset(&actionset);
	struct sigaction act; //Create a new signal action
	act.sa_handler = handler;
	act.sa_flags = NULL;
	act.sa_mask = actionset;
	sigaction(SIGINT, &act, NULL);
	int mid;
	key_t key = ftok("sender.c", 100);
	printf("%d\n", key);
	if((mid = msgget (key, 0666 | IPC_CREAT)) == -1){
		perror("msgget failed\n");
		return -1;
	};

	msgsnd (mid, (void *)&m1, sizeof (msg), 0);
	msgsnd (mid, (void *)&m2, sizeof (msg), 0);
	while(1){
		//Keep process alive so we can observe the signal reception;
	}
}

