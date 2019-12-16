//
// Created by Grant Udstrand on 2019-12-06.
//

#ifndef EE5371_EULER_H
#define EE5371_EULER_H

#include "test_parameters.h"
#include "sys/time.h"
#include "../gmath/Quaternion.hpp"
/**
 * A test for running quaternion operations that end as quaternions.
 * @return
 */
int euler_in_quaternion_out(){
	unsigned long long count = 0;
	int now_sec = 0;
	int now_usec = 0;
	Vector3 rot_vec = Vector3(M_PI/4, M_PI/4, M_PI/4);
	Vector3 base_vec = Vector3(0,0,0);
	Matrix3x3 base_matrix = Matrix3x3 ::FromEulerYXZ(base_vec);
	Quaternion base_quat = Quaternion::FromEuler(base_vec);
	//Setup time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = tv.tv_sec;
	int start_usec = tv.tv_usec;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
//	//Time test
	if(TIMELOAD) {
		while (test_duration > (unsigned long long)(now_sec * 1000000) + now_usec) {
//			Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(base_vec);
			Quaternion rot_quaternion = Quaternion::FromEuler(base_vec);
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}

	double test_length = (double)((now_sec*1000000)+now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time: %fS\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));
}
/**
 * A function that returns a transformation matrix at the end of every computation.
 * @return
 */
int euler_in_matrix_out(){
	unsigned long long count = 0;
	int now_sec = 0;
	int now_usec = 0;
	Vector3 rot_vec = Vector3(M_PI/4, M_PI/4, M_PI/4);
	Vector3 base_vec = Vector3(0,0,0);
	Matrix3x3 base_matrix = Matrix3x3 ::FromEulerYXZ(base_vec);
	Quaternion base_quat = Quaternion :: FromEuler(base_vec);
	//Setup time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = tv.tv_sec;
	int start_usec = tv.tv_usec;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
//	//Time test
	if(TIMELOAD) {
		while (test_duration > (unsigned long long)(now_sec * 1000000) + now_usec) {
			Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(base_vec);
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}

	double test_length = (double)((now_sec*1000000)+now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time: %fS\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));
}
/**
 * A function that returns an euler angle based on quaternion input
 * @return
 */
int euler_in_euler_out_computation(int arg){
	unsigned long long count = 0;
	int now_sec = 0;
	int now_usec = 0;
	Vector3 rot_vec = Vector3(M_PI/4, M_PI/4, M_PI/4);
	Vector3 base_vec = Vector3(0,0,0);
	Matrix3x3 base_matrix = Matrix3x3 ::FromEulerYXZ(base_vec);
	Quaternion base_quat = Quaternion :: FromEuler(base_vec);
	//Setup time
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = tv.tv_sec;
	int start_usec = tv.tv_usec;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
//	//Time test
	if(TIMELOAD) {
		while (test_duration > (unsigned long long)(now_sec * 1000000) + now_usec) {
			if(arg) {
				Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(rot_vec);
				base_matrix = base_matrix *= rot_matrix;
			}
			else{
				Quaternion rot_quat = Quaternion::FromEuler(rot_vec.X, rot_vec.Y, rot_vec.Z);
				base_quat *= rot_quat;
			}
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}
//	//Workload test
	if(WORKLOAD){
		while (count < COUNT_LIMIT) {
			Matrix3x3 rot_matrix = Matrix3x3::FromEulerYXZ(rot_vec);
			base_matrix = base_matrix *= rot_matrix;
			if(VERBOSE){
				matrix_print(base_matrix);
			}
			gettimeofday(&tv, NULL);
			now_sec = tv.tv_sec;
			now_usec = tv.tv_usec;
			count += 1;
		}
	}
//
	if(VERBOSE) {
		printf("Final Angles: \n");
		base_vec = Matrix3x3::ToEulerYXZ(base_matrix);
		printf("%f %f %f \n", base_vec.X, base_vec.Y, base_vec.Z);
		printf("\n");
		printf("\n");
	}

//
	double test_length = (double)((now_sec*1000000)+now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time: %fS\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));
//	return count;
}
#endif //EE5371_EULER_H
