//
// Created by Grant Udstrand on 2018-12-04.
//
#include <sys/socket.h>

int main(int argc, char* argv[]){
	struct sockaddr socket_info;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(bind(sockfd,&socket_info, sizeof(socket_info)) < 0){
		perror("Failed to bind to a socket.");
	}
	while(1) {
		listen(sockfd, 2);
		int new_socket = accept(sockfd, &socket_info, sizeof(socket_info));
	}
	printf("I ran correctly\n");
}