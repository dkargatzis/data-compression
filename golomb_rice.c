#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "encoding.h"
#include "dec_to_bin.h"
#include "huffman_encoding.h"
#include "golomb_rice.h"




void golomb_rice_encode(int *array, int array_size, int divisor)
{
	int i, rmd_bits = 0, j, glb_pointer, w, i_unary, i_rmnder, r;
	int *glb_array, *bin, *rmnder_bin;
	int quotient, rmnder;
	bin = (int *)malloc(K_BITS*sizeof(int));
	rmnder_bin = (int *)malloc(K_BITS*sizeof(int));
	huff_heap glb_heap;
	glb_heap = init_heap(glb_heap);

	for (i=0; i<array_size; i++) {
		insert_value_to_heap(array[i], glb_heap);
	}
	
	glb_array = (int *)malloc(glb_heap->counter * sizeof(int));
	i = 0;
	node current;
	current = init_node(current);
	current = glb_heap->head;
	while (current != NULL) {
		glb_array[i] = current->value;
		i++;
		current = current->next;
	}
	glb_pointer = i;
	bin = convert_to_bin(divisor);
	
	rmd_bits = ceil(log2(divisor));
	//printf("BITS: %d\n", rmd_bits);
	i=0;
	i_unary = 0;
	current = glb_heap->head;
	//printf("%d\n", glb_pointer);
	while(current != NULL) {//i<glb_pointer) {
		//printf("%d\n", i);
		for(j=0; j<divisor; j++) {
			for(w=0; w<K_BITS; w++) {
				rmnder_bin[w] = -1;
			}
			if (i+j >= glb_pointer) { 
				//printf("break!!\n");
				break;
			}
			if(current->value > 15){
				current->bin[0] = current->value;
				current = current->next;
				continue;
			}
			quotient = abs(current->value)/divisor;//abs(glb_array[i])/divisor;
			//printf("QUOTIENT: %d\n", quotient);
			rmnder = abs(current->value)%divisor;//abs(glb_array[i])%divisor;
			//printf("REMAINDER: %d\n", rmnder);
			int x;
			int *aux;
			aux = (int *)malloc(K_BITS*sizeof(int));
			int l;
			for(l=0; l<K_BITS; l++) {
				aux[l] = -1;
			}
			if (rmnder == 0) {
				for (r=0; r<rmd_bits; r++) {
					rmnder_bin[r] = 0;
				}
			}
			else {
				rmnder_bin = convert_to_bin(abs(rmnder));
				x=0;
				while (rmnder_bin[x] != -1) {
					x++;
				}
				//printf("\nX: %d\n", x);
				//printf("\nRmd_bits-x: %d\n\n", rmd_bits-x+1);
				if (x-1 < rmd_bits) {
					int h;
					for (h=0; h<(rmd_bits-x); h++){
						aux[h] = 0;
					}
					int k=0;
					//printf("YEEEEESSS\n");
					while (rmnder_bin[k] != -1) {
						aux[h+k] = rmnder_bin[k];
						k++;
					}
					for (k=0; k<K_BITS; k++) {
						rmnder_bin[k] = aux[k];
					}
				}
			}
			//printf("RMN_BIN\n");
			r = 0;
			while(rmnder_bin[r] != -1) {
				//printf("%d", rmnder_bin[r]);
				r++;
			}
			//printf("\n");
			if (current->value < 0) {
				current->bin[0] = 1;
			}
			else {
				current->bin[0] = 0;
			}
			for(w=0; w<quotient; w++){
				current->bin[w+1] = 0;
			}
			current->bin[w+1] = 1;
			i_rmnder = 0;
			while(rmnder_bin[i_rmnder] != -1) {
				current->bin[w+2+i_rmnder] = rmnder_bin[i_rmnder];
				i_rmnder++;
			}
			current = current->next;
		}
		i_unary++;
		if(i+j>glb_pointer)
			break;
		i = i+j;
	}


	current = glb_heap->head;
	int y;
	while (current != NULL) {
		printf("\t%d\t", current->value);
		y = 0;
		while(current->bin[y] != -1) {
			printf("%d", current->bin[y]);
			y++;
		}
		printf("\n");
		current = current->next;
	}

	write_golomb_bitstream(glb_heap, array, array_size);
	free(bin);
	free(rmnder_bin);
	free(glb_array);
	free_heap(glb_heap);
}


void write_golomb_bitstream(huff_heap glb_heap, int *array, int array_size){
    FILE *output;
    int i,j;
    int *bin;

    output = fopen(golomb_file_output, "w");
    if(output == NULL) 
        printf("Unable to create file.");

    bin = (int *)malloc(K_BITS * sizeof(int));

    for (i=0;i<array_size;i++){
        bin = search_value_in_heap(glb_heap, array[i]);
        for(j=0;j<K_BITS;j++){
            if(bin[j] == -1)
                break;
            fprintf(output, "%d\n", bin[j]);
            golomb_bits++;
        }

    }
    free(bin);
    fclose(output);

}


void decode_golomb_bitstream(int divisor){
	FILE *input1,*validation;
	int *bin,i=0,q,r,flag=0,sign,r_bits,tmp = 0,bit;

	r_bits = ceil(log2(divisor));

	validation = fopen(valid, "a");
	if(validation == NULL)
		printf("Unable to create file.");

	input1 = fopen(golomb_file_output,"r");
	if(input1 == NULL)
		printf("Unable to open file.");

	bin = (int *)malloc(K_BITS * sizeof(int));

	while(fscanf(input1,"%d",&bit) == 1){
		if(bit != 0 && bit != 1){
			tmp = tmp + bit;
			fprintf(validation, "%d\n", tmp);
			continue;
		}
		//bin[i] = bit;
		if(i==0){
			sign = bit;
			i++;
			continue;
		}
		if(i>0 && bit == 1 && flag == 0){
			q=i-1;
			//printf("%d\n", q);
			flag = 1;
			i++;
			continue;
		}
		if(flag == 1){
			bin[i-q-2] = bit;
			if(r_bits == i-q-1){
				int k;	
				r = 0;	
				for(k=0;k<r_bits;k++){
					if(bin[k] == 1)
						r = r *2 +1;
					else
						r *= 2;
				}
				/*if(sign ==1)
					printf("%d\n",- (q*divisor + r));
				else
					printf("%d\n", (q*divisor + r)); */
				if(sign == 1)
					tmp = tmp - (q*divisor + r);
				else
					tmp = tmp + q*divisor + r;
				fprintf(validation, "%d\n", tmp);
				i = 0;
				flag = 0;
			}
			else{
				i++;
			}
		}
		else{
			i++;
		}

	}
	free(bin);
	fclose(input1);
	fclose(validation);

}  



void golomb_rice_exp_encode(int *array, int array_size)
{
	int divisor, i_div;
	int i, rmd_bits = 0, j, glb_pointer, w, i_unary, i_rmnder, r;
	int *glb_array, *bin, *rmnder_bin;
	int quotient, rmnder;
	bin = (int *)malloc(K_BITS*sizeof(int));
	rmnder_bin = (int *)malloc(K_BITS*sizeof(int));
	huff_heap glb_heap, aux_heap;
	glb_heap = init_heap(glb_heap);
	aux_heap = init_heap(aux_heap);

	for (i=0; i<array_size; i++) {
		insert_value_to_heap(array[i], aux_heap);
	}
	int count = aux_heap->counter;
	//printf("Counter: %d\n", aux_heap->counter);
	for (i=0; i<count; i++) {
		simple_insert_to_heap(delete_from_heap(aux_heap), glb_heap);
	}
	
	glb_array = (int *)malloc(glb_heap->counter * sizeof(int));
	i = 0;
	node current;
	current = init_node(current);
	current = glb_heap->head;
	while (current != NULL) {
		glb_array[i] = current->value;
		i++;
		current = current->next;
	}
	glb_pointer = i;
	//bin = convert_to_bin(divisor);
	
	//rmd_bits = ceil(log2(divisor));
	//printf("BITS: %d\n", rmd_bits);
	i=0;
	i_div = 0;
	i_unary = 0;
	current = glb_heap->head;
	//printf("%d\n", glb_pointer);
	while(current != NULL) {//i<glb_pointer) {
		divisor = pow(2, i_div);
		rmd_bits = ceil(log2(divisor));
		//printf("%d\n", i);
		for(j=0; j<divisor; j++) {
			for(w=0; w<K_BITS; w++) {
				rmnder_bin[w] = -1;
			}
			if (i+j >= glb_pointer) { 
				//printf("break!!\n");
				break;
			}
			if(current->value > 15){
				current->bin[0] = current->value;
				current = current->next;
				continue;
			}

			if (divisor == 1) {
				//printf("------------------------------DIVIDOR1\t %d\n", current->value);
				rmnder_bin[0] = -1;
			}
			else if (divisor == 2) {
				if (j == 0) {
					rmnder_bin[0] = 0;
				}
				else {
					rmnder_bin[0] = 1;
				}
			}
			else {
			//quotient = abs(current->value)/divisor;//abs(glb_array[i])/divisor;
			///printf("QUOTIENT: %d\n", quotient);
			//rmnder = abs(current->value)%divisor;//abs(glb_array[i])%divisor;
			//printf("REMAINDER: %d\n", rmnder);
				rmnder = current->value%(divisor-1);
				int x;
				int *aux;
				aux = (int *)malloc(K_BITS*sizeof(int));
				int l;
				for(l=0; l<K_BITS; l++) {
					aux[l] = -1;
				}
				if (rmnder == 0) {
					for (r=0; r<rmd_bits; r++) {
						rmnder_bin[r] = 0;
					}
				}
				else {
					rmnder_bin = convert_to_bin(abs(rmnder));
					x=0;
					while (rmnder_bin[x] != -1) {
						x++;
					}
					//printf("\nX: %d\n", x);
					//printf("\nRmd_bits-x: %d\n\n", rmd_bits-x+1);
					if (x-1 < rmd_bits) {
						int h;
						for (h=0; h<(rmd_bits-x); h++){
							aux[h] = 0;
						}
						int k=0;
						//printf("YEEEEESSS\n");
						while (rmnder_bin[k] != -1) {
							aux[h+k] = rmnder_bin[k];
							k++;
						}
						for (k=0; k<K_BITS; k++) {
							rmnder_bin[k] = aux[k];
						}
					}
				}
			}
			//printf("RMN_BIN\n");
			r = 0;
			while(rmnder_bin[r] != -1) {
				//printf("%d", rmnder_bin[r]);
				r++;
			}
			//printf("\n");
			if (current->value < 0) {
				current->bin[0] = 1;
			}
			else {
				current->bin[0] = 0;
			}
			for(w=0; w<rmd_bits; w++){
				current->bin[w+1] = 0;
			}
			current->bin[w+1] = 1;
			i_rmnder = 0;
			while(rmnder_bin[i_rmnder] != -1) {
				current->bin[w+2+i_rmnder] = rmnder_bin[i_rmnder];
				i_rmnder++;
			}
			current = current->next;
		}
		i_unary++;
		
		if(i+j>glb_pointer)
			break;
		i_div++;
		i = i+j;
	}


	current = glb_heap->head;
	int y;
	while (current != NULL) {
		printf("\t%d\t", current->value);
		y = 0;
		while(current->bin[y] != -1) {
			printf("%d", current->bin[y]);
			y++;
		}
		printf("\n");
		current = current->next;
	}
	//print_heap(glb_heap);
	//free_heap(glb_heap);

	write_exp_golomb_bitstream(glb_heap, array, array_size);
	free(bin);
	free(rmnder_bin);
	free(glb_array);
	free_heap(glb_heap);
}

void write_exp_golomb_bitstream(huff_heap glb_heap, int *array, int array_size){
    FILE *output;
    int i,j;
    int *bin;

    output = fopen(golomb_exp_file_output, "w");
    if(output == NULL) 
        printf("Unable to create file.");

    bin = (int *)malloc(K_BITS * sizeof(int));

    for (i=0;i<array_size;i++){
        bin = search_value_in_heap(glb_heap, array[i]);
        for(j=0;j<K_BITS;j++){
            if(bin[j] == -1)
                break;
            fprintf(output, "%d\n", bin[j]);
            exp_golomb_bits++;
        }

    }
    free(bin);
    fclose(output);

}


void decode_golomb_exp_bitstream(){
	FILE *input1,*validation;
	int *bin,sign,tmp = 0,bit;
	int i=0,q,w=0,r,flag = 0;


	validation = fopen(valid, "a");
	if(validation == NULL)
		printf("Unable to create file.");

	input1 = fopen(golomb_exp_file_output,"r");
	if(input1 == NULL)
		printf("Unable to open file.");

	bin = (int *)malloc(K_BITS * sizeof(int));

	while(fscanf(input1,"%d",&bit) == 1){
		if(bit != 0 && bit != 1){
			tmp = tmp + bit;
			fprintf(validation, "%d\n", tmp);
			continue;
		}
		if(i == 0){
			sign = bit;
			i++;
			continue;
		}
		if(i>0 && bit == 1 && flag == 0){
			q=i-1;
			//printf("%d\n", q);
			flag = 1;
		
			if(q == 0 && flag == 1 && i == 1){
				fprintf(validation, "%d\n", tmp);
				flag = 0;
				i=0;
				continue;
			}
			i++;
			continue;
		}
		if(q == 1 && flag == 1){
			if(bit == 0 && sign == 0){
				tmp = tmp + 1;
			}
			else{
				tmp = tmp - 1;
			}
			fprintf(validation, "%d\n", tmp);
			flag = 0;
			i=0;
			continue;
		}
		if(flag == 1){
			bin[w] = bit;
			if(w == q - 1){
				int k;	
				r = 0;	
				for(k=0;k<q;k++){
					if(bin[k] == 1)
						r = r *2 +1;
					else
						r *= 2;
				}
				if(sign == 0)
					tmp = tmp + pow(2,q-1) -1 +r;
				else
					tmp = tmp - (pow(2,q-1) -1 +r);
			
				fprintf(validation, "%d\n", tmp);
				w =0;
				i = 0;
				flag = 0;
			}
			else{
				w++;
				i++;
			}
		}
		else{
			i++;
		}

	}
	free(bin);
	fclose(input1);
	fclose(validation);
}
