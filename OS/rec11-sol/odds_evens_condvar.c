// odd/even thread coordination on incrementing an global
// variable. This version uses a condition variable to efficiently
// notify threads of changes to the global variable.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  2                               // number of odd/evens threads
#define TOT_THREADS (2*NUM_THREADS)                  // total threads
#define THREAD_ITERS 5                               // total iterations  each thread will add on

void update(int *x){
  *x = *x+1;
  usleep(1);          // simulate some "work" done while locked
}

int count = 0;                                       // global variable all threads are modifiying
pthread_mutex_t count_mutex;                         // mutex to check count
pthread_cond_t  count_condv;                         // condition variable to receive wake-ups

// Run by even child threads, increment count when it is even
void *even_work(void *t) {
  int tid = *( (int *) t);
  for(int i=0; i<THREAD_ITERS; i++){
    pthread_mutex_lock(&count_mutex);
    while(count % 2 != 0){                           // wait until count is even
      pthread_cond_wait(&count_condv, &count_mutex); // await notification to check again, gets lock on wakeup
    }
    printf("Count : %d, Thread ID : %d printed EVEN number during %d iteration \n", count, tid, i);
    update(&count);                                         // now have lock and condition is correct can safely increment
    pthread_mutex_unlock(&count_mutex);              // release lock
    pthread_cond_broadcast(&count_condv);            // notify all others waiting
  }
  printf("%d FINISHED %d iterations\n", tid, THREAD_ITERS);
  return NULL;
}

// Run by odd child threads, increment count when it is odd
void *odd_work(void *t) {
  int tid = *( (int *) t);
  for(int i=0; i<THREAD_ITERS; i++){
    pthread_mutex_lock(&count_mutex);
    while(count % 2 != 1){                           // wait until count is odd
      pthread_cond_wait(&count_condv, &count_mutex); // await notification to check again, gets lock on wakeup
    }
    printf("Count : %d, Thread ID : %d printed ODD number during %d iteration \n", count, tid, i);
    update(&count);                                         // now have lock can safely increment
    pthread_mutex_unlock(&count_mutex);              // release lock
    pthread_cond_broadcast(&count_condv);            // notify all others waiting, pthread_cond_signal() only notifies single thread
  }
  printf("%d FINISHED %d iterations\n", tid, THREAD_ITERS);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t threads[TOT_THREADS];
  int tids[TOT_THREADS];

  pthread_mutex_init(&count_mutex, NULL);            // Initialize mutex and condition variable
  pthread_cond_init (&count_condv, NULL);

  for(int i=0; i<TOT_THREADS; i+=2){
    tids[i] = i;
    pthread_create(&threads[i],   NULL, even_work, (void *) &(tids[i]));
    tids[i+1] = i+1;
    pthread_create(&threads[i+1], NULL, odd_work,  (void *) &(tids[i+1]));
  }

  for(int i=0; i<TOT_THREADS; i++) {                 // Wait for all threads to complete
    pthread_join(threads[i], NULL);  // This is the equivalent of a wait statement for threads.
  }

  printf("main: count is %d\n",count);

  pthread_mutex_destroy(&count_mutex);               // Clean up and exit
  pthread_cond_destroy(&count_condv);
  exit(0);
}
