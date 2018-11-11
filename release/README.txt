/* CSci4061 F2018 Assignment 2
* login: liebe110@umn.edu
* date: 11/10/18
* name: Grant James Udstrand, Kieran Lieberg, Jacob Coenen
* id: 5262811, id: 5241931, id 5177976
* Workload Distribution:
*  Grant: Handled creation of server children, and got basic communication
*     between the server and children.
*
*  Jake: Implemented the commands for the clients, i.e p2p, list, exit.
*
*  Kieran: Implemented the commands for the server, i.e list, kick, and exit.
*
*  The goal of this assignment was to create a chat room for different clients
*  from the same computer to communicate. 
*
*  The program can be compiled using gcc, using the provided Makefile.
*  There is one server, and multiple clients connect to that server.
*  To start the server, run the server executable provided.
*  To start a client, open up a new terminal, and call the client executable
*  with a username as the first argument. If no argument is passed, or a very long
*  username is entered, unexpected behavior may occur.
*  
*  Noncommands typed into the server terminal will be broadcast to each client. 
*  Noncommands typed into the client will also be broadcast to each other client.
*   
*  Multiple errors can occur during the use of this chat system. Most of these will
*  be handled by our code, however there are a few exceptions.
*
*  Errors produced by system calls are all handled properly. For example, if not enough
*  memory is available to fork, an error message will be outputted, and the child proccess 
*  code will not be run.
*
*  Errors produced by client connections are not handled. If a user control C's, or closes
*  their terminal, the server will not detect it. The server will continue to run, but will
*  not be aware of the client's disconnection. If the server control C's, the client also
*  will be unaware that the server is no longer running. 
*/
