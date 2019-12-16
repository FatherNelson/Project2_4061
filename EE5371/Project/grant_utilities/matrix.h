//
// Created by Grant Udstrand on 2019-11-19.
//

#ifndef EE5371_MATRIX_H
#define EE5371_MATRIX_H

#include "../gmath/Matrix3x3.hpp"
#include "sys/time.h"
#include "test_parameters.h"


int matrix_print(Matrix3x3 matrix){
	printf("\n");
	for(int i =0; i< 3; i++) {
		printf("\n");
		for(int j =0; j < 3; j++) {
			double data = matrix.data[i][j];
			printf("%f ", data);
		}
	}
	printf("\n");
	return 1;
}



/**
 * A test for running quaternion operations that end as quaternions.
 * @return
 */
int matrix_in_quaternion_out(){
	unsigned long long count = 0;
	int now_sec = 0;
	int now_usec = 0;
	Vector3 vec = Vector3(M_PI/4, 0, 0);
	Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(vec);
	if(VERBOSE) {
		printf("Rotation Matrix:\n");
		matrix_print(rot_matrix);
		printf("\n: Rotation Matrix\n");
	}
	Matrix3x3 base_matrix = Matrix3x3::Identity();
	//Setup time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = (int)tv.tv_sec;
	int start_usec = tv.tv_usec;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
	//Time test
	while (test_duration > (unsigned long long)(now_sec * 1000000) + now_usec) {
//		base_matrix = base_matrix *= rot_matrix;
		Quaternion out = Matrix3x3 :: ToQuaternion(base_matrix);
		gettimeofday(&tv, NULL);
		now_sec = (int)tv.tv_sec;
		now_usec = tv.tv_usec;
		count += 1;
	}
	//Workload test
	if(WORKLOAD){
		while (count < COUNT_LIMIT) {
			base_matrix = base_matrix *= rot_matrix;
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}

	if(VERBOSE) {
		printf("Final Matrix: \n");
		matrix_print(base_matrix);
		printf("\n");
		printf("\n");
	}

	double test_length = (double)((now_sec*1000000)+ now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time: %fS\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));

	return count;
}
/**
 * A function that returns a transformation matrix at the end of every computation.
 * @return
 */
int matrix_in_matrix_out_computation(){
	unsigned long long count = 0;
	int now_sec = 0;
	int now_usec = 0;
	Vector3 vec = Vector3(M_PI/4, 0, 0);
	Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(vec);
	if(VERBOSE) {
		printf("Rotation Matrix:\n");
		matrix_print(rot_matrix);
		printf("\n: Rotation Matrix\n");
	}
	Matrix3x3 base_matrix = Matrix3x3::Identity();
	//Setup time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = (int)tv.tv_sec;
	int start_usec = tv.tv_usec;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
	//Time test
	while (test_duration > (unsigned long long)(now_sec * 1000000) + now_usec) {
		base_matrix = base_matrix *= rot_matrix;
		gettimeofday(&tv, NULL);
		now_sec = (int)tv.tv_sec;
		now_usec = tv.tv_usec;
		count += 1;
	}
	//Workload test
	if(WORKLOAD){
		while (count < COUNT_LIMIT) {
			base_matrix = base_matrix *= rot_matrix;
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}

	if(VERBOSE) {
		printf("Final Matrix: \n");
		matrix_print(base_matrix);
		printf("\n");
		printf("\n");
	}

	double test_length = (double)((now_sec*1000000)+ now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time: %fS\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));

	return count;
}
/**
 * A function that returns an euler angle based on quaternion input
 * @return
 */
int matrix_in_euler_out(){
	unsigned long long count = 0;
	int now_sec = 0;
	int now_usec = 0;
	Vector3 vec = Vector3(M_PI/4, 0, 0);
	Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(vec);
	if(VERBOSE) {
		printf("Rotation Matrix:\n");
		matrix_print(rot_matrix);
		printf("\n: Rotation Matrix\n");
	}
	Matrix3x3 base_matrix = Matrix3x3::Identity();
	//Setup time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = (int)tv.tv_sec;
	int start_usec = tv.tv_usec;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
	//Time test
	while (test_duration > (unsigned long long)(now_sec * 1000000) + now_usec) {
//		base_matrix = base_matrix *= rot_matrix;
		Vector3 out = Matrix3x3 :: ToEulerYXZ(base_matrix);
		gettimeofday(&tv, NULL);
		now_sec = (int)tv.tv_sec;
		now_usec = tv.tv_usec;
		count += 1;
	}
	//Workload test
	if(WORKLOAD){
		while (count < COUNT_LIMIT) {
			base_matrix = base_matrix *= rot_matrix;
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}

	if(VERBOSE) {
		printf("Final Matrix: \n");
		matrix_print(base_matrix);
		printf("\n");
		printf("\n");
	}

	double test_length = (double)((now_sec*1000000)+ now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time: %fS\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));

	return count;
}


#endif //EE5371_MATRIX_H
