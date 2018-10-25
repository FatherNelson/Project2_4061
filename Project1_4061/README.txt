/* CSci4061 S2018 Assignment 1
* login: liebe110@umn.edu
* date: 10/05/18
* name: Grant James Udstrand, Kieran Lieberg, Jacob Coenen
* id: 5262811, id: 5241931, id 5177976
* Workload Distribution:
*  Grant: Most skilled C programmer, so he did most of the work for
*     the functions relating to forking and executing child processes.
*     So he made most of check_dependency_list and execute_command.
*
*  Jake: Provided secondary input to Grant for check_dependency_list and
*     execute_command. Debugged much of Grant's code and got the program to
*     actually execute build commands. Also did most of the work figuring
*     out how to check the timestamps of files, and how to check if a dependency
*     has already been built, and thus does not need to be built again.
*
*  Kieran: Least skilled C programmer, so he spent time understanding the code
*     provided by Jake and Grant, in order to write the documentation for each
*     function. Provided help with error handling. Also tested the executable
*     with the given tests to see whether make4061 functions as intended in most
*     if not all defined cases.
*/
