#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "encoding.h"
#include "dec_to_bin.h"
#include "huffman_encoding.h"


int *convert_to_bin(int dec)
{
	int *bin;
	bin = (int *)malloc(K_BITS * sizeof(int));
	int *temp;
	temp = (int *)malloc(K_BITS * sizeof(int));
	int i,j,t=0;
	for (i=0; i<K_BITS; i++) {
		bin[i] = -1;
		temp[i] = -1;
	}
	i = 0;
	while (dec > 0) {
		bin[i] = dec%2;
		i++;
		dec = dec/2;
	}

	for(j=i-1;j>=0;j--){
		temp[t] = bin[j];
		t++;
	}
	return temp;
	
}


int convert_to_dec(int *bin)
{
	int s = 0, i = 0,k;
	while(bin[i] != -1)
		i++;

	for(k=0;k<i;k++){
			if(bin[k] == 1)
				s = s *2 +1;
			else
				s *= 2;
		}
	return s;
}




