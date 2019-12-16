//
// Created by Grant Udstrand on 2019-11-19.
//

#include "test.h"
#include "gmath/Matrix3x3.hpp"
#include "grant_utilities/matrix.h"
#include "grant_utilities/quaternion.h"
#include "grant_utilities/euler.h"
#include "grant_utilities/rotations.h"
#include "grant_utilities/unit_test.h"
#include "grant_utilities/test_parameters.h"

int test(){
	printf("Quaternion Computation:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		quaternion_in_quaternion_out_computation();
	}
	printf("\n");
	printf("Matrix Computation:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		matrix_in_matrix_out_computation();
	}
	printf("\n");
	printf("Euler-Matrix Computation:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		euler_in_euler_out_computation(1);
	}
	printf("\n");
	printf("Euler-Quaternion Computation:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		euler_in_euler_out_computation(0);
	}
	printf("\n");
	printf("Quaternion Matrix Conversion:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		quaternion_in_matrix_out();
	}
	printf("\n");
	printf("Quaternion Euler Conversion:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		quaternion_in_euler_out();
	}
	printf("\n");
	printf("Matrix Quaternion Conversion:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		matrix_in_quaternion_out();
	}
	printf("\n");
	printf("Matrix Euler Conversion:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		matrix_in_euler_out();
	}
	printf("\n");
	printf("Euler Quaternion Conversion:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		euler_in_quaternion_out();
	}
	printf("\n");
	printf("Euler Matrix Conversion:\n");
	for(int i = 0; i < NUM_TESTS; i++){
		euler_in_matrix_out();
	}

}