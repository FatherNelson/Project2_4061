//
// Created by Grant Udstrand on 2019-04-05.
//

#include "conversions.h"
#include <string.h>
#include <stdio.h>
#include <iostream>


long convertBinaryToDecimal(char* binary_string, int bits){
	int i = 0;
	long n = strtoll(binary_string, NULL, 2);
	int negative = 0; //Default to non-negative, if set, it means it is negative
	long decimalNumber = 0;
	long check = n >> (bits-1);
//	TODO: Make sure that negative numbers work too.
	if(check & 1){
		n = ~n + 1;
		negative = 1;
//		printf("I am a negative number\n");
	}
	while (i < bits) {
		int lsb = n & 1;
		decimalNumber += lsb * pow(2, i);
		n = n >> 1;
		i++;
	}
	if(negative){
		return decimalNumber*-1;
	}
	else {
		return decimalNumber;
	}
}

float convertIntToReal(long n){
	return n * 1.1444091796875 * pow(10,-6);
}

