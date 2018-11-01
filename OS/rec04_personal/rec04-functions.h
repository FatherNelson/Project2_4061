//
// Created by Grant Udstrand on 10/14/18.
//

#ifndef OS_REC04_FUNCTIONS_H
#define OS_REC04_FUNCTIONS_H

#include "rec04-definitions.h"
void print_stuff();
int open_file(char* pname);
void close_file(int fd);
void read_file(int fd);
void write_file(char* msg);
void stream_read_file(int fd);
void stream_write_file(char* msg);
void read_write();
void file_copy(char* pname);
void rec04main();

#endif //OS_REC04_FUNCTIONS_H
