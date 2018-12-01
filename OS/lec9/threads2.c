#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <fcntl.h>

pthread_t tid1, tid2; //Thread id's
int fd = 0; // A file descriptor

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *myThreadFun(void *vargp)
{
	sleep(1);
	printf("Printing GeeksQuiz from Thread \n");
//	pthread_cancel(tid1); //Can kill tid1 with this line.
	return NULL;
}
void *anotherFunction(int fd)
{
	printf("Received an fd of %d", fd);
	sleep(2);
	printf("Heya jack\n");
	char* buf[10000];
	if(read(fd, buf, sizeof(buf))==-1){
		perror("Failed on read\n");
	};
	printf("Got past read");
	if(write(0, buf, sizeof(buf))==-1){
		perror("Failed on write\n");
	};
	return 17;
}

int main()
{
	pthread_t thread_id;
	printf("Before Thread\n");
	int error = 0; // Possible error code
	int return1,return2;
	if(fd = open("1.txt", O_RDONLY)==-1){
		perror("Failed to open 1.txt");
	}
	printf("1.txt is at %d\n", fd);
	pthread_create(&thread_id, NULL, myThreadFun, NULL);
	pthread_create(&tid1, NULL, anotherFunction, fd);
	pthread_create(&tid2, NULL, myThreadFun, NULL);
	pthread_join(thread_id, NULL);
	pthread_join(tid1, return1);
	printf("After Thread\n");
	printf("Return 1 has a value of %d\n", return1);
	exit(0);
}//
// Created by Grant Udstrand on 11/26/18.
//

