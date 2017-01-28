#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "encoding.h"
#include "huffman_encoding.h"
#include "laplace.h"



int test()
{
	printf("Laplace Distribution!\n");
	return 0;
}


int array_mean_value(int *array)
{
	int s = 0;
	int i;
	for (i=0; i<block_size; i++) {
		s = s + array[i];
		//printf("%d\t%d\n", i, s);
	}
	return (s/block_size);
}

int array_s(int *array, int mean)
{
	int s2, s;
	int sum = 0;
	int i;
	for (i=0; i<block_size; i++) {
		sum = sum + ((array[i] - mean)^2);
	}
	s2 = sum/block_size;
	s = sqrt(s2);
	return s;
}