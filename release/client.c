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
#include "comm.h"

/* -------------------------Main function for the client ----------------------*/
void main(int argc, char * argv[]) {
	int pipe_user_reading_from_server[2], pipe_user_writing_to_server[2];
//	pipe(pipe_user_reading_from_server);
//	pipe(pipe_user_writing_to_server);
	char buf[MAX_MSG];
	char rx_buf[MAX_MSG]; //Received from the child process
	char tx_buf[MAX_MSG]; //To write to the child process
	char* username = argv[1];

	// You will need to get user name as a parameter, argv[1].

	if(connect_to_server("YOUR_UNIQUE_ID", username, pipe_user_reading_from_server,pipe_user_writing_to_server) == -1) {
		exit(-1);
	}
	else {
		printf("Successfully connected to server!\n");
		print_prompt(username);
	}
	/* -------------- YOUR CODE STARTS HERE -----------------------------------*/
//	print_prompt(username);
	fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //This line makes stdin non-blocking
//	fcntl(pipe_user_reading_from_server[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
	while(1) {


		// Poll stdin (input from the terminal) and send it to server (child process) via pipe
//		print_prompt(username);
		if(read(STDIN_FILENO, rx_buf, MAX_MSG) > 0) {
			printf("READ DATA\n");
			close(pipe_user_writing_to_server[0]);
			write(pipe_user_writing_to_server[1], rx_buf, MAX_MSG);
//		printf("wrote to the pipe: %s\n", message);
			open(pipe_user_writing_to_server[0], O_RDONLY);
			print_prompt(username);

			// poll pipe retrieved and print it to sdiout - Retrieve information from the child process
		}
//		close(pipe_user_reading_from_server[1]);
		if(read(pipe_user_reading_from_server[0], tx_buf, MAX_MSG) > 0) {
			if (tx_buf != "\0") {
				printf("Client received from the server: %s\n", tx_buf);
			}
			open(pipe_user_reading_from_server[1], O_WRONLY);
		}
		

	}

	/* -------------- YOUR CODE ENDS HERE -----------------------------------*/
}

/*--------------------------End of main for the client --------------------------*/