//
// Created by Grant Udstrand on 10/15/18.
//

#ifndef OS_CHAIN_H
#define OS_CHAIN_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
//void chain(){
//	pid_t childpid = 0;
//	int i, n;
//
//	if(argc!= 2){
//		fprintf(stderr, "Usage %s processes\n", argv[0]);
//		return 1;
//	}
//
//	n = atoi(argv[1]);
//	for(i = 1; i < n; i++){
//		if(childpid = fork()){
//			break;
//		}
//	}
//	fprintf(stderr, "i: %d, process id: %ld parent ID: %ld child id: %ld\n",
//	        i, (long) getpid(), (long) getppid(), (long) childpid );
//}
#endif //OS_CHAIN_H
