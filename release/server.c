#include <stdio.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "comm.h"
#include "util.h"

/* -----------Functions that implement server functionality -------------------------*/

/*
 * Returns the empty slot on success, or -1 on failure
 */
int find_empty_slot(USER * user_list) {
	// iterate through the user_list and check m_status to see if any slot is EMPTY
	// return the index of the empty slot
    int i = 0;
	for(i=0;i<MAX_USER;i++) {
    	if(user_list[i].m_status == SLOT_EMPTY) {
			return i;
		}
	}
	return -1;
}

/*
 * list the existing users on the server shell
 */
int list_users(int idx, USER * user_list)
{
	// iterate through the user list
	// if you find any slot which is not empty, print that m_user_id
	// if every slot is empty, print "<no users>""
	// If the function is called by the server (that is, idx is -1), then printf the list
	// If the function is called by the user, then send the list to the user using write() and passing m_fd_to_user
	// return 0 on success
	int i, flag = 0;
	char buf[MAX_MSG] = {}, *s = NULL;

	/* construct a list of user names */
	s = buf;
	strncpy(s, "---connecetd user list---\n", strlen("---connecetd user list---\n"));
	s += strlen("---connecetd user list---\n");
	for (i = 0; i < MAX_USER; i++) {
		if (user_list[i].m_status == SLOT_EMPTY)
			continue;
		flag = 1;
		strncpy(s, user_list[i].m_user_id, strlen(user_list[i].m_user_id));
		s = s + strlen(user_list[i].m_user_id);
		strncpy(s, "\n", 1);
		s++;
	}
	if (flag == 0) {
		strcpy(buf, "<no users>\n");
	} else {
		s--;
		strncpy(s, "\0", 1);
	}

	if(idx < 0) {
		printf(buf);
		printf("\n");
	} else {
		/* write to the given pipe fd */
		if (write(user_list[idx].m_fd_to_user, buf, strlen(buf) + 1) < 0)
			perror("writing to server shell");
	}

	return 0;
}

/*
 * add a new user
 */
int add_user(int idx, USER * user_list, int pid, char * user_id, int pipe_to_child, int pipe_to_parent)
{
	// populate the user_list structure with the arguments passed to this function
	USER* next_user = (USER*) malloc(sizeof(USER));
	next_user->m_pid = pid;
	strcpy(next_user->m_user_id,user_id);
	next_user->m_fd_to_server = pipe_to_parent;
	next_user->m_fd_to_user = pipe_to_child;
	int place_for_next_user  = find_empty_slot(user_list);
	user_list[place_for_next_user] = *next_user;
	// return the index of user added
	return place_for_next_user;
}

/*
 * Kill a user
 */
void kill_user(int idx, USER * user_list) {
	// kill a user (specified by idx) by using the systemcall kill()
	// then call waitpid on the user
}

/*
 * Perform cleanup actions after the used has been killed
 */
void cleanup_user(int idx, USER * user_list)
{
	// m_pid should be set back to -1
	// m_user_id should be set to zero, using memset()
	// close all the fd
	// set the value of all fd back to -1
	// set the status back to empty
}

/*
 * Kills the user and performs cleanup
 */
void kick_user(int idx, USER * user_list) {
	// should kill_user()
	// then perform cleanup_user()
}

/*
 * broadcast message to all users
 */
int broadcast_msg(USER * user_list, char *buf, char *sender)
{
	//iterate over the user_list and if a slot is full, and the user is not the sender itself,
	//then send the message to that user
	//return zero on success
	return 0;
}

/*
 * Cleanup user chat boxes
 */
void cleanup_users(USER * user_list)
{
	// go over the user list and check for any empty slots
	// call cleanup user for each of those users.
}

/*
 * find user index for given user name
 */
int find_user_index(USER * user_list, char * user_id)
{
	// go over the  user list to return the index of the user which matches the argument user_id
	// return -1 if not found
	int i, user_idx = -1;

	if (user_id == NULL) {
		fprintf(stderr, "NULL name passed.\n");
		return user_idx;
	}
	for (i=0;i<MAX_USER;i++) {
		if (user_list[i].m_status == SLOT_EMPTY)
			continue;
		if (strcmp(user_list[i].m_user_id, user_id) == 0) {
			return i;
		}
	}

	return -1;
}

/*
 * given a command's input buffer, extract name
 */
int extract_name(char * buf, char * user_name)
{
	char inbuf[MAX_MSG];
    char * tokens[16];
    strcpy(inbuf, buf);

    int token_cnt = parse_line(inbuf, tokens, " ");

    if(token_cnt >= 2) {
        strcpy(user_name, tokens[1]);
        return 0;
    }

    return -1;
}

int extract_text(char *buf, char * text)
{
    char inbuf[MAX_MSG];
    char * tokens[16];
    char * s = NULL;
    strcpy(inbuf, buf);

    int token_cnt = parse_line(inbuf, tokens, " ");

    if(token_cnt >= 3) {
        //Find " "
        s = strchr(buf, ' ');
        s = strchr(s+1, ' ');

        strcpy(text, s+1);
        return 0;
    }

    return -1;
}

/*
 * send personal message
 */
void send_p2p_msg(int idx, USER * user_list, char *buf)
{

	// get the target user by name using extract_name() function
	// find the user id using find_user_index()
	// if user not found, write back to the original user "User not found", using the write()function on pipes. 
	// if the user is found then write the message that the user wants to send to that user.
}

//takes in the filename of the file being executed, and prints an error message stating the commands and their usage
void show_error_message(char *filename)
{
}


/*
 * Populates the user list initially
 */
void init_user_list(USER * user_list) {

	//iterate over the MAX_USER
	//memset() all m_user_id to zero
	//set all fd to -1
	//set the status to be EMPTY
	int i=0;
	for(i=0;i<MAX_USER;i++) {
		user_list[i].m_pid = -1;
		memset(user_list[i].m_user_id, '\0', MAX_USER_ID);
		user_list[i].m_fd_to_user = -1;
		user_list[i].m_fd_to_server = -1;
		user_list[i].m_status = SLOT_EMPTY;
	}
}

/* ---------------------End of the functions that implementServer functionality -----------------*/
void pollSTDIN(char* cmd_buf, USER user_list[], int pipe_SERVER_writing_to_child[], int pipe_SERVER_reading_from_child[]){
	if(read(0, cmd_buf, MAX_CMD_LENGTH) > 0) {
//		pipe(pipe_SERVER_reading_from_child);
//		pipe(pipe_SERVER_writing_to_child);
		read(STDIN_FILENO, cmd_buf, MAX_CMD_LENGTH);
		if (cmd_buf != "") {
			printf("%s \n", cmd_buf);
		}
		printf("cmd_buf has a value of %s \n", cmd_buf);
		/**This block should trim any excess char. Make sure to take care of excess malloc **/
		char* tmp = (char*) malloc(strlen(cmd_buf)-1);
		strncpy(tmp, cmd_buf, strlen(cmd_buf)-1);
		// This value is the value that indicates what position in the commands array in util.c the command was
		// found at
		int found = find_command_type(tmp);
		//In this block the decision as to what the input meant is done here.
		/** List **/
		if(found == 0){
			printf("List command was entered");
			list_users(-1, user_list); //See definition of list_users, must be called with -1 to make sense.
			cmd_buf = "\0";
		}
			/** Kick **/
		else if(found ==1){
			print_prompt("admin");
			cmd_buf = "\0";
		}
			/** P2P **/
		else if(found ==2){
			print_prompt("admin");
			cmd_buf = "\0";
		}
			/** Seg **/
		else if(found ==3){
			print_prompt("admin");
			cmd_buf = "\0";
		}
		else if(found == 4){
			print_prompt("admin");
			cmd_buf = "\0";
		}
		else if(found == 5){
//			printf("No valid command was entered. Here is the list of acceptable commands for the admin: "
//			       "\\list, \\kick, \\p2p, \\seg, \\exit");
			printf("Broadcasting message to all users \n");
			close(pipe_SERVER_writing_to_child[0]);
			write(pipe_SERVER_writing_to_child[1], "hello", 6);
			cmd_buf = "\0";
		}
		//the commands array the command typed in was found at. We have a 100char limit on command strings.
		printf("%d \n", found);

//		close(pipe_SERVER_writing_to_child[0]); //Prevent reading from pipe while writing
//		write(pipe_SERVER_writing_to_child[1], cmd_buf, MAX_CMD_LENGTH);

		//Take care of the malloc we made to make the casting work, regardless of what the stdin term char is
		free(tmp);
		//Should always print the admin prompt before exiting to get ready for the next input
		print_prompt("admin");
	}
}

/* ---------------------Start of the Main function ----------------------------------------------*/
int main(int argc, char * argv[])
{
	int nbytes;
	setup_connection("YOUR_UNIQUE_ID"); // Specifies the connection point as argument.

	USER user_list[MAX_USER];
	init_user_list(user_list);   // Initialize user list

	char buf[MAX_MSG]; 
	fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
	print_prompt("admin");



	//
	while(1) {
		/* ------------------------YOUR CODE FOR MAIN--------------------------------*/

		// Handling a new connection using get_connection
		int pipe_SERVER_reading_from_child[2];
		int pipe_SERVER_writing_to_child[2];

		int pipe_child_reading_from_server[2];
		int pipe_child_writing_to_server[2];

		char cmd_buf[MAX_CMD_LENGTH] = {"\0"};
		char user_id[MAX_USER_ID];
		char msg_buf[MAX_MSG];

		/** If we find a new connection, then we have a server and child process to take care of.**/
		if(get_connection(user_id, pipe_child_reading_from_server, pipe_child_writing_to_server) != -1) {
			pipe(pipe_SERVER_reading_from_child);
			pipe(pipe_SERVER_writing_to_child);
			int pid = fork();
			/** We want to make sure this user list is known to the server and the child process equally**/
			// Check max user and same user id
			int index_of_user = find_user_index(user_list, user_id);
			printf("%d\n", index_of_user);
			if(index_of_user < 0){
				int index_of_new_user = find_empty_slot(user_list);
				printf("%d\n", index_of_new_user);
				add_user(index_of_new_user, user_list, getpid(), user_id, pipe_SERVER_reading_from_child,
						pipe_SERVER_writing_to_child);
				list_users(-1,user_list); //When list users is called by negative one, it indicates the server
				//asking for the information.
				print_prompt("admin");
			}
			/** 																						**/
			// Child process:
			if (pid == 0) {
				while(1) {
//			poll users and SERVER
					char tx_buf[MAX_MSG];

//					printf("I am the child process and was called by user: %s\n", user_id);
					//This will retrieve information from the client to the child process
					close(pipe_child_writing_to_server[1]);
//					fcntl(pipe_child_writing_to_server[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
					if (read(pipe_child_writing_to_server[0], tx_buf, MAX_MSG) > 0){
						printf("Message received from client: %s\n", tx_buf);
						open(pipe_child_writing_to_server[1], O_WRONLY);

						//Write the message received from the client, that is now in the child, to the server
						close(pipe_SERVER_reading_from_child[0]);
						write(pipe_SERVER_reading_from_child[1], tx_buf, MAX_MSG);
						printf("Wrote to the server\n");
						open(pipe_SERVER_reading_from_child[0], O_RDONLY);
					}

					printf("Exiting the read client process\n");

					//Read from the server process trying to talk to the client
//					close(pipe_child_reading_from_server[1]);
					char tmp[MAX_MSG];
//					fcntl(pipe_SERVER_writing_to_child[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
					if(read(pipe_child_reading_from_server[0], tmp, MAX_MSG) > 0) {
						if (tmp != " ") {
							printf("Child read %s from the server\n", tmp);
							//This should write to the client, note that the "pipe_SERVER_writing_to_child" now writes to client
							close(pipe_SERVER_writing_to_child[0]);
							write(pipe_SERVER_writing_to_child[1], tmp, MAX_MSG);
							printf("Child wrote %s to the client\n", tmp);
							close(pipe_SERVER_writing_to_child[1]);
							open(pipe_SERVER_writing_to_child[0], O_RDONLY);
						}
					}
				}
			}

				// Server process:
			if(pid>0){
				while(1) {
					char tx_buf[MAX_MSG];
					char rx_buf[MAX_MSG];
//				fcntl(pipe_SERVER_reading_from_child[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);

//				printf("Got here in the server process. Called by user: %s\n", user_id);
					close(pipe_SERVER_reading_from_child[1]);
					read(pipe_SERVER_reading_from_child[0], rx_buf, MAX_MSG);
					printf("Server rx from child: %s\n", rx_buf);
					open(pipe_SERVER_reading_from_child[1], O_WRONLY);

					/** In this block, we are writing from the server to the child **/

					char stdin[MAX_MSG] = {"\0"};
					read(0,stdin, MAX_MSG);
					close(pipe_child_reading_from_server[0]);
					write(pipe_child_reading_from_server[1], stdin, MAX_MSG);
					open(pipe_child_reading_from_server[0], O_RDONLY);
//					char test[MAX_MSG] = {"hello"};
					write(pipe_child_reading_from_server[1], stdin, MAX_MSG);
					printf("pipe to child has contents: %s\n", stdin);

					/**															**/

					// Add a new user information into an empty slot
					// poll child processes and handle user commands

					// Poll stdin (input from the terminal) and handle admnistrative command
//				pollSTDIN(cmd_buf, user_list, pipe_SERVER_writing_to_child ,pipe_SERVER_reading_from_child);

					//Print the prompt at the end of a command
					print_prompt("admin");
				}
			}
			else{
				perror("pipe failed\n");
				exit(-1);
			}
		}

		/** If we do not have a new user, then these actions will take place **/


//		pollSTDIN(cmd_buf, user_list, pipe_SERVER_writing_to_child ,pipe_SERVER_reading_from_child);
	
		/* ------------------------YOUR CODE FOR MAIN--------------------------------*/
	}
}

/* --------------------End of the main function ----------------------------------------*/
