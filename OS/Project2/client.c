#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "util.h"
#include "comm.h"
#define BUFSIZE 100
/* -------------------------Main function for the client ----------------------*/
void main(int argc, char * argv[]) {
	char buf[BUFSIZE];
	int pipe_to_user[2], pipe_to_server[2];
	char *username = argv[1];
	print_prompt(username);


	pipe(pipe_to_server);
	pipe(pipe_to_user);


	if(connect_to_server("YOUR_UNIQUE_ID", username, pipe_to_user, pipe_to_server) == -1) {
		printf("Failed to connect");
		exit(-1);
	}
	print_prompt(username);

	/* -------------- YOUR CODE STARTS HERE -----------------------------------*/
	fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //This line makes the read call non-blocking
	while(1) {
		// You will need to get user name as a parameter, argv[1].
		char tmp_buf[BUFSIZE] = "\0";
		// poll pipe retrieved and print it to sdiout
//		close(pipe_to_server[1]); //Closes the input side of the pipe
//		read(pipe_to_server,tmp_buf,BUFSIZE);
//		if(strlen(tmp_buf) > 0){
//			printf("%s",tmp_buf);
//		}

		// Reads from the output side of the pipe

		// Poll stdin (input from the terminal) and send it to server (child process) via pipe
		if(read(STDIN_FILENO, tmp_buf, MAX_CMD_LENGTH) > 0) {
			read(STDIN_FILENO, tmp_buf, MAX_CMD_LENGTH);
			char *tmp = (char *) malloc(strlen(tmp_buf) - 1);
			strncpy(tmp, tmp_buf, strlen(tmp_buf) - 1);
			printf("%d \n", strlen(tmp));
			printf("tmp_buf has a value of %s. I am stored inside of the client process \n", tmp);
			/**This block should trim any excess char. Make sure to take care of excess malloc **/
			printf("%s \n", tmp);
			close(pipe_to_server[0]);
			write(pipe_to_server[1],tmp,BUFSIZE);
			print_prompt(username);
		}
	}
		
	/* -------------- YOUR CODE ENDS HERE -----------------------------------*/
}

/*--------------------------End of main for the client --------------------------*/


