//
// Created by Grant Udstrand on 10/14/18.
//

#ifndef OS_REC04_DEFINITIONS_H
#define OS_REC04_DEFINITIONS_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

void print_stuff(){
	printf("Hello World\n");
}

int open_file(char* pname){
	int fd = open(pname, O_RDWR);
	if(fd == -1){
		perror("Failed to open the file");
	}
	return fd;
}

void close_file(fd){
	int close_status = close(fd);
	if(close_status==-1){
		perror("Error closing the file");
	}
	else{
		printf("Closed file successfully\n");
	}
}

void read_file(fd){
	int buf_pos=0; //Gives the current index in the buffer, for printing out contents.
	int a_count = 0;
	char buf[2000];
	int bytes = (int) read(fd, buf,2000); // Transfer 1000 bytes from the file to the buffer at most. Could stop before
	printf("%d\n", bytes);
	while(buf_pos < bytes){
		char* this_char = buf[buf_pos]; //This is an address in the buffer
		printf("%s", &this_char);
		if(strcmp(&this_char,"a") == 0){
			a_count +=1;
		}
		buf_pos+=1;
	}
//	printf("\nThere were %d a's in this document \n", a_count); //Prints off the number of a's in the document
}

void write_file(char* msg){
	int wfd;  // The file we are writing, a foo.txt
	int sz = 0;
	wfd = open("foo.txt",O_WRONLY);
	/**Write something here to sanity check that the file has no chars. If there are, erase them**/
	if(wfd < 0){
		perror("Failed to write to file");
	}
	sz = (int) write(wfd, msg, strlen(msg));
	if(sz < strlen(msg)){
		perror("There was a failure in writing all of the bytes");
	}
}

void stream_read_file(){
	FILE* f;
	char buf[100];
	memset(buf,0, sizeof(buf));
	if ((f = fopen("./foo.txt","r")) == NULL) {
		perror("Couldn't open file");
	}
	int r_num = fread(buf, 1, 100, f);
	printf("%d \n", r_num);
	int i = 0;
	printf("%s", &buf);
	fclose(f);
}
void read_write(){
	FILE *fp;
	char c[] = "This is somet";
	char buffer[100];
	fp = fopen("rec04-files/freadwrite_output.txt", "w+");
	if (fp==NULL)
		perror ("Error opening file");
	else {
		fwrite(c, strlen(c) + 1, 1, fp);
		fseek(fp, SEEK_SET, 0);
		fread(buffer, strlen(c)+1, 1, fp);
		printf("%s\n", buffer);
		fseek(fp, 0, SEEK_END);
		printf("Length is = %ld\n", ftell(fp));
		fclose(fp);
	}
}
void file_copy(char* pname){
	int fd = open_file(pname);
	printf("%d", fd);
	int buf_pos=0; //Gives the current index in the buffer, for printing out contents.
	int a_count = 0;
	char buf[2000];
	int bytes = (int) read(fd, buf,2000); // Transfer 1000 bytes from the file to the buffer at most. Could stop before
	printf("%d\n", bytes);
	while(buf_pos < bytes){
		char* this_char = buf[buf_pos]; //This is an address in the buffer
		printf("%s", &this_char);
		if(strcmp(&this_char,"a") == 0){
			a_count +=1;
		}
		buf_pos+=2;
	}

}
void rec04main(){
	char* pname = "foo.txt";
	int fd; //The file we are reading, in this case the input file from recitation four
//	fd = open_file(pname);
//	printf("%d \n", fd);
//	read_file(fd);
//	write_file("Here is the place where all the junkies go"
//			" Where everything is fast and the rest is slow\n");
//	FILE* f;
//	read_write();
	file_copy(pname);
	close_file(fd);
}
#endif //OS_REC04_DEFINITIONS_H
