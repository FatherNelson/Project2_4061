//
// Created by Grant Udstrand on 2019-09-22.
//

void test(){
	long unsigned int max = 100000000;
	double* new_arr = new double [max];
	for (int i = 0; i < max; i++){
		new_arr[i] = new_arr[i] + new_arr[i];
	}
}