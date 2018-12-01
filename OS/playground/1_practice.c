//
// Created by Grant Udstrand on 11/27/18.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
int x,y; //Bogus ints

struct my_msg_t{
	long mtype; //This is where we will store the pid of a process
	long data; // This is where we will put data.
} message;

int buffer;// identifies the mailbox from the child to the parent.

int main(){
	pid_t pid;
	pid = fork();
	if(pid == 0){ //If child
		printf("I am the child\n");
		struct my_msg_t child_to_parent; // Instantiate a message from child to parent.
		key_t key;
		int perms= 0644;
		buffer = msgget(key, perms | IPC_CREAT);
		child_to_parent.mtype = pid;
		child_to_parent.data = getpid();
		msgsnd(buffer, &child_to_parent, sizeof(message), 0);
		printf("Child sent message: %d\n", child_to_parent.data);
	}
	else{
		wait(NULL);
//		printf("I am the parent\n");
		struct my_msg_t parent_to_child; //Instantiate a message from parent to child.
		key_t key;
		int perms= 0644;
//		int buffer = msgget(pid, perms);
//		parent_to_child.mtype = pid;
//		parent_to_child.data = 101;
		struct my_msg_t rcv;
		msgrcv(buffer, &rcv, sizeof(message), 0,0);
		printf("I am the parent and I received %d\n", rcv.data);
	}
}
