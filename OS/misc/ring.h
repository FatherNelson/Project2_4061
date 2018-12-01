//
// Created by Grant Udstrand on 10/15/18.
//

#ifndef OS_RING_H
#define OS_RING_H
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MSGSIZE 100

void ring(){
	int i = 0;
	while(i <5) {
		int parent_child[2];
		int child_parent[2];
		pipe(parent_child);
		pipe(child_parent);
		pid_t pid = fork();
		if (pid < 0) {
			perror("Failed to fork\n");
		} else if (pid == 0) {
			char buf[MSGSIZE];
			close(parent_child[1]);
			close(child_parent[0]);
			write(child_parent[1], "message from child", MSGSIZE);
			read(parent_child[0], buf, MSGSIZE);
			printf("I am the child process %d %s\n", (int) getpid(), buf);
		} else {
			char buf[MSGSIZE];
			close(parent_child[0]);
			close(child_parent[1]);
			read(child_parent[0], buf, MSGSIZE);
			write(parent_child[1], "message from parent", MSGSIZE);
			printf("I am the parent process %d %s\n", (int) getpid(), buf);
		}
		i++;
	}
}
#endif //OS_RING_H
