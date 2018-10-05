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

//Phase1: Warmup phase for parsing the structure here.
//Show targets will print out the 4 fields of each node in the DAG
void show_targets(target_t targets[], int nTargetCount)
{
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

//Logs an error message. Called when a target name is not found in the given makefile
void show_targets_error(char* ExecName){
	fprintf(stderr, "Target %s does not exist. Please enter a valid build target \n", ExecName);
	exit(0);
}

//This function checks the extensions of files, i.e., will give you .o or .c designation.
char* check_extension(char *file_to_check){
	char *ext;
	ext = strrchr(file_to_check, '.');
	if (!ext) {
		/* no extension */
		printf("this file has no extension\n");
		return NULL;
	}
	else {
		printf("extension is %s\n", ext + 1);
		return ext+1;
	}
}
/*
//***Checks the timestamp of the last modification of a file.
int check_date(char *file_to_check){
	printf("I am checking the date of file %s\n", file_to_check);
	char *date = "date";
	struct stat stbuf;
	stat(file_to_check, &stbuf);
	return (int)stbuf.st_atime;
}
*/


//change_target changes the target being built to the one specified.
void change_target(char* dependency, char* Makefile){
	char *file = "./make4061";
	char *argv[6];
	argv[0] = "./make4061";
	argv[1] = "-f";
	argv[2] = Makefile;
	argv[3] = dependency;
	argv[4] = NULL;

	if(execvp(file, argv)<0){ //attempts to execute make4061 with the new target
		printf("exec failed");
	}
	exit(0);
}
//Separates a command string into an array of strings. Base is the array we want split, target is the array we are
//modifying in the function. Also will return the number of words in the command string.
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
//Once you run out of dependencies, you launch the command for the parent node. This function will execute the command
//that the target must.
int execute_command(target_t targets[], int array_pos, char* cmd){
	int n; //number of words
	int i; //loop counter
	int ctime; //last modification timestamp of the c file
	int otime; //last modification timestamp of the o file
	char *str= cmd; //The actual command passed to us after verifying the other files are built.
	char arr[10][20] = {'\0'}; //Initialized with null chars to ensure the program memory is empty.

	n=getWords(str,arr); //Parse the command string into digestible chunks so that we can call its' command.
	/** This block prints out the command strings**/
	printf("START OF COMMAND STRING\n");
	for(i=0;i<=n;i++)
		printf("%s\n",arr[i]);
	printf("END OF COMMAND STRING\n");
	const char *file = arr[0];
	//***printf("I am the value of file %s\n", file);
	char *argv[MAX_DEPENDENCIES];
	for(i = 0; i <= n; i++){
		if(strlen(arr[i]) > 0){
			argv[i] = arr[i];
//***			printf("I am the value of arri %s\n", arr[i]);
//***			printf("I am the value of argvi %s\n", argv[i]);
		}
		/*** Part of dev, remove later**/
		//***else{
			//***printf("I am not a string\n");
			//***break;
		//***}
	}
	argv[i] = NULL;
//***	char *file_extension =check_extension(argv[2]);
	char *target = (char*) targets[array_pos].TargetName; // Set the target for time comparisons (target we are building)
	int comparison = 0;
	for(int i = 0; i < targets[array_pos].DependencyCount; i++){
		// Check to see if a dependency must be built.
		comparison = compare_modification_time(target, targets[array_pos].DependencyNames[i]);
		if(comparison == 2){
			break;
		}
	}
//***	compare_datetime(argv[2], file_extension);
	if(comparison == 2 || !does_file_exist(target)) {
		execvp(file, argv);
		//*** If here, exec failed
		printf("exec failed\n");
		printf("errno is %s\n", strerror(errno));
	}
	else{
		printf("We didn't need to run another process \n");
	}
	exit(0);
}
//A function to check the dependency list of any node given the list of targets and where the current node being checked
// is in the graph
int check_dependency_list(target_t targets[], int array_pos, char* Makefile, int nTargetCount){
	for (int j = 0; j < MAX_DEPENDENCIES; j++) {
		//If at the end of the dependecy list,
		if (strlen(targets[array_pos].DependencyNames[j]) < 1) {
			//***TODO: Figure out if this is an unsafe way to check the end of the array
			//***printf("I have checked all of my dependencies\n");
			char *cmd = targets[array_pos].Command;
			pid_t childpid,wpid;
			int status = 0;
			childpid = fork();
			if (childpid == -1) {
				printf("Error: Failed to fork into child process");
				return 0;
			}
			if (childpid == 0) {
				execute_command(targets, array_pos, cmd);
				exit(0); //Return from the child process
			}
			else {
//				while ((wpid = wait(&status)) > 0);
				wait(NULL);
				targets[array_pos].Status = 1; //Writes the status of the root
			}
			break;
		}
//			In the case there are more to the dependency list, change the node we are looking at and evaluate it for
//          dependencies
		else {
			printf("%s \n", targets[array_pos].DependencyNames[j]);
//***				TODO: We need to branch here to running ./make4061 specificTarget, Better logging statements plz
			pid_t childpid, wpid;
			int status = 0;
			childpid = fork();
			if (childpid == -1) {
				printf("I failed to fork");
				return 0;
			}
			if (childpid == 0) {
				printf("Execute: ./make4061 %s \n", targets[array_pos].DependencyNames[j]);
				char *dependency = (char*)targets[array_pos].DependencyNames[j];
				change_target(dependency, Makefile);
				exit(0); //Return from the child process
			}
			else {
//				while ((wpid = wait(&status)) > 0);
				wait(0); // wait for the child process to finish
				printf("My child is done running\n");
				printf("I am the node rooted at %s\n", targets[array_pos].TargetName);
				targets[array_pos].Status = 1; //Writes the status of the root
//				show_status(nTargetCount, targets);
			}
		}
	}
	return 0;
}

//This function will go through the given target array and attempt to find the target that was called for in the command line
//If successful, it will then check the dependency list for that target.
void check_target_list(char* TargetName,target_t targets[], int nTargetCount, char* Makefile){
//  		Check for where the set target is in the build file
	for(int i = 0; i < nTargetCount; i++){
		//If the target to start at matches this target, check the dependencies here
		if(strcmp(targets[i].TargetName, TargetName) == 0){
			check_dependency_list(targets,i, Makefile,nTargetCount);
			return;
		}
			/* If there is a problem with the target name, display an error*/
		else if(strcmp(targets[i].TargetName, TargetName) != 0 && i == nTargetCount){
			show_targets_error(TargetName);
			return;
		}
	}
}

//This function merely shows the status of each node whether that is a date object or simply an integer
/*** int show_status(int nTargetCount, target_t targets[] ){
	for(int i = 0; i < nTargetCount; i++){
		printf("I am target %s and I have status %d\n", targets[i].TargetName, targets[i].Status);
	}
	return 0;
}
*/

/*-------------------------------------------------------END OF HELPER FUNCTIONS-------------------------------------*/


/*-------------------------------------------------------MAIN PROGRAM------------------------------------------------*/
int main(int argc, char *argv[])
{
	//***printf("argc has value %d\n", argc);
	//printf("first part of command is %s\n", argv[0]);
	//printf("second part of the command %s\n",argv[1]);
	target_t targets[MAX_NODES];
	int nTargetCount = 0;

  char Makefile[64] = "Makefile";
  char TargetName[64];

  /* Declarations for getopt */
  extern int optind;
  extern char * optarg;
  int ch;
  char *format = "f:h";
  char *temp;

  /* Checks if a file/target was set, 1 if true, 0 if false */
  int fileSet = 0;
  int targetSet = 0;

  //Getopt function is used to access the command line arguments. However there can be arguments which may or may not need the parameters after the command
  //Example -f <filename> needs a finename, and therefore we need to give a colon after that sort of argument
  //Ex. f: for h there won't be any argument hence we are not going to do the same for h, hence "f:h"
  while((ch = getopt(argc, argv, format)) != -1)
  {
	  switch(ch)
	  {
	  	  case 'f':
	  		  temp = strdup(optarg);
	  		  strcpy(Makefile, temp);  // here the strdup returns a string and that is later copied using the strcpy
	  		  free(temp);	//need to manually free the pointer
//	  		  printf(Makefile);
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
	  return -1;   //***This line is not needed
  }

  /* Init Targets */
  memset(targets, 0, sizeof(targets));   //initialize all the nodes first, just to avoid the valgrind checks

  /** This block is breaking the build**/
   //Parse graph file or die, This is the main function to perform the toplogical sort and hence populate the structure
  if((nTargetCount = parse(Makefile, targets)) == -1)  //here the parser returns the starting address of the array of the structure. Here we gave the makefile and then it just does the parsing of the makefile and then it has created array of the nodes
	  return -1;



  //Phase1: Warmup-----------------------------------------------------------------------------------------------------
//  show_targets(targets, nTargetCount);
  //End of Warmup------------------------------------------------------------------------------------------------------

  /*
   * Set Targetname
   * If target is not set, set it to default (first target from makefile)
   */
  if(argc == 1) {
//***  	    TODO: Fix this area potentially, we are getting a failure when the target is listed before the -f flag
      strcpy(TargetName, argv[optind]);    //*** here we have the given target, this acts as a method to begin the building
      targetSet = 1;
  }
  else {
	  strcpy(TargetName, targets[0].TargetName);//*** default part is the first target
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
  	    printf("Provided a build file. \n");
	  //***  	TODO: Unsafe way of checking the default, must be improved
	  if(targetSet){
			/**If a target is provided, check to see if it is a valid target.
			 * Then check the dependencies for the target, and execute the
			 * target. check_target_list does all of this. */
		  check_target_list(TargetName, targets, nTargetCount, Makefile);
	  }
	  else {
			/**If no target is provided, default to the first target in the makefile
			 * Then check the dependencies for the tartget, and execute it
			 * check_dependency_list does all of this **/
			check_dependency_list(targets, 0, Makefile, nTargetCount);
//***		    show_status(nTargetCount, targets);
	  }
  }

  /** If no build file is provided **/
  else{
//***  	show_targets(targets, nTargetCount);
//***  	TODO: Unsafe way of checking the default, must be improved
  	if(targetSet){
  	//***	printf("Target was set: %s \n \n", TargetName);
		//Check for where it is in the build file, if the target is not in the build file, complain
		for(int i = 0; i < nTargetCount; i++){
			printf("%s \n",targets[i].TargetName);
			printf("%s \n", TargetName);
			printf("\n");
		//TODO: Resolve the type conflict between targets[i].TargetName and TargetName

			//If the target to start at matches this target, check the dependencies here
			if(strcmp(targets[i].TargetName, TargetName) == 0){
			//printf("Found it in the array at position %d \n", i);
				check_dependency_list(targets,i, Makefile,nTargetCount);
//				show_status(nTargetCount, targets);
			}
			/* If there is a problem with the target name, we will display an error*/
			else if(strcmp(targets[i].TargetName, TargetName) != 0 && i == nTargetCount-1){
//				TODO: if not the first target passed, we should be running the command for the node here.
//				printf("%s has no dependencies\n", TargetName);
				//show_targets_error(TargetName);
				break;
			}
		}
  	}
  	else {
			check_dependency_list(targets, 0, Makefile, nTargetCount);
    }
  }
  //End of Phase2------------------------------------------------------------------------------------------------------

  return 0;
}
/*-------------------------------------------------------END OF MAIN PROGRAM------------------------------------------*/
