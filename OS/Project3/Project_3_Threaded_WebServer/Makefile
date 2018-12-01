CC = gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread -pthread


web_server: server.c
	${CC} -o web_server server.c util.o ${LDFLAGS}

util:
	${CC} -c util.c

clean:
	rm web_server
