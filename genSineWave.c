#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(){

	FILE *file = fopen("wavs/sine.wav", "wb");
	
	int i = 0;
	for(i = 0; i < 1000; i++){
		fprintf(file, "%f\n", sin(0.01 * i));
	}

	fclose(file);
}
