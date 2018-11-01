//
// Created by Grant Udstrand on 10/15/18.
//

#ifndef OS_WEEK06_DEFINITIONS_H
#define OS_WEEK06_DEFINITIONS_H

#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MSGSIZE 50

int pipes(){
	int child_write[2]; // Pipe for child writing to the parent
	int parent_write[2]; //Pipe for parent writing to the child
	/**Create the Pipe**/
	if(pipe(child_write) == -1){
		perror("Pipe Error");
		exit(-1);
	}
	else{
		printf("Pipe was created\n");
	}
	if(pipe(parent_write) == -1){
		perror("Pipe Error");
		exit(-1);
	}
	else{
		printf("Pipe was created\n");
	}
	/** Fork the program**/
	pid_t pid = fork();
	if(pid == 0){
		close(child_write[0]);
		printf("I am the child process\n");
		write(child_write[1],"Hello, I am from the child process", MSGSIZE);
		exit(0); //0 indicates a normal return, -1 a failure.
	}
	else if(pid == -1){
		printf("I failed to fork");
		return 0;
	}
		/**Parent of the process**/
	else{
		int *stat_loc;  //Provide a pointer for the wait function
		wait(stat_loc); //The value of the status will be written to this stat_loc
		close(child_write[1]);
		char buf[MSGSIZE];
		read(child_write[0],buf,MSGSIZE);
		if(WIFEXITED(stat_loc)){
			printf("Exited normally with status code %d \n", WEXITSTATUS(stat_loc));
		}
		printf("I am the parent process\n");
		printf("I read |%s| from the pipe\n", buf);
	}
}
#endif //OS_WEEK06_DEFINITIONS_H
