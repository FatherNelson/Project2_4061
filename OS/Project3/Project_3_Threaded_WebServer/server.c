#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "util.h"
#include <stdbool.h>
//#include "slice.h"

#define MAX_THREADS 100
#define MAX_queue_len 100
#define MAX_CE 100
#define INVALID -1
#define BUFF_SIZE 1024

/*
  THE CODE STRUCTURE GIVEN BELOW IS JUST A SUGESSTION. FEEL FREE TO MODIFY AS NEEDED
*/

// structs:
typedef struct request_queue {
   int fd;
   void *request;
} request_t;

typedef struct cache_entry {
    int len;
    char *request;
    char *content;
} cache_entry_t;

/* ************************ Dynamic Pool Code ***********************************/
// Extra Credit: This function implements the policy to change the worker thread pool dynamically
// depending on the number of requests
void * dynamic_pool_size_update(void *arg) {
  while(1) {
    // Run at regular intervals
    // Increase / decrease dynamically based on your policy
  }
}
/**********************************************************************************/

/* ************************************ Array Operations ********************************/
void slice_queue(request_t* src, request_t* dest, int start, int end)
{
  int pos_in_dest =0;
  for(int i=start; i < end; i++){
    dest[pos_in_dest] = src[i];
    pos_in_dest++;
  }
}
/**********************************************************************************/

/* ************************************ Cache Code ********************************/

//Cache must be a global variable so all threads can access it.

cache_entry_t* CACHE; //Create a global array for cache

// Function to check whether the given request is present in cache
int getCacheIndex(char *request){
  /// return the index if the request is present in the cache
}

// Function to add the request and its file content into the cache
void addIntoCache(char *mybuf, char *memory , int memory_size){
  // It should add the request at an index according to the cache replacement policy
  // Make sure to allocate/free memeory when adding or replacing cache entries
}

// clear the memory allocated to the cache
void deleteCache(){
  // De-allocate/free the cache memory
}

// Function to initialize the cache
void initCache(){
  // Allocating memory and initializing the cache array
  CACHE =  malloc(MAX_CE * sizeof(struct cache_entry)); //We are setting the array to point the
  //Cache at the block of memory we asked for.
  printf("Successfully initialized cache\n");
}

// Function to open and read the file from the disk into the memory
// Add necessary arguments as needed
int readFromDisk(/*necessary arguments*/) {
  // Open and read the contents of file given the request
}

/**********************************************************************************/

/* ************************************ Queue Code ********************************/
request_t* QUEUE; //Create a global queue accessible by all threads. THIS IS A POINTER. This is important to remember
int QUEUE_LEN = 0; //Length of the queue. Init at zero, change to however many queue requests we have.

// Function to add the request and its file content into the queue
void addIntoQueue(int fd, void* request){
//  printf("The size of the queue before here was %d. The size of a request is %d\n", sizeof(QUEUE),sizeof(request_t));
  QUEUE_LEN +=1; //increment the length. The highest index will be QUEUE_LEN-1.
  request_t new_request; //Create a new request. Put the request data in this request struct.
  new_request.request = request;
  new_request.fd = fd;
  request_t* tmpQ = (request_t*) malloc((QUEUE_LEN)* sizeof(request_t)); //Create a temporary queue
  tmpQ[QUEUE_LEN] =  new_request; //Add the request at the end of the q
//  tmpQ = QUEUE; //Put the current Queue in the place pointed to by tmp
//  free(QUEUE); //Remove the data that was in queue
  QUEUE = tmpQ; //Assign new queue to q.
  printf("The oldest request is: %s\n",(char*)QUEUE[QUEUE_LEN].request);
  printf("Got this far in addIntoQueue\n");
//  free(tmpQ); //Free the memory at the place of tmpQ
  printf("Successfully added to the queue! Queue is now size %d\n", sizeof(tmpQ));
}

//Function to remove the first request in the queue, and decrement the queue length by one.
request_t removeRequestFromQueue(){
  printf("%s\n", QUEUE[0].request);
  return *QUEUE; //Should give the first element in the queue.
//  request_t* tmpQ = (request_t*) malloc((QUEUE_LEN-1)* sizeof(request_t)); //Create a temporary queue that is one smaller
//  slice_queue(QUEUE, tmpQ, (int)1, QUEUE_LEN); //Copy from the first position to the last of the queue
//  QUEUE = tmpQ; //Assign the new queue to the global queue pointer.
//  free(tmpQ); //Free the temporary queue pointer.
}

// clear the memory allocated to the queue
void deleteQueue(){
  // De-allocate/free the queue memory
  request_t* tmpQ = malloc(0);
  QUEUE = tmpQ;
  QUEUE_LEN = 0;
  printf("I have deleted everything from the Queue\n");
}
// Function to initialize the queue
void initQueue(){
  // Allocating memory and initializing the cache array
  QUEUE =  malloc(MAX_CE * sizeof(struct request_queue)); //Create a block of memory the size of MAX_CE requests.
  //Cache at the block of memory we asked for.
  printf("Successfully initialized queue!\n");
}
/**********************************************************************************/

/* ************************************ Utilities ********************************/
// Function to get the content type from the request
char* getContentType(char * mybuf) {
  // Should return the content type based on the file type in the request
  // (See Section 5 in Project description for more details)
}

// This function returns the current time in milliseconds
int getCurrentTimeInMills() {
  struct timeval curr_time;
  gettimeofday(&curr_time, NULL);
  return curr_time.tv_usec;
}

/**********************************************************************************/


// Function to receive the request from the client and add to the queue
void * dispatch(void *arg) {
  printf("Dispatcher thread number %d created\n", arg);
  while (1) {
    printf("Entered the while statement\n ");
    // Accept client connection
    char filename[BUFF_SIZE]; //Holds the filename
    int fd; // Descriptor for further request processing
    fd = accept_connection(); //Gets the file descriptor
    printf("I found an fd of %d\n", fd);
    if(fd > 0) {

      // Get request from the client
      if(get_request(fd, filename) != 0){
        printf("Bad Request in Dispatch"); //TODO: Make sure this actually handles the error.
      }

      // Add the request into the queue
      addIntoQueue(fd, filename);

    }
    else{
      printf("I am a new dispatcher thread that did nothing\n");
//      return NULL;
    }

   }
   return NULL;
}

/**********************************************************************************/

// Function to retrieve the request from the queue, process it and then return a result to the client
void * worker(void *arg) {
  printf("Worker thread number %d initialized\n", arg);
   while (1) {
    int time_of_request = 0; //Where the time measured will be stored.
    // Start recording time
    int start = getCurrentTimeInMills(); // Get a starting timestamp


    // Get the request from the queue
//    request_t cur_request = removeRequestFromQueue();
//      removeRequestFromQueue();
//    printf("here in worker thread\n");
//    printf("The current request has fd %d and message %s\n", cur_request.fd, cur_request.request);

    // Get the data from the disk or the cache

    // Stop recording the time
    int end = getCurrentTimeInMills();
    time_of_request = end-start;

    // Log the request into the file and terminal
//    printf("Worker thread made a request\n");
    return NULL;

    // return the result
  }
  return NULL;
}

/**********************************************************************************/

void display_help(){
  printf("This server is initialized with the command: "
         "./web_server port path num_dispatch num_workers dynamic_flag qlen cache_entries");
  printf("Here is a description of the arguments:\n"
         "port - number on your local machine. May only use ports 1025-65535\n"
         "path - is the path to your web root location from where the file will be served\n"
         "num_dispatcher - is how many dispatcher threads to start up\n"
         "num_workers - is how many worker threads to start up"
         "dynamic_flag - indicates whether the thread pool will be static or dynamic. 0 = static, 1 = dynamic\n"
         "qlen - is the fixed, bounded length of the request queue\n"
         "cache_entries - is the number of entries available in the cache\n");

}
/**********************************************************************************/
int main(int argc, char **argv) {

  //Check if the help function was hit
  if(strcmp(argv[1], "-h") == 0){
    display_help();
    return 0;
  }

  // Error check on number of arguments
  // Decided to check if caching is enabled [argc == 8 -> Caching enabled]
  if(argc != 7 && argc != 8){
    printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
    return -1;
  }
  // Get the input args
  int port = atoi(argv[1]); //The port number
  char* path[BUFF_SIZE];
  strcpy(path, argv); // The path to the web root location from where the files will be served
  int num_dispatcher = atoi(argv[3]); // How many dispatcher threads to start up
  int num_workers = atoi(argv[4]); // How many worker threads to start up
  int dynamic_flag = atoi(argv[5]); // Indicates whether to make the thread pool size static or dynamic
  int qlen = atoi(argv[6]); // Fixed bounded length of the queue.
  int cache_entries = atoi(argv[7]); //The number of entries available in the cache

  // Perform error checks on the input arguments
  if(port < 1024 || port > 65535){
    printf("Please specify a port number between 1025 and 65535\n");
    return 0;
  }
  //TODO: Error check the path variable on startup
  if(num_dispatcher < 0||num_workers < 0||dynamic_flag < 0||qlen < 0||cache_entries < 0){
    printf("You must enter a positive integer for these number, run \" ./web_server -h \" for help\n");
    return 0;
  }
  // Change the current working directory to server root directory
  chdir(path); // Changes the current directory to the specified path.
  // Start the server and initialize cache
  init(port);
  initCache();

//   Create dispatcher and worker threads
//  pthread_t workers;
//  int worker_count = 0;
//  for(worker_count = 0; worker_count < num_workers; worker_count++){
//    void* args;
//    args = worker_count;
//    pthread_create(&workers, NULL, worker, worker_count);
//  }
  pthread_t dispatchers;
  int dispatch_count = 0;
  for(dispatch_count = 0; dispatch_count < num_dispatcher; dispatch_count++){
    pthread_create(&dispatchers, NULL, dispatch, dispatch_count);
    pthread_join(&dispatchers[0], NULL);
  }

  // Clean up
  printf("Successfully Started Up\n");
  return 0;
}
