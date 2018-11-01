/* CSci4061 F2018 Assignment 1
* login: cselabs login name (login used to submit)
* date: mm/dd/yy
* name: full name1, full name2, full name3 (for partner(s))
* id: id for first name, id for second name, id for third name */

// This is the main file for the code
#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
/*-------------------------------------------------------PROGRAM CONSTANTS---------------------------------*/
#define MAX_DEPENDENCIES 10
/*-------------------------------------------------------GLOBAL VARIABLES---------------------------------*/
target_t *targets_ext;
/*-------------------------------------------------------HELPER FUNCTIONS PROTOTYPES---------------------------------*/
void show_error_message(char * ExecName);

void show_targets(target_t targets[], int nTargetCount);
/*-------------------------------------------------------END OF HELPER FUNCTIONS PROTOTYPES--------------------------*/


/*-------------------------------------------------------HELPER FUNCTIONS--------------------------------------------*/

//This is the function for writing an error to the stream
//It prints the same message in all the cases
void show_error_message(char * ExecName)
{
	fprintf(stderr, "Usage: %s [options] [target] : only single target is allowed.\n", ExecName);
	fprintf(stderr, "-f FILE\t\tRead FILE as a makefile.\n");
	fprintf(stderr, "-h\t\tPrint this message and exit.\n");
	exit(0);
}

//Phase1: Warmup phase for parsing the structure here. Do it as per the PDF (Writeup)
void show_targets(target_t targets[], int nTargetCount)
{
	//Write your warmup code here
    printf("%d", nTargetCount);
    printf("\n");
    int i =0;
    for(i = 0; i < nTargetCount; i++) {
        target_t target_view = targets[i];
        printf("<-------- START OF TARGET --------> \n");
        printf("Target Number : %d \n", i);
        printf("Target name: %s \n", target_view.TargetName);
        printf("Dependency Count: %d \n", target_view.DependencyCount);
        printf("Dependency Names: %s \n", target_view.DependencyNames);
        printf("Command: %s \n", target_view.Command);
        printf("Status: %d \n", target_view.Status);
        printf("<-------- END OF TARGET --------> \n \n");
    }
    return;
}

// Write your functions here
// Logs an error message when a target name is not found in the given makefile
void show_targets_error(char* ExecName){
	fprintf(stderr, "Target %s does not exist. Please enter a valid build target \n", ExecName);
	exit(0);
}

//This function makes you switch the target you are looking at. This is dependent on our implementation so we may
//hard code the vector as we did here.
void change_target(char* dependency, char* Makefile){
	char *file = "./make4061";
	char *argv[6];
	argv[0] = "./make4061";
	argv[1] = "-f";
	argv[2] = Makefile;
	argv[3] = dependency;
	argv[4] = NULL;

	if(execvp(file, argv)<0){ // Attempts to execute make4061 with new target
		printf("exec failed");
	}
	exit(0);
}

//Separates a command string into an array of strings. Base is the array we want split, target is the array we are
//inserting these commadns into.  Returns number of words in the command string
int getWords(char *base, char target[10][20])
{
	int n=0,i,j=0;

	for(i=0;TRUE;i++)
	{
		if(base[i]!=' '){
			target[n][j++]=base[i];
		}
		else{
			target[n][j++]='\0';//insert NULL
			n++;
			j=0;
		}
		if(base[i]=='\0')
			break;
	}
	return n;
}

//Once you run out of dependencies, you execute the command for the parent node. This function will execute
//the target's command
int execute_command(target_t targets[], int array_pos, char* cmd){
	int n; //number of words
	int i; //loop counter
	int ctime; //Gives the last modification datetime of the c file
	int otime; //Gives the last modification datetime of the o file
	char *str= cmd; //The actual command passed to us after verifying the other files are built.
	char arr[10][20] = {'\0'}; //Initialized with null chars to ensure the program memory is empty.

	n=getWords(str,arr); //Parse the command string into digestible chunks so that we can call its' command.

	const char *file = arr[0];
	char *argv[MAX_DEPENDENCIES];
	for(i = 0; i <= n; i++){
		if(strlen(arr[i]) > 0){
			argv[i] = arr[i];
		}
	}
	argv[i] = NULL;
	char *target = (char*) targets[array_pos].TargetName; // Set the target for time comparisons (target we are building)
	int comparison = 0; // Check to see if the build command must be run.
	for(int i = 0; i < targets[array_pos].DependencyCount; i++){
		comparison = compare_modification_time(target, targets[array_pos].DependencyNames[i]);
		if(comparison == 2){
			break;
		}
	}
	if(comparison == 2 || !does_file_exist(target)) {
		printf("Executing command: ");
		for(i=0;i<=n;i++) {
			printf("%s ", arr[i]);
		}
		printf("\n");
		execvp(file, argv);

		printf("exec failed\n");
		printf("errno is %s\n", strerror(errno));
	}
	exit(0);
}
// A function to check the dependency list of any node given the list of targets
// and where the current node being checked is in the graph.  Executes the command
// if there are no dependencies.
int check_dependency_list(target_t targets[], int array_pos, char* Makefile, int nTargetCount){
	for (int j = 0; j < MAX_DEPENDENCIES; j++) {
		// If we are at the end of the dependency list
		if (strlen(targets[array_pos].DependencyNames[j]) < 1) {
			char *cmd = targets[array_pos].Command;
			pid_t childpid,wpid;
			int status = 0;
			childpid = fork();
			if (childpid == -1) {
				printf("Target %s failed to fork.\n", targets[array_pos].TargetName);
				exit(1);
			}
			if (childpid == 0) {
				execute_command(targets, array_pos, cmd);
				exit(0); //Return from the child process
			}
			else {
				int status;
				wait(&status);
				if(status > 0){
					printf("The command %s for target %s failed with status code  %d.\n", cmd, targets[array_pos].TargetName,
							status);
					exit(1);
				}
			}
			break;
		}
//			In the case there are more dependencies, change the node we are looking at and evaluate it for dependencies
		else {
			printf("Building target: %s \n", targets[array_pos].DependencyNames[j]);
			pid_t childpid, wpid;
			int status = 0;
			childpid = fork();
			if (childpid == -1) {
				printf("Target %s failed to fork.\n", targets[array_pos].TargetName);
				exit(1);
			}
			if (childpid == 0) {
				char *dependency = (char*)targets[array_pos].DependencyNames[j];
				change_target(dependency, Makefile);
				exit(0); //Return from the child process
			}
			else {
				int status;
				wait(&status);
				if(status > 0){
					exit(1);
				}
			}
		}
	}
	return 0;
}

//This function will go through the DAG and attempt to find the target that was called for in the command line
//If successful, it will call check_dependency_list for that target.
void check_target_list(char* TargetName,target_t targets[], int nTargetCount, char* Makefile){
// Check for where it is in the build file, if the target is not in the build file, complain
	for(int i = 0; i < nTargetCount; i++){
		//If the target to start at matches this target, check the dependencies here
		if(strcmp(targets[i].TargetName, TargetName) == 0){
			check_dependency_list(targets,i, Makefile,nTargetCount);
			return;
		}
			/* If there is a problem with the target name, complain*/
		else if(strcmp(targets[i].TargetName, TargetName) != 0 && i == nTargetCount){
			show_targets_error(TargetName);
			return;
		}
	}
}

/*-------------------------------------------------------END OF HELPER FUNCTIONS-------------------------------------*/


/*-------------------------------------------------------MAIN PROGRAM------------------------------------------------*/
int main(int argc, char *argv[])
{
	target_t targets[MAX_NODES];
	int nTargetCount = 0;

  char Makefile[64] = "Makefile";
  char TargetName[64];						// The target we are currently executing on

  /* Declarations for getopt */
  extern int optind;
  extern char * optarg;
  int ch;
  char *format = "f:h";
  char *temp;

  /* Checks if a file/target was set, 1 if true, 0 if false */
  int fileSet = 0;
  int targetSet = 0;

  //Getopt function is used to access the command line arguments. However there can be arguments
  // which may or may not need the parameters after the command
  //Example -f <filename> needs a finename, and therefore we need to give a colon after that sort of argument
  //Ex. f: for h there won't be any argument hence we are not going to do the same for h, hence "f:h"
  while((ch = getopt(argc, argv, format)) != -1)
  {
	  switch(ch)
	  {
	  	  case 'f':
	  		  temp = strdup(optarg);
	  		  strcpy(Makefile, temp);  // here the strcpy returns a string and that is later copied using the strcpy
	  		  free(temp);	//need to manually free the pointer
	  		  fileSet = 1; //Sets variable that lets us know a file was chosen
	  		  break;

	  	  case 'h':
	  	  default:
	  		  show_error_message(argv[0]);
	  		  exit(1);
	  }

  }

  argc -= optind;
  if(argc > 1)   //Means that we are giving more than 1 target which is not accepted
  {
	  show_error_message(argv[0]);
  }

  /* Init Targets */
  memset(targets, 0, sizeof(targets));   //initialize all the nodes first, just to avoid the valgrind checks

   //Parse graph file or die, This is the main function to perform the toplogical sort and hence populate the structure
  if((nTargetCount = parse(Makefile, targets)) == -1)  //here the parser returns the starting address of the array of the structure. Here we gave the makefile and then it just does the parsing of the makefile and then it has created array of the nodes
	  return -1;



  //Phase1: Warmup-----------------------------------------------------------------------------------------------------
  /* Comment out the following line before Phase2 */
//  show_targets(targets, nTargetCount);
  //End of Warmup------------------------------------------------------------------------------------------------------

  /*
   * Set Targetname
   * If target is not set, set it to default (first target from makefile)
   */
  if(argc == 1) {
      strcpy(TargetName, argv[optind]);
      targetSet = 1;
  }
  else {
	  strcpy(TargetName, targets[0].TargetName);  // defaults to first target
  }
  /*
   * Now, the file has been parsed and the targets have been named.
   * You'll now want to check all dependencies (whether they are
   * available targets or files) and then execute the target that
   * was specified on the command line, along with their dependencies,
   * etc. Else if no target is mentioned then build the first target
   * found in Makefile.
   */

  //Phase2: Begins ----------------------------------------------------------------------------------------------------
  /** If a build file is provided**/
  if(fileSet == 1){
	  if(targetSet){
			// Find the target passed in, then check its dependencies
		  check_target_list(TargetName, targets, nTargetCount, Makefile);
	  }
	  else {
			// Default to first target
			check_dependency_list(targets, 0, Makefile, nTargetCount);
	  }
  }

  /** If no build file is provided **/
  else{
  	if(targetSet){
			//Check for where it is in the build file, if the target is not in the build file, complain
			for(int i = 0; i < nTargetCount; i++){
				//If the target to start at matches this target, check the dependencies here
				if(strcmp(targets[i].TargetName, TargetName) == 0){
					check_dependency_list(targets,i, Makefile,nTargetCount);
				}
				// If there is a problem with the target name, complain
				else if(strcmp(targets[i].TargetName, TargetName) != 0 && i == nTargetCount-1){
					show_targets_error(TargetName);
					break;
				}
	  	}
		}
  	else {
			check_dependency_list(targets, 0, Makefile, nTargetCount);

    }
  }

  /*End of your code*/
  //End of Phase2------------------------------------------------------------------------------------------------------

  return 0;
}
/*-------------------------------------------------------END OF MAIN PROGRAM------------------------------------------*/
