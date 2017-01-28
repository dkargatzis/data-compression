#include <stdio.h>
#include <stdlib.h>


int main(){

	FILE *input1,*input2;

	input1 = fopen("COMP516coursedoc_32.txt", "r");
	if(input1 == NULL) printf("Unable open file1\n");
	
	input2 = fopen("valid.txt", "r");
	if(input2 == NULL) printf("Unable open file2\n");

	int i, diff=0;
	int num1=0,num2=0;
	for (i=0; i<22097; i++){
		if(fscanf(input1, "%d", &num1) != 1 || fscanf(input2, "%d", &num2) !=1)
			break;
		if(num1 != num2)	diff++;
	}
	printf("iteration num\t%d\n",i);
	printf("Number of differents measures\t%d\n", diff);

	fclose(input1);
	fclose(input2);

	return 0;

}
