//
// Created by Grant Udstrand on 2018-11-30.
//

#ifndef OS_SLICE_H
#define OS_SLICE_H
typedef struct request_queue {
	int fd;
	void *request;
} request_t;

void slice_queue(request_t* src, request_t* dest, int start, int end);
#endif //OS_SLICE_H
