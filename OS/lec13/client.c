#include <sys/socket.h>

int main(int argc, char* argv[]){
	struct sockaddr socket_info;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	connect(sockfd, &socket_info, sizeof(socket_info));
}