//
// Created by Grant Udstrand on 2019-09-22.
//

void end(FILE* fp, char* A, char* B){
	fclose(fp);
	free(A);
	free(B);
}

void cwd(){
	int size = 1000;
	char buf[size];
	printf("%s\n", getcwd(buf,size));
}

//void positive_stride(int max_stride){
//	for (int stride = 0; stride < max_stride; stride = stride + 1) {
//
//		/* Start the timer by reading its current value.            */
//		gettimeofday(&start_time,NULL);
//
//		/* Stuff to be measured */
//		for (long i = n; i > 0; i = i + stride) {
//			A[i] = B[i];
//		}
//
//		/* Measure the elapsed time by subtracting the start value
//		   from the current value.                                  */
//		gettimeofday(&end_time, NULL);
//		start_count = (double) start_time.tv_sec
//		              + 1.e-6 * (double) start_time.tv_usec;
//		end_count = (double) end_time.tv_sec +
//		            1.e-6 * (double) end_time.tv_usec;
//		elapsed_time = (end_count - start_count);
//		if(DEBUG) {
//			printf("value at start:%d stride: %d \n", A[0], stride);
//			printf("The total elapsed time is:  %f seconds\n", elapsed_time);
//		}
//		if(!DEBUG){
////			printf("%f %d\n", elapsed_time, stride);
//			if(fp != NULL) {
//				fprintf(fp, "%f %d\n", elapsed_time, stride);
//			}
//			else{
//				printf("Failed to open file\n");
//			}
//		}
//	}
//}
//void negative_stride(int max_stride){
//	for (int stride = -1; stride > -max_stride; stride = stride  - 1) {
//
//		/* Start the timer by reading its current value.            */
//		gettimeofday(&start_time,NULL);
//
//		/* Stuff to be measured */
//		for (long i = n; i > 0; i = i + stride) {
//			A[i] = B[i];
//		}
//
//		/* Measure the elapsed time by subtracting the start value
//		   from the current value.                                  */
//		gettimeofday(&end_time, NULL);
//		start_count = (double) start_time.tv_sec
//		              + 1.e-6 * (double) start_time.tv_usec;
//		end_count = (double) end_time.tv_sec +
//		            1.e-6 * (double) end_time.tv_usec;
//		elapsed_time = (end_count - start_count);
//		if(DEBUG) {
//			printf("value at start:%d stride: %d \n", A[0], stride);
//			printf("The total elapsed time is:  %f seconds\n", elapsed_time);
//		}
//		if(!DEBUG){
////			printf("%f %d\n", elapsed_time, stride);
//			if(fp != NULL) {
//				fprintf(fp, "%f %d\n", elapsed_time, stride);
//			}
//			else{
//				printf("Failed to open file\n");
//			}
//		}
//	}
//}