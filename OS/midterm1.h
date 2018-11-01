//
// Created by Grant Udstrand on 10/18/18.
//

#ifndef OS_MIDTERM1_H
#define OS_MIDTERM1_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MSGSIZE 100
void midterm(){
	int i = 0;
	int k = 5;
	int p_c[2];
	pipe(p_c);
	int pid = fork();
	if(pid == 0){
		//Do nothing
//		wait(NULL);
		close(p_c[1]);
		int buf[MSGSIZE];
		read(p_c[0], buf,1);
		printf("%d\n", buf[0]);
	}
	else{
		while(i < k){
			close(p_c[0]);
			int pid = fork();
			if(pid == 0) {
				//If child
				write(p_c[1], (void *) getpid(), 5);
				printf("%d\n", getpid());
				exit(0);
			}
			i++;
		}
	}
}
#endif //OS_MIDTERM1_H
