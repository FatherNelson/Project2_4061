#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include "comm.h"
#include "util.h"
/* -----------Functions that implement server functionality -------------------------*/

/*
 * Returns the index of th first empty slot in the user list on success, or -1 on failure
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
  // If the function is called by the server (that is, idx is -1), then printf the list
	// If the function is called by the user, then send the list to the user using write() and passing m_fd_to_user
	int i, flag = 0;
	char buf[MAX_MSG] = {}, *s = NULL;

	/* construct a list of user names */
	s = buf;
	strncpy(s, "---connected user list---\n", strlen("---connected user list---\n"));
	s += strlen("---connected user list---\n");
	for (i = 0; i < MAX_USER; i++) {
    // iterate through the user list
  	// if you find any slot which is not empty, print that m_user_id
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
    // if every slot is empty, print "<no users>""
	} else {
		s--;
		strncpy(s, "\0", 1); //Place Null character at end of string buffer.
	}

	if(idx < 0) {
		printf(buf);
		printf("\n");
	} else {
		/* write to the given pipe fd */
		if (write(user_list[idx].m_fd_to_server, buf, strlen(buf) + 1) < 0)
			perror("failed to write user_list to server shell");
	}

	return 0; 	// return 0 on success
}

/*
 * add a new user
 */
int add_user(int idx, USER * user_list, int pid, char * user_id, int pipe_to_user, int pipe_to_server)
{
	// populate the user_list structure with the arguments passed to this function
	USER* next_user = (USER*) malloc(sizeof(USER));
	next_user->m_pid = pid;
	strcpy(next_user->m_user_id,user_id);
	next_user->m_fd_to_server = pipe_to_server;
	next_user->m_fd_to_user = pipe_to_user;
	int place_for_next_user = find_empty_slot(user_list);  //if for example someone leaves the server, we may want the
                                                         //next person who enters to take their spot in the user list, instead
                                                         //of appending them to the end.
	user_list[place_for_next_user] = *next_user;
	// return the index of user added
	return place_for_next_user;
}

/*
 * Kill a user
 */
void kill_user(int child_idx, USER * user_list) {
	// kill a user (specified by idx) by using the systemcall kill()
	USER dude = user_list[child_idx];
	printf("Killing ID: %s\n", dude.m_user_id);
	int pid_to_kill = dude.m_pid;
	char buf[10];
	sprintf(buf, "%d", pid_to_kill);
	if (kill(pid_to_kill, SIGKILL) < 0) {
		printf("Failed to kill the selected USER\n");
	}; // The system call to kill the user client
	int status;
	waitpid(pid_to_kill, &status, 0);
	printf("Killed client process: %d\n", pid_to_kill);
	if (kill(pid_to_kill-1, SIGKILL) < 0) {
		printf("Failed to kill the child proecess\n");
	}; // The system call to kill the child process
	int status2;
	waitpid(pid_to_kill-1, &status, 0);
	printf("Killed child process: %d\n", pid_to_kill-1);
}
/*
 * Perform cleanup actions after the user has been killed
 */
void cleanup_user(int child_idx, USER * user_list)
{
	USER user_cleaning = user_list[child_idx];
	printf("%s\n", user_list[child_idx].m_user_id);
	// m_pid should be set back to -1
	user_cleaning.m_pid = -1;
	// m_user_id should be set to zero, using memset()
	memset(user_cleaning.m_user_id, 0, sizeof(user_cleaning.m_user_id));
	// close all the fd
	close(user_cleaning.m_fd_to_server);
	close(user_cleaning.m_fd_to_user);
	// set the value of all fd back to -1
	user_cleaning.m_fd_to_user =  -1;
	user_cleaning.m_fd_to_server = -1;
	// set the status back to empty
	user_cleaning.m_status = SLOT_EMPTY; //Set it to the empty val
	user_list[child_idx] = user_cleaning;
}

/*
 * Kills the user and performs cleanup
 */
void kick_user(int user_idx, USER * user_list) {
	// should kill_user()
	kill_user(user_idx, user_list);
	// then perform cleanup_user()
	cleanup_user(user_idx, user_list);
}

/*
 * broadcast message to all users
 */
int broadcast_msg(USER * user_list, char *buf, char *sender)
{
	//iterate over the user_list and if a slot is full, and the user is not the sender itself,
	//then send the message to that user
	//return zero on success
  for (int i = 0; i < MAX_USER; i++) {
    if (user_list[i].m_status == SLOT_EMPTY) { // Reached the last user
      continue;
    }
    else {
      write(user_list[i].m_fd_to_user, buf, MAX_CMD_LENGTH);
    }
  }
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
	int i;

	if (user_id == NULL) {
		fprintf(stderr, "NULL name passed.\n");
		return -1;
	}
	for (i=0;i<MAX_USER;i++) {
		if (user_list[i].m_status == SLOT_EMPTY)
			continue;
		if (strcmp(user_list[i].m_user_id, user_id) == 0) {
			return i;
		}
	}
  fprintf(stderr, "No user found with user_id: %s\n", user_id);
	return -1;
}

/*
 * given a command's input buffer, extract the name into user_name
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

/*
 * given a message input buffer, extract the text into text parameter
 */
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

// parse_message parses, and executes the commmand given in cmd_buf. If no command is given, it broadcast the message
// idx is the index of the user who sent the message, or -1 if the server sent the message
void parse_message(char* cmd_buf, USER user_list[], int idx){
		/** Wrap this section as a parsing function **/
		char* arg_array[MAX_MSG] = {"\0"}; //Holds the arguments we are about to split.
		char* pch; //This holds the split string, imagine it as another arg array
    char tmp[MAX_MSG];
    strcpy(tmp, cmd_buf);
		pch = strtok (tmp," ");
		int length_of_cmd_array = 0; // This tells us how many arguments are in the string.
		while (pch != NULL)
		{
			if(pch != "\n") {
//				printf("%s\n", pch);
				arg_array[length_of_cmd_array] = pch;
//				printf("%s\n", arg_array[length_of_cmd_array]);
				pch = strtok(NULL, " \n"); //Trims all spaces and newlines, should now have an arg array in pch.
				if(pch != "") {
					char *tmp = (char *) malloc(strlen(arg_array[length_of_cmd_array]));
					strncpy(tmp, arg_array[length_of_cmd_array], strlen(arg_array[length_of_cmd_array])); //Trims new lines
					arg_array[length_of_cmd_array] = tmp;
					length_of_cmd_array += 1;
				}
			}
		}

		//Sanity check in case of array only being one word long.
		if(length_of_cmd_array == 1){
//			printf("length of cmd array was one\n");
			char tmp[MAX_CMD_LENGTH] = {"\0"};
			strncpy(tmp, arg_array[0], strlen(arg_array[0])-1); //Trims new lines
			arg_array[0] = tmp;
			printf("array is one long: %s", arg_array[0]);
		}
		/**This block should trim any excess char, specifically, the trailing newline.
		 * Make sure to take care of excess malloc **/
		for(int i = 0; i < length_of_cmd_array; i++) {
			printf("Position %d: %s\n", i, arg_array[i]);
		}

	// This value is the value that indicates what position in the commands array in util.c the command was
		// found at
		int found = find_command_type(arg_array[0]);
		//In this block the decision as to what the input meant is done here.
    switch (found){
  		case LIST:
        /** List **/
  			printf("List command was entered");
  			list_users(-1, user_list); //See definition of list_users, must be called with -1 to make sense.
        break;
   		case KICK:
        /** Kick **/
   			printf("Kick command was entered for user: %s\n",arg_array[1]);
        int child_idx = find_user_index(user_list, arg_array[1]);
        if (child_idx != -1)
   			  kick_user(child_idx, user_list);
         break;
   		case P2P:
         /** P2P **/
         break;
   		case SEG:
         /** Seg **/
         break;
   		case EXIT:
         /** Exit **/
         break;
   		default:
         /** No Command Entered **/
   			printf("Broadcasting message to all users \n"); //when no command is entered,
                                                        //broadcast the message to everyone
        broadcast_msg(user_list, cmd_buf, "admin");
	 	}
		//Should always print the admin prompt before exiting to get ready for the next input
    cmd_buf = "\0";
		print_prompt("admin");
}

/* ---------------------Start of the Main function ----------------------------------------------*/
int main(int argc, char * argv[])
{
	int nbytes;
	setup_connection("Project2_59"); // Specifies the connection point as argument.

	USER user_list[MAX_USER];
	init_user_list(user_list);   // Initialize user list

	char buf[MAX_MSG];
	fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
	print_prompt("admin");




	//
	int pipe_SERVER_reading_from_child[2];
	int pipe_SERVER_writing_to_child[2];

	int pipe_child_reading_from_server[2];
	int pipe_child_writing_to_server[2];

	char cmd_buf[MAX_CMD_LENGTH] = {"\0"};
	char user_id[MAX_USER_ID];
	char msg_buf[MAX_MSG];
	while(1) {
		usleep(SLEEP_TIME);
		/* ------------------------YOUR CODE FOR MAIN--------------------------------*/

		/**Server Process**/
		//This is the server process, will run all the time
		usleep(SLEEP_TIME);
		char tx_buf[MAX_MSG];
		char rx_buf[MAX_MSG];
		char stdin[MAX_MSG] = {"\0"};
		fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //Makes reads of stdin non-blocking
		fcntl(pipe_SERVER_reading_from_child[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //Makes reads of stdin non-blocking
		// Add a new user information into an empty slot
		// poll child processes and handle user commands
//					close(pipe_SERVER_reading_from_child[1]);

    //for (int i = 0; i < MAX_USER; i++) {
      ///if(read(user_list[i].m_fd_to_server, rx_buf, MAX_MSG > 0)) {
      //  printf("I suk: %s\n", rx_buf);
      //}
    //}

		if(read(pipe_SERVER_reading_from_child[0], rx_buf, MAX_MSG) > 0) {
			printf("Server received from child: %s\n", rx_buf);
//						open(pipe_SERVER_reading_from_child[1], O_WRONLY);
			print_prompt("admin");
		}
		// Poll stdin (input from the terminal) and handle admnistrative command
		if(read(STDIN_FILENO,stdin, MAX_MSG) > 0) { //If there is data in stdin, put it into a buffer
			printf("Server recceived from STDIN%s\n", stdin);
			parse_message(stdin, user_list, -1);
		}


		// Handling a new connection using get_connection
		/** If we find a new connection, then we have a server and child process to take care of.**/
		if(get_connection(user_id, pipe_child_reading_from_server, pipe_child_writing_to_server) != -1) {
			printf("Added a new user!\n");
			pipe(pipe_SERVER_reading_from_child);
			pipe(pipe_SERVER_writing_to_child);
			int index_of_user = find_user_index(user_list, user_id);
			int index_of_new_user;
			printf("%d\n", index_of_user);
			if(index_of_user < 0){
				index_of_new_user = find_empty_slot(user_list);
				printf("%d\n", index_of_new_user);
			}
			/** only stable version so far has this bloc of non-blocking statements, will keep until a better strat **/
			fcntl(pipe_child_writing_to_server[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_child_writing_to_server[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_SERVER_writing_to_child[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_SERVER_writing_to_child[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_child_reading_from_server[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_child_reading_from_server[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_SERVER_reading_from_child[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(pipe_SERVER_reading_from_child[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
			fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);

			/**																											**/
			int pid = fork();
			if(pid > 0) { // Should give the pid to the parent process

				/*It is important to note that the actual client process always will have a value one lower than the
				 * value of getpid() here, so we must decide either to kill just the child process or kill both. Ideally
				 * We probably should kill the child and as part of the death procedures after killing the child to stop
				 * the messages, we should then axe the client.
				 * */

				printf("I am a child process with pid: %d. I am at index: %d\n", pid, index_of_new_user);
				/** These users should be children or clients? **/
				add_user(index_of_new_user, user_list, pid, user_id, pipe_SERVER_writing_to_child[1], pipe_SERVER_reading_from_child[0]);
				list_users(-1, user_list); //When list users is called by negative one, it indicates the server
				//asking for the information. The last argument turns on or off verbose mode.
				print_prompt("admin");
			}
			/** We want to make sure this user list is known to the server and the child process equally**/
			// Check max user and same user id
			/** 																						**/
			// Child process:
			if (pid == 0) {
				while(1) {
					usleep(SLEEP_TIME);
//			poll users and SERVER
					//Read from the SERVER process trying to talk to the client
//					close(pipe_child_reading_from_server[1]);
					char tmp[MAX_MSG];
//					fcntl(pipe_SERVER_writing_to_child[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);


//					fcntl(pipe_SERVER_writing_to_child[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);
//					fcntl(pipe_SERVER_reading_from_child[1], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK);

					if(read(pipe_SERVER_writing_to_child[0], tmp, MAX_MSG) > 0){
						printf("The child process received: %s\n", tmp);
//						open(pipe_child_reading_from_server[1], O_WRONLY);
//						close(pipe_child_reading_from_server[0]);
						write(pipe_child_reading_from_server[1], tmp, MAX_MSG);
//						open(pipe_child_reading_from_server[0], O_RDONLY);
//						close(pipe_child_reading_from_server[1]);
						print_prompt("admin");
					}
					char tx_buf[MAX_MSG];

					if (read(pipe_child_writing_to_server[0], tx_buf, MAX_MSG) > 0){
						printf("Child received from client: %s\n", tx_buf);
//						open(pipe_child_writing_to_server[1], O_WRONLY);
//						//Write the message received from the client, that is now in the child, to the server
//						close(pipe_SERVER_reading_from_child[0]);
						write(pipe_SERVER_reading_from_child[1], tx_buf, MAX_MSG);
						printf("Child wrote to the server\n");
//						open(pipe_SERVER_reading_from_child[0], O_RDONLY);
						print_prompt("admin");
					}

				}
			}
			/** The server process at one time was part of the fork and this was wrong, now we want it to be the
			 * identity of main and to have many children, as is the setup now.
			 */

				// Server process:
//			if(pid>0){
//				while(1) {
//					usleep(SLEEP_TIME);
//					char tx_buf[MAX_MSG];
//					char rx_buf[MAX_MSG];
//					char stdin[MAX_MSG] = {"\0"};
//					fcntl(0, F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //Makes reads of stdin non-blocking
//					fcntl(pipe_SERVER_reading_from_child[0], F_SETFL, fcntl(0, F_GETFL)| O_NONBLOCK); //Makes reads of stdin non-blocking
//					// Add a new user information into an empty slot
//					// poll child processes and handle user commands
////					close(pipe_SERVER_reading_from_child[1]);
//					if(read(pipe_SERVER_reading_from_child[0], rx_buf, MAX_MSG) > 0) {
//						printf("Server received from child: %s\n", rx_buf);
////						open(pipe_SERVER_reading_from_child[1], O_WRONLY);
//						print_prompt("admin");
//					}
//
//					// Poll stdin (input from the terminal) and handle admnistrative command
//					/** In this block, we are polling the input **/
//
//					if(read(STDIN_FILENO,stdin, MAX_MSG) > 0) { //If there is data in stdin, put it into a buffer
//						printf("%s\n", stdin);
//						pollSTDIN(stdin, user_list, pipe_SERVER_writing_to_child, pipe_SERVER_reading_from_child);
//						/** This block executes if told to broadcast **/
//					}
//				}
//			}
//			else{
//				perror("pipe failed\n");
//				exit(-1);
//			}
		}


		/** If we do not have a new user, then these actions will take place **/


//		pollSTDIN(cmd_buf, user_list, pipe_SERVER_writing_to_child ,pipe_SERVER_reading_from_child);

		/* ------------------------YOUR CODE FOR MAIN--------------------------------*/
	}
}

/* --------------------End of the main function ----------------------------------------*/
