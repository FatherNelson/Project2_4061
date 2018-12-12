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


#define MAX_THREADS 100
#define MAX_queue_len 100
#define MAX_cache_len 100
#define MAX_CE 100
#define INVALID - 1
#define BUFF_SIZE 1024
#define STDOUT_FILENO 1
#define INT_SIZE 8
#define STD_BASE 10

//TODO: 3) Perform Analytics

// structs:
typedef struct request_queue {
	int fd;
	void * request;
}
		request_t;

typedef struct cache_entry {
	int len;
	char * request;
	char * content;
}
		cache_entry_t;

/**GLOBAL VARIABLES**/
//int gfd; // Descriptor for further request processing
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dispatch_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t worker_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t worker_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t gfd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gfd_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_full_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_empty_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cache_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cache_cond = PTHREAD_COND_INITIALIZER;

/* ************************ Dynamic Pool Code ***********************************/
// Extra Credit: This function implements the policy to change the worker thread pool dynamically
// depending on the number of requests
void * dynamic_pool_size_update(void * arg) {
	while (1) {
		// Run at regular intervals
		// Increase / decrease dynamically based on your policy
	}
}
/**********************************************************************************/

/* ************************************ Array Operations ********************************/
// void slice_queue(request_t * src, request_t * dest, int start, int end) {
// 	int pos_in_dest = 0;
// 	for (int i = start; i < end; i++) {
// 		dest[pos_in_dest] = src[i];
// 		pos_in_dest++;
// 	}
// }
/**********************************************************************************/

/** Integer to ASCII **/
// inline function to swap two numbers
void swap(char * first_number, char * second_number) {
	char t = * first_number;* first_number = * second_number;* second_number = t;
}

// function to reverse buffer[i..j]
char * reverse(char * buf, int i, int j) {
	while (i < j)
		swap( & buf[i++], & buf[j--]);
	return buf;
}

// Iterative function to implement itoa() function in C
char * itoa(int number, char * buf, int base) {
	// invalid input
	if (base < 2 || base > 32)
		return buf;

	// consider absolute value of number
	int n = abs(number);

	int i = 0;
	while (n) {
		int r = n % base;

		if (r >= 10)
			buf[i++] = 65 + (r - 10);
		else
			buf[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buf[i++] = '0';

	// If base is 10 and value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (number < 0 && base == 10)
		buf[i++] = '-';

	buf[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buf, 0, i - 1);
}

/** **/

/* ************************************ Cache Code ********************************/

//Cache must be a global variable so all threads can access it.

cache_entry_t * CACHE; //Create a global array for cache
int CACHE_LEN = 0; //Create a global var for the length of the cache. Should start at length zero. Newest entry at
//CACHE_LEN -1

//Function to spit out the contents of the cache on demand
void printCache() {
	for (int i = 0; i < CACHE_LEN; i++) {
		printf("Cache Entry %d: %s\n", i, CACHE[i].request);
	}
}

// Function to check whether the given request is present in cache
int getCacheIndex(char * request) {
	/// return the index if the request is present in the cache, return -1 if not in cache
	for (int i = 0; i < CACHE_LEN; i++) {
		if (strcmp(CACHE[i].request, request) == 0) {
			return i;
		}
	}
	return -1;
}

// Function to add the request and its file content into the cache
void addIntoCache(char * request, char * memory, int memory_size) {
	// It should add the request at an index according to the cache replacement policy
	// Make sure to allocate/free memeory when adding or replacing cache entries
	printf("Entered the addIntoCache function\n");
	cache_entry_t new_entry;
	cache_entry_t * new_cache;
	if ((new_cache = malloc((CACHE_LEN + 1) * sizeof(cache_entry_t))) == NULL) {
		perror("Failed to allocate memory for cache");
	} //The location of the new cache.
	new_entry.request = strdup(request);
	char content_buf[memory_size];
	strcpy(content_buf, memory);
	new_entry.content = strdup(memory);
	new_entry.len = memory_size;
	for (int i = 0; i < CACHE_LEN; i++) {
		new_cache[i] = CACHE[i]; //Copy over what we had before
	}
	new_cache[CACHE_LEN] = new_entry; //Add the new entry at the end of the cache
	CACHE_LEN += 1; //Indicate that we have added a new struct at the end of the array.
	free(CACHE);
	CACHE = new_cache; //new_cache now has the data we desire.
	for (int i = 0; i < CACHE_LEN; i++) {
		printf("The new cache has at the %d position: %s\n", i, CACHE[i].request);
	}
	printf("\n");
	printCache();

}

// clear the memory allocated to the cache
void deleteCache() {
	// De-allocate/free the cache memory
	CACHE_LEN = 0;
	free(CACHE);
}

// Function to initialize the cache
void initCache() {
	// Allocating memory and initializing the cache array
	CACHE = malloc(MAX_CE * sizeof(cache_entry_t)); //We are setting the array to point the
	//Cache at the block of memory we asked for.
	printf("Successfully initialized cache\n");
}



/**********************************************************************************/

/* ************************************ Queue Code ********************************/
request_t * QUEUE; //Create a global queue accessible by all threads. THIS IS A POINTER. This is important to remember
int QUEUE_LEN = 0; //Length of the queue. Init at 0, change to however many queue requests we have.
int QUEUE_START = 0;

// Function to add the request and its file content into the queue
void add_into_queue(int fd, void * request) {
	pthread_mutex_lock(&queue_mutex);
	request_t new_request; //Create a new request. Put the request data in this request struct.
	new_request.request = request;
	new_request.fd = fd;
	while (QUEUE_START - QUEUE_LEN == 1 || (QUEUE_START == 0 && QUEUE_LEN== 99)) {
		// Queue is full
		pthread_cond_wait(&queue_full_cond, &queue_mutex);
	}
	QUEUE[QUEUE_LEN % MAX_queue_len] = new_request;
	QUEUE_LEN = (QUEUE_LEN + 1) % MAX_queue_len; //increment the length. The highest index will be QUEUE_LEN-1.
	pthread_cond_broadcast(&queue_empty_cond);
	pthread_mutex_unlock(&queue_mutex);
	printf("Added request to the queue Queue is now size %d\n", QUEUE_LEN-QUEUE_START);
	// Print queue:
	for (int i = QUEUE_START; i < QUEUE_LEN; i++) {	// fix to wrap around
		printf("%d", i);
		if(QUEUE[i].request != NULL) {
			printf("QUEUE Entry %d: %s\n", i, QUEUE[i].request);
		}
		printf("%d", i);
		printf("\n");
	}


}

//Function to remove the first request in the queue, and decrement the queue length by one. Returns -1 if no queue
//entries, and zero if there are.
request_t removeRequestFromQueue() {
	request_t this_request = QUEUE[QUEUE_START]; //The last index will always be the length of the queue minus one.
	printf("%s\n", (char*)QUEUE[QUEUE_START].request);
	//request_t * tmpQ = (request_t * ) malloc((QUEUE_LEN - 1) * sizeof(request_t)); //Create a temporary queue that is one smaller
	//TODO: Assign the data already in QUEUE from 1->QUEUE_LEN and assign to the tmp var.
	//QUEUE[] = tmpQ; //Assign this
	QUEUE_START = (QUEUE_START + 1) % MAX_queue_len;
	printf("Successfully exited the remove request function. Removed request: %s\n", (char * ) this_request.request);
	return this_request;
//	printf("Returning a dud from remove request \n");
//	request_t *dud = (request_t*)malloc(sizeof(request_t));
//	return *dud;
}

// clear the memory allocated to the queue
void deleteQueue() {
	// De-allocate/free the queue memory
	free(QUEUE);
	QUEUE_LEN = 0;
	printf("I have deleted everything from the Queue\n");
}
// Function to initialize the queue
void initQueue() {
	// Allocating memory and initializing the cache array
	QUEUE = malloc(MAX_CE * sizeof(struct request_queue) * MAX_queue_len); //Create a block of memory the size of MAX_CE requests.
	//Cache at the block of memory we asked for.
	printf("Successfully initialized queue!\n");
}
/**********************************************************************************/

/* ************************************ Utilities ********************************/
// Function to get the content type from the request
char * getContentType(char * mybuf) {
	// Should return the content type based on the file type in the request
	// (See Section 5 in Project description for more details)
	printf("%ld\n", strlen(mybuf));
	int dot_pos = strcspn(mybuf, ".");
	printf("%d\n", dot_pos);
	printf("%s\n", & mybuf[dot_pos]);
	char content_type[16]; //TODO: Change this to a sensical value.
	strcpy(content_type, "\0"); //Clear the content_type buffer
	strcpy(content_type, & mybuf[dot_pos]); //https://stackoverflow.com/questions/12504657/copy-end-of-string-in-c
	printf("GOT CONTENT TYPE: %s\n", content_type);
	if (strcmp(content_type, ".html") == 0) {
		return "text/html";
	} else if (strcmp(content_type, ".jpg") == 0) {
		return "image/jpeg";
	} else if (strcmp(content_type, ".gif") == 0) {
		return "image/gif";
	} else if (strcmp(content_type, ".cache") == 0) {
		printCache();
		return "C";
	} else {
		return "text/plain";
	}

}

// This function returns the current time in milliseconds
int getCurrentTimeInMills() {
	struct timeval curr_time;
	gettimeofday( & curr_time, NULL);
	return curr_time.tv_usec;
}

/**********************************************************************************/

// Function to receive the request from the client and add to the queue
void * dispatch(void * arg) {
	printf("Dispatcher thread number %d created\n", (int)arg);
	while (1) {
		// Accept client connection
		char filename[BUFF_SIZE]; //Holds the filename
//		pthread_mutex_lock(&gfd_mutex); //Acquire the mutex
		//TODO: Determine if a global access to this return result is appropriate. More than likely is not so we need ITC
		int gfd = accept_connection(); //Gets the file descriptor. This is a blocking call until we receive a connection
		printf("I found an fd of %d\n", gfd);
		if (gfd > 0) { //if we actually secure a connection

			// Get request from the client
			if (get_request(gfd, filename) != 0) {
				char ERR_BUF[BUFF_SIZE] = "Error - bad request";
				return_error(gfd, ERR_BUF);
			}
			else {
				// Add the request into the queue
				// Lock will be acquired inside add_into_queue
				printf("Adding to queue\n");
				add_into_queue(gfd, filename);
//				pthread_cond_broadcast(&worker_cond);
			}
		}
//		pthread_cond_broadcast(&gfd_cond);
//		pthread_mutex_unlock(&gfd_mutex);
	}
	return NULL;
}

/**********************************************************************************/
//Function to log the results of a particular worker. TODO: Add error checking to this function, and return -1 on fail
void worker_log_results(int worker_id, int requests_processed, int gfd, char * request_string,
                        int bytes_read, int time_of_request, char * hit_miss) {
	char str[BUFF_SIZE]; // This is where the full request will be placed.
	// char id[INT_SIZE]; //This is the worker id
	// itoa(worker_id, id, STD_BASE);
	// char reqNum[INT_SIZE]; //This is the number of requests processed by this worker so far
	// itoa(requests_processed, reqNum, STD_BASE);
	// char acfd[INT_SIZE]; //This is the fd given to us by accept_connection()
	// itoa(gfd, acfd, STD_BASE);
	// char bytes_error[INT_SIZE];
	// itoa(bytes_read, bytes_error, STD_BASE);
	// char time[INT_SIZE]; //This is the time of the request
	// itoa(time_of_request, time, STD_BASE);
	// printf("[%d][%d][%d][%s][%d][%dmS][%s]\n", worker_id, requests_processed, gfd, request_string, bytes_read, time_of_request, hit_miss);
	sprintf(str, "[%d][%d][%d][%s][%d][%dmS][%s]\n", worker_id, requests_processed, gfd, request_string, bytes_read, time_of_request, hit_miss);
	int log_fd = open("./web_server_log", O_WRONLY | O_APPEND);
	write(log_fd, str, strlen(str));
	close(log_fd);
}

// Function to open and read the file from the disk into the memory
// Add necessary arguments as needed
int readFromDisk(request_t cur_request, char * content_type, char ** BUF) {
	printf("Have to search the disk for file %s.\n", cur_request.request);
	/**MAYBE put this green wrapped code in a function by itself. **/
	int gfd = cur_request.fd;
	char search[BUFF_SIZE];
	strcpy(search, "\0"); //Clear the string to start.
	strcat(search, "./testing");
	strcat(search, cur_request.request);
	printf("Have built the search string %s\n", search);
	int fd = -1; //The fd of the file we are pulling from the disk. -1 would indicate an error.
	if ((fd = open(search, O_RDWR)) == -1) {
		char ERR_BUF[BUFF_SIZE] = "Error - bad request";
		return_error(gfd, ERR_BUF);
	};
	struct stat boof;
	fstat(fd, &boof);
	int size = boof.st_size;
	*BUF = (char*)malloc(size * sizeof(char));
	// char BUF[size]; // This is where we are storing the file. Will send this pointer to the cache.
	int bytes_read = -1; //This is how many bytes are returned by a successful request. Will be -1 if we failed.
	if ((bytes_read = read(fd, *BUF, size)) == -1) {
		return_error(gfd, *BUF);
	}; //Read the data stored at that location into the Buffer we provide.
	close(fd);
	return size;
	/** Code that could be in on **/

	// char* REQUEST = (char*)malloc(cur_request.request);
	//char* REQUEST = strdup(cur_request.request);

	//strcpy(REQUEST, (char *) cur_request.request);
	//addIntoCache(REQUEST, BUF, size);	//Add entry to cache

	//printf("Will attempt to return file: fd:%d; content: %s; BUF:%s; size: %d\n", fd, content_type,
	 //      BUF, size);

}

// Function to retrieve the request from the queue, process it and then return a result to the client
void * worker(void * arg) {
	printf("Worker thread number %d initialized\n", (int)arg);
	int worker_id = (int)arg;
	int requests_processed = 0; //The number of requests this worker has processed.

	while (1) {
		int time_of_request = 0; //Where the time measured will be stored.
		// Start recording time

		// Get the request from the queue
		request_t cur_request;
		// pthread_mutex_lock(&worker_mutex);
//		pthread_cond_wait(&worker_cond, &worker_mutex);
//		if (QUEUE_LEN != QUEUE_START) { //We don't want to waste time pulling requests if there aren't any.
			// printCache();
			printf("\n");
			printf("START OF REQUEST END: %d START: %d\n", QUEUE_LEN, QUEUE_START);
			printf("\n");

			//Critical Section
			pthread_mutex_lock(&queue_mutex);
			while (QUEUE_START == QUEUE_LEN) {
				// Queue is empty
				pthread_cond_wait(&queue_empty_cond, &queue_mutex);
			}
			cur_request = removeRequestFromQueue();
			int start = getCurrentTimeInMills();
			printf("start time: %d", start);
			pthread_cond_broadcast(&queue_full_cond);
			pthread_mutex_unlock(&queue_mutex);
			//End Critical Section
			int gfd = cur_request.fd;

			printf("The request I pulled in this worker thread has message %s\n", cur_request.request);
			char content_type[128]; //TODO: Assign a size to buffers handling content size
			strcpy(content_type, getContentType(cur_request.request));
			if (strcmp(content_type, "C") != 0) { //If not asking to print out the cache.
				printf("This request has a content type of %s\n", content_type); // Show us the c-type

				/** Not originally like this, but it keeps from having unnecessary prints and searches. **/
				// Get the data from the disk or the cache
				int cache_index = -1;
				if ((cache_index = getCacheIndex(cur_request.request)) >= 0) {
					printf("Found the request in the cache at position %d\n", cache_index);
					// New cache entries will be added at the end, so index is still valid.  No mutex needed.
					int end = getCurrentTimeInMills();
					printf("end time: %d", end);
					return_result(gfd, content_type, CACHE[cache_index].content, CACHE[cache_index].len);
					time_of_request = end - start;
					pthread_mutex_lock(&log_mutex);
					worker_log_results(worker_id, requests_processed, gfd, CACHE[cache_index].request,
					                   CACHE[cache_index].len, time_of_request, "HIT");
					pthread_mutex_unlock(&log_mutex);
					requests_processed += 1;
				} else {
					char* BUF;
					int bytes_read = readFromDisk(cur_request, content_type, &BUF);	// Also adds to cache and returns result

					// char* REQUEST = (char*)malloc(cur_request.request);
					char* REQUEST = strdup(cur_request.request);
					strcpy(REQUEST, (char *) cur_request.request);
					addIntoCache(REQUEST, BUF, bytes_read);	//Add entry to cache. copy buf in there
					free(REQUEST);

					printf("Gonna return the result");
					int end = getCurrentTimeInMills();
				 if (return_result(gfd, content_type, BUF, bytes_read) != -1) {
				 	printf("Successfully returned a result\n");
				 	free(BUF);
				 } else {
				 	char ERR_BUF[BUFF_SIZE] = "Error - bad request";
				 	return_error(gfd, ERR_BUF);
				 	free(BUF);
				 }


					// Stop recording the time

					time_of_request = end - start;

					//struct stat boof;
					//fstat(gfd, &boof);
					//int size = boof.st_size;
					// Log the request into the file and terminal
					pthread_mutex_lock(&log_mutex);
					worker_log_results(worker_id, requests_processed, gfd, cur_request.request, bytes_read, time_of_request,
					                   "MISS");
					pthread_mutex_unlock(&log_mutex);
					//Cache the results
					//char BUF[BUFF_SIZE];
					//char REQUEST[size];
					//strcpy(REQUEST, (char * ) cur_request.request);
					//addIntoCache(REQUEST, BUF, size);
					requests_processed += 1;
				}
			}
			printf("\n\n END OF REQUEST \n\n");
		}
//		else{
//			pthread_cond_wait(&worker_cond, &worker_mutex);
//		}
//		pthread_cond_broadcast(&worker_cond);
//		pthread_mutex_unlock(&worker_mutex);
		// return the result

	return NULL;
}
/**********************************************************************************/

void display_help() {
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
int main(int argc, char ** argv) {

	//Check if the help function was hit
	if (strcmp(argv[1], "-h") == 0) {
		display_help();
		return 0;
	}

	// Error check on number of arguments
	// Decided to check if caching is enabled [argc == 8 -> Caching enabled]
	if (argc != 7 && argc != 8) {
		printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
		return -1;
	}
	// Get the input args
	int port = atoi(argv[1]); //The port number
	char * path[BUFF_SIZE];
	strcpy(path, *argv); // The path to the web root location from where the files will be served
	int num_dispatcher = atoi(argv[3]); // How many dispatcher threads to start up
	int num_workers = atoi(argv[4]); // How many worker threads to start up
	int dynamic_flag = atoi(argv[5]); // Indicates whether to make the thread pool size static or dynamic
	int qlen = atoi(argv[6]); // Fixed bounded length of the queue.
	int cache_entries = atoi(argv[7]); //The number of entries available in the cache

	// Perform error checks on the input arguments
	if (port < 1024 || port > 65535) {
		printf("Please specify a port number between 1025 and 65535\n");
		return -1;
	}
	if (num_dispatcher > 100 || num_dispatcher < 1) {
		printf("Please specify a dispatcher count between 1 and 100\n");
		return -1;
	}
	if (num_workers > 100 || num_workers < 1) {
		printf("Please specify a worker count between 1 and 100\n");
		return -1;
	}
	if (dynamic_flag < 0 || dynamic_flag > 1) {
		printf("Please set whether you want dynamic worker threads or not.\n"
		       "run \" ./web_server -h \" for help\n ");
		return -1;
	}
	if (qlen < 0 || qlen > 100) {
		printf("Please specify a queue length between 1 and 100\n "
		       "run \" ./web_server -h \" for help\n ");
	}
	if (cache_entries < 0 || cache_entries > 100) {
		printf("Please specify a cache size between 1 and 100\n "
		       "run \" ./web_server -h \" for help\n ");
	}
	//TODO: Error check the path variable on startup
	if (num_dispatcher < 0 || num_workers < 0 || dynamic_flag < 0 || qlen < 0 || cache_entries < 0) {
		printf("You must enter a positive integer for these number, run \" ./web_server -h \" for help\n");
		return 0;
	}
	// Change the current working directory to server root directory
	chdir(path); // Changes the current directory to the specified path.
	// Start the server and initialize cache
	init(port);
	initCache();
	initQueue();

	//   Create dispatcher and worker threads
	pthread_t* workers;
	if ((workers = (pthread_t *)calloc(num_workers, sizeof(pthread_t))) == NULL) {
		perror("Failed to allocate space for Threads");
	}
	int worker_count = 0;
	for (worker_count = 0; worker_count < num_workers; worker_count++) {
		void * args;
		args = worker_count;
		pthread_create(workers + worker_count, NULL, worker, worker_count);
	}
	pthread_t* dispatchers;
	if ((dispatchers = (pthread_t *)calloc(num_dispatcher, sizeof(pthread_t))) == NULL) {
		perror("Failed to allocate space for Threads");
	}
	int dispatch_count = 0;
	for (dispatch_count = 0; dispatch_count < num_dispatcher; dispatch_count++) {
		pthread_create(dispatchers + dispatch_count, NULL, dispatch, dispatch_count);
	}

	for(int i = 0; i < num_dispatcher;i++) {
		if (pthread_equal(pthread_self(), dispatchers[i]))
			continue;
		if (pthread_join(dispatchers[i], NULL) == 0)
			printf("\n\nJoining dispatcher\n\n");
		else
			perror("\n\nfailed to join dispatcher...\n\n");
	}
	free(dispatchers);

	for(int i = 0; i < num_workers; i++) {
		if (pthread_equal(pthread_self(), workers[i]))
			continue;
		if (pthread_join(workers[i], NULL) == 0)
			printf("\n\nJoining working\n\n");
		else
			perror("\n\nfailed to join worker...\n\n");
	}
	free(workers);

	// Clean up
	deleteCache();
	deleteQueue();
	return 0;
}
