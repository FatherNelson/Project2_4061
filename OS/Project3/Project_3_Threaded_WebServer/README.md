/* CSci4061 Fall 2018 Project 3
* Name: Grant James Udstrand, Jacob Coenen, Kieran Lieberg
* X500: udstr005, coene035, liebe110 */

1. How to compile and run
Compile the program with the following series of commands:
make util
make

To run the server, type the following command:
./web_server <port> <path_to_testing>/testing <num_dispatch> <num_worker> 0 <queue_len> <cache_entries>
where port is the unique desired port number, num_dispatch and num_worker are
the number of dispatch and worker threads, queue_len is an arbitrary integer,
and cache_entries is the maximum number of entries in the cache.

Queue_len is ignored because we decided to always make the queue maximum size
anyway to minimize the possibility of it from running out.   

To get data from the server, simply use wget or a similar command such as curl,
with a URL such as http://127.0.0.1:<port>/image/jpg/29.jpg, or enter the
appropriate URL into your browser.

2. Brief Explanation of operation
The program creates "dispatch" threads, which accept incoming requests and add
them into a queue, and "worker" threads, which pop items out of the queue and
service their requests.  The queue is implemented using rolling pointers to the
start and end of the data in an array, with a modulo operation to allow the
pointers to wrap around within the array.  When a piece of data is requested, a
worker searches the cache for it.  If in cache, the data is loaded from there.
If not, the data is loaded from disk and then copied to cache so that future
requests for that data don't require the server to load things from disk.  The
least frequently used cache entry is replaced if the cache is of greater size
than specified in the initial args.  The server and threads run until
interrupted with ctrl-C.  However, infrastructure is in place to rejoin the
threads and clean up the program if we wanted to change this behavior.

3. Explanation of caching mechanism used
The cache, to avoid having a hard limit on size, was made to be a dynamically allocated cache. That is,
when a new request was made, the cache would store a new resource within the cache and on subsequent requests
the response would originate from the randomly populated cache rather than via a disk lookup.
So, for every new request a new cache entry was made, thus avoiding a hard upper bound.
There was an LFU implementation where before every addition to the cache, there was an attempt to sort the most queried
cache requests at the front of the cache. This had the dual effect of making lookup times for frequent searches
faster as well as allowing easy replacement of entries at the end of the array.

4. Explanation of policy to dynamically change the worker thread pool size
N/A

5. Contributions of Team members:

Grant: Wrote helper functions, error handlers,
designed caching mechanism, Wrote the single threaded version of the program before the
other group members implemented a multithreadable strategy. Managed tasks and architecture.
Implemented cache sorting algorithm.

Jake: Implemented request queue, implemented mutexes on queue and log file,
dealt with memory cleanup, debugged code, helped implement multithreaded program,
implemented cache replacement

Kieran: Implemented mutex on cache, implemented thread joining, performed
analytics, extensive debugging
