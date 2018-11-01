CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

all : em_server_mq em_client_mq \
     

em_server_mq : em_server_mq.c
	$(CC) -o $@ $^

em_client_mq : em_client_mq.c
	$(CC) -o $@ $^


clean :
	rm -f em_server_mq em_client_mq \
	     



