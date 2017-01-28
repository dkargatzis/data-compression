
#include <stdio.h>
#include <stdlib.h>
#include "residual_code.h"
#include "huffman_encoding.h"
#include "dec_to_bin.h"
#include "encoding.h"

void residual_encode(int *array, int array_size){ 
	huff_heap residual_heap;
			
	//printf("\tInitializing heap...\n");
	residual_heap = init_heap(residual_heap);

	int i,j;
	for(i=0;i<array_size;i++)
		insert_value_to_heap(array[i], residual_heap);
	
	node current;
    current = init_node(current);
    current = residual_heap->head;
    
    int *bin,*res_bin,sign,zeros;
	bin = (int *)malloc(K_BITS * sizeof(int));
	res_bin = (int *)malloc(K_BITS * sizeof(int));

    while (current != NULL){
    	if(current->value < 0)
    		sign = 1;
    	else 
    		sign =0;

    	for(i=0; i < K_BITS; i++)
    		res_bin[i] = -1;

        bin=convert_to_bin(abs(current->value));
        for(i=0; i < K_BITS; i++){
        	if(bin[i] == -1)
            	break;
        }

        for(j=0;j<K_BITS;j++){
          	if(bin[j] == -1)
            	break;
          	//printf("%d", bin[j]);
        }

        if(current->value >= 15){
          	res_bin[0] = current->value;
        }
        else{
          	res_bin[0] = sign;
          	if(current->value == 0){
          		res_bin[1] = 0;
          		res_bin[2] = 0;
          		res_bin[3] = 1;
          	}
          	else if(i<2){
          		res_bin[1] = 0;
          		res_bin[2] = bin[i-1];
          		res_bin[3] = 1;
          	}
          	else{
          		res_bin[1] = bin[i-2];
          		res_bin[2] = bin[i-1];
          		bin[i-1] = -1;
          		bin[i-2] = -1;
          		zeros = convert_to_dec(bin);
          		for(j=0;j<zeros;j++)
          			res_bin[j+3] = 0;
          		res_bin[j+3] = 1;
          	}
        }
        /* Print residual code - binary - value   */
       	printf("\t%d\t", current->value); 
        for(j=0;j<K_BITS;j++){
            if(res_bin[j] == -1)
                break;
            printf("%d", res_bin[j]);
        }
        printf("\n");
        
       	
       	/*		End of printing  	*/

       	for(i=0; i < K_BITS; i++){
            if(res_bin[i] == -1)
        		    break;
       	    current->bin[i] = res_bin[i];
        }
        current = current->next;
    }
    //print_heap(residual_heap);
    write_residual_bitstream(residual_heap, array, array_size);
    free_heap(residual_heap);
    free(bin);
    free(res_bin);

}

void write_residual_bitstream(huff_heap residual_heap, int *array, int array_size){
    FILE *output;
    int i,j;
    int *bin;

    output = fopen(residual_file_output, "w");
    if(output == NULL) 
        printf("Unable to create file.");


    bin = (int *)malloc(K_BITS * sizeof(int));

    for (i=0;i<array_size;i++){
        bin = search_value_in_heap(residual_heap, array[i]);
        for(j=0;j<K_BITS;j++){
            if(bin[j] == -1)
                break;
            fprintf(output, "%d\n", bin[j]);
            residual_bits++;
        }

    }
    free(bin);
    fclose(output);

}

void decode_residual_bitstream(){
	FILE *input1,*validation;
	int i=0,j=0,*bin,bit,temp[3];
	int value, tmp = 0;;

	printf("decoding...\n");

	validation = fopen(valid, "a");
	if(validation == NULL)
		printf("Unable to create file.");

	input1 = fopen(residual_file_output,"r");
	if(input1 == NULL)
		printf("Unable to open file.");

	bin = (int *)malloc(K_BITS * sizeof(int));

	while(fscanf(input1,"%d",&bit) == 1){
		if(bit != 0 && bit != 1){
			tmp = tmp + bit;
			fprintf(validation, "%d\n", tmp);
			continue;
		}
		bin[j] = bit;
		if(j == 3 && bit == 0){
			j++;
			continue;
		}
		else if(j == 3 && bit == 1){
			temp[2] = bin[2];
			temp[1] = bin[1];
			temp[0]	= 0;
			j = 0;
		}
		else if(j == 4){
			temp[2] = bin[2];
			temp[1] = bin[1];
			temp[0] = 1;
			j = 0;
		}
		else{
			j++;
			continue;
		}
		int k;
	
		value = 0;	
		for(k=0;k<3;k++){
			if(temp[k] == 1)
				value = value *2 +1;
			else
				value *= 2;
		}

		if(bin[0] == 0)
			tmp = tmp + value;
		else
			tmp = tmp - value;
		fprintf(validation, "%d\n", tmp);
	}
  free(bin);
	fclose(input1);
	fclose(validation);
}