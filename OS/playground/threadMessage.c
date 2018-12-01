//
// Created by Grant Udstrand on 11/27/18.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int x,y;
pthread_t t1, t2;
pthread_mutex_t xlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ylock = PTHREAD_MUTEX_INITIALIZER;

void* func_t1(){
	pthread_mutex_lock(&xlock);
	x = x+1;
//	pthread_mutex_lock(&ylock);
	y = y+1;
//	pthread_mutex_unlock(&ylock);
	pthread_mutex_unlock(&xlock);
	printf("Exited t1\n");
}
void func_t2(){
	pthread_mutex_lock(&ylock);
	y = y+2;
	pthread_mutex_lock(&xlock);
	x = x+2;
	pthread_mutex_unlock(&xlock);
	pthread_mutex_unlock(&ylock);
	printf("Exited t2\n");
}

int main(){
	pthread_create(&t1, NULL, func_t1, NULL);
	pthread_join(t1, NULL);
	pthread_create(&t2, NULL, func_t2, NULL);
	pthread_join(t2, NULL);
}