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
#define MAX_cache_len 100
#define MAX_CE 100
#define INVALID -1
#define BUFF_SIZE 1024
#define STDOUT_FILENO 1
#define INT_SIZE 8
#define STD_BASE 10

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

/**GLOBAL VARIABLES**/
int gfd; // Descriptor for further request processing
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

/** Integer to ASCII **/
// inline function to swap two numbers
void swap(char *x, char *y) {
  char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
  while (i < j)
    swap(&buffer[i++], &buffer[j--]);

  return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
  // invalid input
  if (base < 2 || base > 32)
    return buffer;

  // consider absolute value of number
  int n = abs(value);

  int i = 0;
  while (n)
  {
    int r = n % base;

    if (r >= 10)
      buffer[i++] = 65 + (r - 10);
    else
      buffer[i++] = 48 + r;

    n = n / base;
  }

  // if number is 0
  if (i == 0)
    buffer[i++] = '0';

  // If base is 10 and value is negative, the resulting string
  // is preceded with a minus sign (-)
  // With any other base, value is always considered unsigned
  if (value < 0 && base == 10)
    buffer[i++] = '-';

  buffer[i] = '\0'; // null terminate string

  // reverse the string and return it
  return reverse(buffer, 0, i - 1);
}

/** **/

/* ************************************ Cache Code ********************************/

//Cache must be a global variable so all threads can access it.

cache_entry_t* CACHE; //Create a global array for cache
int CACHE_LEN; //Create a global var for the length of the cache.

// Function to check whether the given request is present in cache
int getCacheIndex(char *request){
  /// return the index if the request is present in the cache, return -1 if not in cache
  for(int i = 0; i < MAX_cache_len; i++){
    if(CACHE[i].request == request){
      return i;
    }
  }
  return -1;
}

// Function to add the request and its file content into the cache
void addIntoCache(char *request, char *memory , int memory_size){
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
int QUEUE_LEN = 0; //Length of the queue. Init at 0, change to however many queue requests we have.

// Function to add the request and its file content into the queue
void addIntoQueue(int fd, void* request){
//  printf("The size of the queue before here was %d. The size of a request is %d\n", sizeof(QUEUE),sizeof(request_t));
  request_t new_request; //Create a new request. Put the request data in this request struct.
  new_request.request = request;
  new_request.fd = fd;
  request_t* tmpQ = (request_t*) malloc((QUEUE_LEN+1)* sizeof(request_t)); //Create a temporary queue
  tmpQ[QUEUE_LEN] =  new_request; //Add the request at the end of the q
//  tmpQ = QUEUE; //Put the current Queue in the place pointed to by tmp
//  free(QUEUE); //Remove the data that was in queue
  QUEUE = tmpQ; //Assign new queue to q.
  printf("The oldest request is: %s\n",(char*)QUEUE[QUEUE_LEN].request);
  QUEUE_LEN +=1; //increment the length. The highest index will be QUEUE_LEN-1.
//  printf("Got this far in addIntoQueue\n");
//  free(tmpQ); //Free the memory at the place of tmpQ
  printf("Successfully added to the queue! Queue is now size %d\n", QUEUE_LEN);
}

//Function to remove the first request in the queue, and decrement the queue length by one. Returns -1 if no queue
//entries, and zero if there are.
request_t removeRequestFromQueue(){
  if(QUEUE_LEN > 0){
    request_t this_request = QUEUE[QUEUE_LEN-1]; //The last index will always be the length of the queue minus one.
//    printf("%s\n", (char*)QUEUE[QUEUE_LEN].request);
    request_t* tmpQ = (request_t*) malloc((QUEUE_LEN-1)* sizeof(request_t)); //Create a temporary queue that is one smaller
    //TODO: Assign the data already in QUEUE from 1->QUEUE_LEN and assign to the tmp var.
    QUEUE = tmpQ; //Assign this
    QUEUE_LEN = QUEUE_LEN-1;
    printf("Successfully exited the remove request function. Removed request: %s\n", (char*)this_request.request);
    return this_request; //Added back the one to accomodate for the fact that we already changed the value of QUEUE_LEN
  }
  //TODO: Figure out what to do if the queue is empty, may be okay to just sit and churn, idk
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
  printf("%ld\n", strlen(mybuf));
  int dot_pos = strcspn(mybuf, ".");
  printf("%d\n",dot_pos);
  printf("%s\n", &mybuf[dot_pos]);
  char content_type[16]; //TODO: Change this to a sensical value.
  strcpy(content_type, &mybuf[dot_pos]); //https://stackoverflow.com/questions/12504657/copy-end-of-string-in-c
  printf("GOT CONTENT TYPE: %s\n", content_type);
  if(strcmp(content_type, ".html") == 0){
    return "text/html";
  }
  else if(strcmp(content_type, ".jpg") == 0){
    return "image/jpeg";
  }
  else if(strcmp(content_type, ".gif") == 0){
    return "image/gif";
  }
  else{
    return "text/plain";
  }

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
    //TODO: Determine if a global access to this return resutl is appropriate. More than likely is not so we need ITC
    gfd = accept_connection(); //Gets the file descriptor. This is a blocking call until we receive a connection
    printf("I found an fd of %d\n", gfd);
    if(gfd > 0) { //if we actually secure a connection

      // Get request from the client
      if(get_request(gfd, filename) != 0){
        printf("Bad Request in Dispatch"); //TODO: Make sure this actually handles the error.
      }
      // Add the request into the queue
      addIntoQueue(gfd, filename);
    }
   }
   return NULL;
}

/**********************************************************************************/

// Function to retrieve the request from the queue, process it and then return a result to the client
void * worker(void *arg) {
  printf("Worker thread number %d initialized\n", arg);
  int worker_id = arg;
  int requests_processed = 0; //The number of requests this worker has processed.

   while (1) {
     int time_of_request = 0; //Where the time measured will be stored.
     // Start recording time
     int start = getCurrentTimeInMills(); // Get a starting timestamp


    // Get the request from the queue
    request_t cur_request;
    if(QUEUE_LEN > 0) { //We don't want to waste time pulling requests if there aren't any.
      cur_request = removeRequestFromQueue();
      printf("The request I pulled in this worker thread has message %s\n", cur_request.request);
      char content_type[128]; //TODO: Assign a size to buffers handling content size
      strcpy(content_type, getContentType(cur_request.request));
      printf("This request has a content type of %s\n", getContentType(cur_request.request)); // Show us the c-type


      /** Not originally like this, but it keeps from having unnecessary prints and searches. **/
      // Get the data from the disk or the cache
      if(getCacheIndex(cur_request.request) != -1){
        printf("Found the request in the cache\n");
      }
      else{
        printf("Have to search the disk for file %s.\n", cur_request.request);
        char search[BUFF_SIZE];
        strcat(search, "./testing");
        strcat(search, cur_request.request);
        printf("Have built the search string %s\n", search);
        int fd =-1; //The fd of the file we are pulling from the disk. -1 would indicate an error.
        char BUF[BUFF_SIZE]; // This is where we are storing the file. Will send this pointer to the cache.
        //TODO: On this open call, we want to make sure we return some form of error if we don't find a file.
        if((fd = open(search, O_RDWR)) ==-1){
          return_error(gfd, BUF);
        };
        int bytes_read = -1; //This is how many bytes are returned by a successful request. Will be -1 if we failed.
        if((bytes_read = read(fd, BUF, BUFF_SIZE)) ==-1){
          return_error(gfd, BUF);
        }; //Read the data stored at that location into the Buffer we provide.
        close(fd);
        printf("Will attempt to return file: fd:%d; content: %s; BUF:%s; BUFF_SIZE: %d\n", fd,content_type,BUF,BUFF_SIZE);
        if(return_result(gfd,content_type,BUF,BUFF_SIZE) != -1){
          printf("Successfully returned a result\n");
        }
        else{
          printf("We did NOT return a result... \n");
          //TODO: Figure out what to do if we get a bad request
        }
        // Stop recording the time
        int end = getCurrentTimeInMills();
        time_of_request = end-start;


        //TODO: Block this off into a separate function, we need to abstract some of this worker thread
        char str[BUFF_SIZE]; // This is where the full request will be placed.
        char id[INT_SIZE]; //This is the worker id
        itoa(worker_id, id, STD_BASE);
        char reqNum[INT_SIZE]; //This is the number of requests processed by this worker so far
        itoa(requests_processed, reqNum, STD_BASE);
        char acfd[INT_SIZE]; //This is the fd given to us by accept_connection()
        itoa(gfd, acfd, STD_BASE);
        char request_string[BUFF_SIZE]; //This is the filename buffer filled in by the get request function
        strcpy(request_string, cur_request.request);
        char bytes_error[INT_SIZE];
        itoa(bytes_read, bytes_error, STD_BASE);
        char time[INT_SIZE]; //This is the time of the request
        itoa(time_of_request, time, STD_BASE);
        char hit_miss[INT_SIZE]; //This will be "HIT" on a cache hit and "MISS" on a cache miss
        strcpy(hit_miss, "MISS");

        //TODO: Protect these printf statements. They are for formatted output and need to be locked because of thread safety.
        printf("[%s][%s][%s][%s][%s][%s][%s]\n", id, reqNum, acfd, request_string, bytes_error, time, hit_miss);



        // Log the request into the file and terminal
        write(STDOUT_FILENO, str, strlen(str)); //TODO: Make all of the print statements system calls to write b/c thread safe
      }
    }


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
  pthread_t workers;
  int worker_count = 0;
  for(worker_count = 0; worker_count < num_workers; worker_count++){
    void* args;
    args = worker_count;
    pthread_create(&workers, NULL, worker, worker_count);
  }
  pthread_t dispatchers;
  int dispatch_count = 0;
  for(dispatch_count = 0; dispatch_count < num_dispatcher; dispatch_count++){
    pthread_create(&dispatchers, NULL, dispatch, dispatch_count);
  }
  pthread_join(&dispatchers[0], NULL); //TODO: Figure out if this joining strategy is the best way to do this or not.

  // Clean up
  deleteCache();
  deleteQueue();
  printf("Successfully Started Up\n");
  return 0;
}
