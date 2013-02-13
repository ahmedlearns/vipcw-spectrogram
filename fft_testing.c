#include <stdio.h>
#include <stdlib.h>
#include "fft.h"

int main(){
	int N = 8;                    /* number of points in FFT */
    double (*X)[2];          		 /* pointer to frequency-domain samples */   /* double */
    double x[N][2];             /* double */
    X = malloc(2 * N * sizeof(double));  /* double */

    int i = 0;
    
    //~ printf("Test 1..\n");
    //~ x[0][0] = 1;
    //~ x[0][1] = 0;
	//~ i = 1;
	//~ while(i < N){
		//~ x[i][0] = 0;
		//~ x[i][1] = 0;
		//~ i++;
	//~ }
	
	printf("Test 2..\n");
	i = 0;
	while(i<N){
		x[i][0] = x[i][1] = 0;
		if(i == 1) x[i][0] = 1;
		i++;
	}
    
    //~ while(i < N){
    	//~ x[i][0] = 5;
    	//~ x[i][1] = 0;
    	//~ i++;
    //~ }
    
    for(i = 0; i < N; i++){
		printf("x[%d][0] = %f\tx[%d][1] = %f\n", i, x[i][0], i, x[i][1]);
	}    

	printf("Calculating fft..\n");
    fft(N, x, X);

    for(i = 0; i < N; i++){
    	printf("X[%d][0] = %f\tX[%d][1] = %f\n", i, X[i][0], i, X[i][1]);
    }
}
