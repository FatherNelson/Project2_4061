//
// Created by Grant Udstrand on 10/15/18.
//

#ifndef OS_WEEK05_DEFINITIONS_H
#define OS_WEEK05_DEFINITIONS_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

void copy_file(char* file_to_copy, char* endfile_name){
	if(file_to_copy == NULL){
		perror("File does not exist");
		exit(-1);
	}
	link(file_to_copy, endfile_name);
}

void create_file(char* filename){
	creat(filename, 0777);
}
void delete_file(char* filename){
	unlink(filename);
}

void get_meta(char* filename){
	struct stat buf[100];
	struct stat buf2[100];
	stat(filename, buf);
	printf("This file is %d bytes large \n", (int) buf->st_size);
	printf("It was last updated at %d \n", (int)buf->st_mtime);
	if(S_ISDIR(buf->st_mode)){
		printf("I found a directory\n");
	}
}

void change_permissions(char* filename, int permissions[] ){
	char string[50] = "\0";
	for(int i = 0; i < sizeof(permissions)/sizeof(int) +1; i++){
		if(permissions[i] == 1){
			strcat(string, "7"); //gives "r" "w" "x" at said position
		}
		else{
			strcat(string, "0"); //gives null permissions at said position
		}
	}
	chmod(filename, 0777);
}

void create_directory(char* dirname){
	if(mkdir(dirname, 0777) < 0) {
		//The second part of the command is permissions, I just give permission to everything
		perror("I have failed to create a directory");
	}
}

void remove_directory2(char* dirname){
	if(rmdir(dirname)<0){
		perror("I failed to remove the directory");
	}
}
void change_directory(char* pathname){
	char buf[100];
	getwd(buf);
	printf("%s\n",buf);
	chdir(pathname);
	getwd(buf);
	printf("%s\n",buf);
	get_meta("abc.txt");
}

void exec_command(){
	char* args[] = {"ls", "../freedom", (char*) NULL};
	if(execvp("ls", args)){
		printf("\nFailed to exec\n");
	}
}

void create_symlink(char* dest, char* newpath){
	symlink(dest, newpath);
}

void week05_main(){
	copy_file("foo.txt", "bar.txt");
	delete_file("bar.txt");
	get_meta("people.txt");
//	open_file("people.txt");
	int permissions[] = {1,1,1};
	change_permissions("people.txt", permissions);
	create_file("grant.txt");
	change_directory("freedom");
//	create_directory("Halloween");
//	create_symlink("Halloween", "ghost");
//	remove_directory("Halloween");
//	create_symlink("freedom", "patriot");
//	exec_command();
}

#endif //OS_WEEK05_DEFINITIONS_H
