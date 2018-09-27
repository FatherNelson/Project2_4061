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
#include <unistd.h>

/*-------------------------------------------------------HELPER FUNCTIONS PROTOTYPES---------------------------------*/
void show_error_message(char * ExecName);

//Write your functions prototypes here
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

//Write your functions here
void show_targets_error(char* ExecName){
	fprintf(stderr, "Target %s does not exist. Please enter a valid build target \n", ExecName);
	exit(0);
}

//A function to check the dependency list of any node given the list of targets and where the current node being checked
// is in the graph
int check_dependency_list(target_t targets[], int array_pos){
	//	    We are told we will have at most ten dependencies, so hard code for now
	for (int j = 0; j < 10; j++) {
		printf("%d", array_pos);
		//Indicates that we are at the end of the dependency list
		if (strlen(targets[array_pos].DependencyNames[j]) < 1) {
			//TODO: Figure out if this is an unsafe way to check the end of the array
//				printf("%s\n", targets[i].Command);
			char *x = targets[array_pos].Command;
			pid_t childpid;
			childpid = fork();
			if (childpid == -1) {
				printf("I failed to fork");
				return 1;
			}
			if (childpid == 0) {
//				Make this statement true
				printf("I am the child that will run my command since my parent has"
				       " no dependencies %ld \n", (long) getpid());
				printf("Execute: %s \n", x);
				return 1; // MUST return from teh child process
			}
			else {
				printf("I am the parent that has no dependencies %ld \n", (long) getpid());
			}
//				printf("I am process %ld and my x is %s \n", (long)getpid(), x);
			break;
		}
//			In the case there are more to the dependency list, change the node we are looking at
		else {
			printf("%s \n", targets[array_pos].DependencyNames[j]);
//				TODO: We need to branch here to running ./make4061 specificTarget
		}
	}
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
        printf("Status: %s \n", target_view.Status);
        printf("<-------- END OF TARGET --------> \n \n");
    }
    return;
}

/*-------------------------------------------------------END OF HELPER FUNCTIONS-------------------------------------*/


/*-------------------------------------------------------MAIN PROGRAM------------------------------------------------*/
//Main commencement
int main(int argc, char *argv[])
{
  target_t targets[MAX_NODES];
  int nTargetCount = 0;

  /* Variables you'll want to use */
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
	  return -1;   //This line is not needed
  }

  /* Init Targets */
  memset(targets, 0, sizeof(targets));   //initialize all the nodes first, just to avoid the valgrind checks

  /** This block is breaking the build**/
   //Parse graph file or die, This is the main function to perform the toplogical sort and hence populate the structure
  if((nTargetCount = parse(Makefile, targets)) == -1)  //here the parser returns the starting address of the array of the structure. Here we gave the makefile and then it just does the parsing of the makefile and then it has created array of the nodes
	  return -1;



  //Phase1: Warmup-----------------------------------------------------------------------------------------------------
  //Parse the structure elements and print them as mentioned in the Project Writeup
  /* Comment out the following line before Phase2 */
  show_targets(targets, nTargetCount);
  //End of Warmup------------------------------------------------------------------------------------------------------

  /*
   * Set Targetname
   * If target is not set, set it to default (first target from makefile)
   */
  if(argc == 1) {
//  	    TODO: Fix this area potentially, we are getting a failure when the target is listed before the -f flag
      strcpy(TargetName, argv[optind]);    // here we have the given target, this acts as a method to begin the building
      targetSet = 1;
  }
  else {
	  strcpy(TargetName, targets[0].TargetName);// default part is the first target
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
  /*Your code begins here*/

  /** If a build file is provided**/
  if(fileSet == 1){
  	    printf("Provided a build file.");
  }

  /** If no build file is provided **/
  else{
//  	show_targets(targets, nTargetCount);
//  	TODO: Unsafe way of checking the default, must be improved
//  	if(targetSet){
////  		printf("%s \n", TargetName);
////  		Check for where it is in the build file, if the target is not in the build file, complain
//		for(int i = 0; i < nTargetCount; i++){
//			printf("%s \n",targets[i].TargetName);
//			printf("%s \n", TargetName);
////			TODO: Resolve the type conflict between targets[i].TargetName and TargetName
//
//			//parameter
//			if(targets[i].TargetName == TargetName){
//				printf("Found it in the array at position %d \n", i);
//				check_dependency_list(targets,i);
//			}
//			/* If there is a problem with the target name, we will display an error*/
//			else if(targets[i].TargetName != TargetName && i == nTargetCount){
//				show_targets_error(TargetName);
//				break;
//			}
//		}
//  	}
//  	else {
	    //Should be to nTargetCount in the final version, it is set to one for testing
//	    for (int i = 0; i < 1; i++) {
//		    printf("Start of target %d \n", i);
//		    printf("%s \n", targets[i].TargetName);
//		    printf("Name of target is above here \n");
//	    We are told we will have at most ten dependencies, so hard code for now
//			check_dependency_list(targets, i);
//	    }
    }
//  }
  
  
  /*End of your code*/
  //End of Phase2------------------------------------------------------------------------------------------------------

  return 0;
}
/*-------------------------------------------------------END OF MAIN PROGRAM------------------------------------------*/
