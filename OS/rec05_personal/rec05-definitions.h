//
// Created by Grant Udstrand on 10/14/18.
//

#ifndef OS_REC05_DEFINITIONS_H
#define OS_REC05_DEFINITIONS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rec05-structs.h"

#define NUMBER_OF_PEOPLE 8 //Defines the maximum number of people we can read from people.txt
void make_directory(char* pname){
	mkdir(pname,0777); //Make a directory with all permissions
}

void remove_directory(char* pname){
	rmdir(pname);
}

void open_directory(char* pname){
	DIR *dir;
	dir = opendir(pname);
	if(dir == NULL){
		perror("Directory doesn't exist");
	}
}
void read_file_to_person(fd){
	int buf_pos=0; //Gives the current index in the buffer, for printing out contents.
	int a_count = 0;
	char buf[2000];
	int bytes = (int) read(fd, buf,2000); // Transfer 1000 bytes from the file to the buffer at most. Could stop before
	printf("%d\n", bytes);
	while(buf_pos < bytes){
		char* this_char = buf[buf_pos]; //This is an address in the buffer
		printf("%s", &this_char);
		buf_pos+=1;
	}
}

struct Person* parse_people(char* pname){
	struct Person PEOPLE[NUMBER_OF_PEOPLE]; //An array for ten people
	int PEOPLE_COUNTER = 0;
	FILE* fh;
	fh = fopen(pname, "r");

//check if file exists
	if (fh == NULL){
		printf("file does not exist %s\n", pname);
		return PEOPLE;
	}


//read line by line
	const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, fh) != NULL)  {
		//Create a new person
		struct Person* this_one = malloc(sizeof(struct Person));
		char* split_text = malloc(50* sizeof(char));
		char* second_half;
		char* first_half = malloc(50 * sizeof(char));
		strcpy(split_text, line);
		first_half = strtok_r(split_text, "\t", &second_half);
		this_one->name = first_half;
		this_one->age = atoi(second_half);
		printf("%s %d\n", this_one->name, this_one->age);
		PEOPLE[PEOPLE_COUNTER] = *this_one;
		PEOPLE_COUNTER+=1;
		free(this_one);
		free(first_half);
//		free(split_text);
//		first_half = &first_half + sizeof(first_half);
	}
	printf("\n");
	free(line);
	fclose(pname);
	return PEOPLE;
}
void rec05_main(){
	char* dir = "freedom";
	char* pname = "people.txt";
	struct Person* PEOPLE; //An array for ten people
	int PEOPLE_COUNTER = 0;
	make_directory(dir);
	open_directory(dir);
	PEOPLE = parse_people(pname);
}
#endif //OS_REC05_DEFINITIONS_H
