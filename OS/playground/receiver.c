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
int x,y; //Bogus ints

struct msg_contents_t{
	long mtype; //This is where we will store the pid of a process
	char data[50]; // This is where we will put data.
	pid_t pid; //This is the pid of the process we are getting it from for simplicity
} msg_contents_t;

int buffer;

int main(){
	struct msg_contents_t msg, othermsg;
	int mid;
	key_t key = ftok("sender.c", 100);
	printf("%s", msg.data);
	if((mid = msgget (key, 0666 | IPC_CREAT)) == -1){
		perror("msgget failed\n");
		return -1;
	};
// read msgs with tag 15 and 20
// will block if such messages are not there
	int ret;
	if((ret = msgrcv (mid, &msg, sizeof (msg_contents_t), 20, 0))==-1){
		perror("ret is bad");
	};
	printf("%s\n", msg.data);
	if((ret= msgrcv (mid, &othermsg, sizeof (msg_contents_t), 15, 0))==-1){
		perror("ret is bad");
	}
	printf("%s\n", othermsg.data);
	printf("%d\n", othermsg.pid);
	int pid = othermsg.pid;
	kill(pid, SIGINT);
	msgctl(mid,  IPC_RMID, NULL);
	return 0;
}
