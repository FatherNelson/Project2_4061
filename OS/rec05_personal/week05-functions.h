//
// Created by Grant Udstrand on 10/15/18.
//

#ifndef OS_WEEK05_FUNCTIONS_H
#define OS_WEEK05_FUNCTIONS_H
void copy_file(char* file_to_copy, char* endfile_name);
void delete_file(char* filename);
void change_permissions(char* filename, int permissions[]);
void change_directory(char* filename);
void exec_command();
void create_directory(char* dirname);
void remove_directory2(char* dirname);
void week05_main();
#endif //OS_WEEK05_FUNCTIONS_H
