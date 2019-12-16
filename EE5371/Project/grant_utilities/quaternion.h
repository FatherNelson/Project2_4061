//
// Created by Grant Udstrand on 2019-12-06.
//

#ifndef EE5371_QUATERNION_H
#define EE5371_QUATERNION_H

#include "../gmath/Quaternion.hpp"
#include "time.h"
#include "test_parameters.h"
/**
 * A test for running quaternion operations that end as quaternions.
 * @return
 */
void print_quaternion(Quaternion q){
	printf("%f + %fi + %fj + %fk\n", q.W, q.X, q.Y, q.Z);
}
int quaternion_in_quaternion_out_computation(){
	unsigned long long count = 0;
	//This quaternion is a 45 degree rotation in all directions
//  Quaternion v = Quaternion(0.844623, 0.461940, 0.191342, 0.191342);
	Quaternion p = Quaternion :: Identity();
	Vector3 base_vec = Vector3(M_PI/4,0,0);
	Matrix3x3 base_matrix = Matrix3x3 ::FromEulerYXZ(base_vec);
	Quaternion v = Matrix3x3 :: ToQuaternion(base_matrix);
	if(VERBOSE) {
		Matrix3x3 V = Matrix3x3::FromQuaternion(v);
		matrix_print(V);
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = tv.tv_sec;
	int start_usec = tv.tv_usec;
	int now_sec = 0;
	int now_usec = 0;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
	if(TIMELOAD) {
		while (test_duration > (unsigned long long) (now_sec * 1000000) + now_usec) {
			p *= v;
			count += 1;
			gettimeofday(&tv, NULL);
			now_sec = (int)tv.tv_sec;
			now_usec = (int)tv.tv_usec;
//			printf("We calculated this quaternion: \n");
//			print_quaternion(p);
		}
	}
	if(WORKLOAD) {
		while (count < COUNT_LIMIT) {
			if (VERBOSE) {
				printf("We received this quaternion: \n");
				print_quaternion(p);
			}
			p *= v;
			count += 1;
			gettimeofday(&tv, NULL);
			now_sec = (int)tv.tv_sec;
			now_usec = (int)tv.tv_usec;
			if (VERBOSE) {
				printf("We calculated this quaternion: \n");
				print_quaternion(p);
			}
		}
	}
	Vector3 euler = Quaternion::ToEuler(v);
//	printf("%d\n", count);
//	printf("%f %f %f \n", euler.X, euler.Y, euler.Z);
//	printf("%f + %fi + %fj + %fk \n", p.W, p.X, p.Y, p.Z);
//	Matrix3x3 o = Matrix3x3:: FromQuaternion(p);
//	matrix_print(o);

	double test_length = (double)((now_sec*1000000)+now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time (seconds): %f\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));
	return count;
}
/**
 * A function that returns a transformation matrix at the end of every computation.
 * @return
 */
double quaternion_in_matrix_out(){
	unsigned long long count = 0;
	Quaternion p = Quaternion :: Identity();
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = tv.tv_sec;
	int start_usec = tv.tv_usec;
	int now_sec = 0;
	int now_usec = 0;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
	if(TIMELOAD) {
		while (test_duration > (unsigned long long) (now_sec * 1000000) + now_usec) {
			Matrix3x3 out = Matrix3x3:: FromQuaternion(p);
			count += 1;
			gettimeofday(&tv, NULL);
			now_sec = (int)tv.tv_sec;
			now_usec = (int)tv.tv_usec;
//			printf("We calculated this quaternion: \n");
//			print_quaternion(p);
		}
	}

	double test_length = (double)((now_sec*1000000)+now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time (seconds): %f\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));
	return count;
}
/**
 * A function that returns an euler angle based on quaternion input
 * @return
 */
double quaternion_in_euler_out(){
	unsigned long long count = 0;
	Quaternion p = Quaternion :: Identity();
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int start_sec = tv.tv_sec;
	int start_usec = tv.tv_usec;
	int now_sec = 0;
	int now_usec = 0;
	unsigned long long test_duration = (start_sec * 1000000) + start_usec + LENGTH_OF_TEST;
	if(TIMELOAD) {
		while (test_duration > (unsigned long long) (now_sec * 1000000) + now_usec) {
			Vector3 out = Quaternion :: ToEuler(p);
			count += 1;
			gettimeofday(&tv, NULL);
			now_sec = (int)tv.tv_sec;
			now_usec = (int)tv.tv_usec;
//			printf("We calculated this quaternion: \n");
//			print_quaternion(p);
		}
	}

	double test_length = (double)((now_sec*1000000)+now_usec - (start_sec*1000000) - start_usec)/1000000;
//	printf("Final Run Time (seconds): %f\n", test_length);
//	printf("Total Computations: %d\n", count);
//	printf("Computations/uS: %f\n", count/(test_length*1000000));

	printf("%f         %d          %f\n", test_length, count, count/(test_length*1000000));
	return count;
}
#endif //EE5371_QUATERNION_H
