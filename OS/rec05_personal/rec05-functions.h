//
// Created by Grant Udstrand on 10/14/18.
//

#ifndef OS_REC05_FUNCTIONS_H
#define OS_REC05_FUNCTIONS_H

#include "rec05-definitions.h"
#include "rec05-structs.h"

void make_directory(char* pname);
void remove_directory(char* pname);
void open_directory(char* pname);
void read_file_to_person(int fd);
struct Person* parse_people(char* pname);
void rec05_main();

#endif //OS_REC05_FUNCTIONS_H
