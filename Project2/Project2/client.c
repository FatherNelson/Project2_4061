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

	// You will need to get user name as a parameter, argv[1].
	if(connect_to_server("YOUR_UNIQUE_ID", username, pipe_to_user, pipe_to_server) == -1) {
		printf("Failed to connect");
		exit(-1);
	}


	/* -------------- YOUR CODE STARTS HERE -----------------------------------*/
	while(1) {
		char tmp_buf[100];
		// poll pipe retrieved and print it to sdiout
		close(pipe_to_server[1]); //Closes the input side of the pipe
		read(pipe_to_server[0], tmp_buf, BUFSIZE); // Reads from the output side of the pipe
		// Poll stdin (input from the terminal) and send it to server (child process) via pipe
		printf("%s \n", tmp_buf);
	}
		
	/* -------------- YOUR CODE ENDS HERE -----------------------------------*/
}

/*--------------------------End of main for the client --------------------------*/


