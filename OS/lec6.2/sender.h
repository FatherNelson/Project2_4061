//
// Created by Grant Udstrand on 10/18/18.
//

#ifndef OS_SENDER_H
#define OS_SENDER_H
#include <string.h>
#include <sys/msg.h>
#define SOMESIZE 100

typedef char msg_contents_t[SOMESIZE];
struct  mymsg_t{
	long mtype; //Used for tag selection
	char* mtext; // Just a bunch of contiguous bytes
};
typedef struct mymsg_t mymsg_t;

void sender(){
	mymsg_t m1 = { 15,  "hello"};
	mymsg_t m2 = {20,  "goodbye"};
	int mid;
	key_t key = 100;
//	u,g,o can read/write into queue
			mid = msgget (key, 0666 | IPC_CREAT); //ugo can read/write into the queue
			//msgsnd will block if full otherwise

	msgsnd (mid, (void *)&m1, sizeof (msg_contents_t), 0); msgsnd (mid, (void *)&m2, sizeof (msg_contents_t), 0);
//	msgsnd will block if queue is full, otherwise:
	msgsnd (mid, (void *)&m1, sizeof (msg_contents_t), IPC_NOWAIT);
//	Returns -1 if cannot send (and errno = ENOMSG)
}
void receiver(){
	mymsg_t msg;
	int mid;
	key_t key = 100;
	mid = msgget (key, 0666 | IPC_CREAT); // read msgs with tag 15 and 20
// will block if such messages are not there
	msgrcv (mid, (void *)&msg, sizeof (msg_contents_t), 20, 0);
	msgrcv (mid, (void *)&msg, sizeof (msg_contents_t), 15, 0);
//	non-blocking:
//	res = msgrcv (mid, (void *)&msg, sizeof (msg_contents_t), 30, IPC_NOWAIT);
//	Returns -1 if not on queue (and errno = ENOMSG)
}
#endif //OS_SENDER_H
