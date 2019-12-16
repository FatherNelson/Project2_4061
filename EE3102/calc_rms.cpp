#include "rms.h"
#include "math.h"
#include <cstdint>
#include <stdio.h>
//
// Created by Grant Udstrand on 2019-03-06.
//

/* The calculation of RMS in this program is following this prescription:
 * 1. Collect samples from the ADC - Assume samples array of floats.
 * 2. Sum the squares of the samples - √
 * 3. Divide the sum by the number of samples √
 * 4. Take a square root of the quotient
 * 5. Convert to Voltage. - This was already done for you in the creation of an array of floats.
 * In dummy.txt is a randomized sample of 2000 24 bit binary numbers
 *
 //Here we are assuming we have the array of 2000 samples.
 */

//Square root
float root_samples(float samples_quotient){
	return sqrt(samples_quotient);
}

//TODO: Do we NEED a 64 bit value here, or can we assume a middling/low input voltage
int sum_squares(int samples[]){
	uint64_t result = 0;
	for(int i = 0; i < 3000; i++){
		printf("I have a value of %d\n", samples[i]);
		result += pow(samples[i],2);
	}
	printf("I am the sum of squares: %ld\n", result);
	return result;
}

//Divide the sum by the number of samples
float divide_samples(uint64_t sum_of_samples, int number_of_samples){
	return sum_of_samples/(number_of_samples);
}

float calc_rms(int samples[]){
	uint64_t sum_of_squares = sum_squares(samples);
	float quotient = divide_samples(sum_of_squares, 3000);
	float output = root_samples(quotient);
	printf("I am the RMS voltage before scaling: %f\n", output);
	return output;
}
