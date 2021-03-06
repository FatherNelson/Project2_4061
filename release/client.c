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
	printf("Connecting to server...\n");
	int pipe_user_reading_from_server[2], pipe_user_writing_to_server[2];
	char buf[MAX_MSG];

	char* username = argv[1];
	if(connect_to_server("Project2_59", username, pipe_user_reading_from_server,pipe_user_writing_to_server) == -1) {
		exit(-1);
	}
	else {
		printf("Successfully connected to server!\n");
		print_prompt(username);
	}
	/* -------------- YOUR CODE STARTS HERE -----------------------------------*/
	fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //This line makes stdin non-blocking
	fcntl(pipe_user_reading_from_server[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
	while(1) {
		char rx_buf[MAX_MSG] = {"\0"}; //Received from the child process
		char tx_buf[MAX_MSG] = {"\0"}; //To write to the child process
		usleep(SLEEP_TIME); //Makes polling less taxing on the hardware. Humans write slowly, so this will not be a huge problem
		
		// Poll stdin (input from the terminal) and send it to server (child process) via pipe
		if(read(STDIN_FILENO, tx_buf, MAX_MSG) > 0) {
			write(pipe_user_writing_to_server[1], tx_buf, MAX_MSG);
			print_prompt(username);
		}
		// poll pipe retrieved and print it to sdiout - Retrieve information from the child process
		if(read(pipe_user_reading_from_server[0], rx_buf, MAX_MSG) > 0) {
			close(pipe_user_reading_from_server[1]);
		 	if (rx_buf != "\0") {
		 	if (strcmp(rx_buf,"User already exists\n") == 0) {
					printf("%s", rx_buf); //If the same username is entered as another user, exit the client
		 			exit(0);
				}
				printf("%s", rx_buf);
				print_prompt(username);
			}
			open(pipe_user_reading_from_server[1], O_WRONLY);
		}



	}

	/* -------------- YOUR CODE ENDS HERE -----------------------------------*/
}

/*--------------------------End of main for the client --------------------------*/
