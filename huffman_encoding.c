 /* 
 * File:   huffman_encoding.c
 * Author: d|kargatzis
 *
 * Created on February 
 */

#include <stdio.h>
#include <stdlib.h>
#include "huffman_encoding.h"
#include "encoding.h"


void write_bitstream(huff_tree tree, int *diff_array){
	FILE *output;
	int i,num=0;

	output = fopen(file_output, "w");
	if(output == NULL) 
		printf("Unable to create file.");

	for (i=0;i<block_size;i++){
		if(diff_array[i] == -222)
			break;
		if(abs(diff_array[i])>15 && case_A == 0){
			fprintf(output, "%d\n", diff_array[i]);
			huffman_bits++;
			continue;
		}
		found_node = init_node(found_node);
		found = 0;
		search_huff_tree(tree->root, diff_array[i]);
		int j=0;
		while(j < found_node->pointer && found == 1){
			num++;
			fprintf(output, "%d\n", found_node->bin[j]);
			huffman_bits++;
			j++;
		}
		//fprintf(output, "\n" );
		//print_node(found_node);
		//free(found_node);
	}
	fclose(output);
}

void decode_bitstream(huff_tree tree){
	FILE *input1,*validation;
	int bit;
	int tmp = 0;
	node current;

	current = init_node(current);
	current = tree->root;

	validation = fopen(valid, "a");
	if(validation == NULL)
		printf("Unable to create file.");

	input1 = fopen(file_output,"r");
	if(input1 == NULL)
		printf("Unable to open file.");

	while(fscanf(input1,"%d",&bit) == 1){
		if(bit != 0 && bit != 1){
			tmp = tmp + bit;
			fprintf(validation, "%d\n", tmp);
			continue;
		}
		if(current->left_node !=NULL && current->right_node != NULL){
			if(bit == 0)
				current = current->left_node;
			else
				current = current->right_node;
		}
		if(current->left_node == NULL && current->right_node == NULL){
			//print_node(current);
			fprintf(validation, "%d\n", tmp + current->value);
			tmp = tmp + current->value;

			current = tree->root;
			
		}
	}
	fclose(validation);
	fclose(input1);
		
			
}

void print_menu() {
		printf("\nData compression program:\n");
		printf("----------------------------------------------\n\n");
		printf("\t1. Huffman encoding.\n");
		printf("\t2. Residual encoding.\n");
		printf("\t3. Golomb rice encoding.\n");
		printf("\t4. Golomb rice exponential encoding.\n\n");
		printf("----------------------------------------------\n\n");
		printf("Please give a choise:\n");

	}

int main(int argc, char const *argv[])
{
	printf("\nInput training data file name:\t");
	scanf("%s",&file_input);
	
	printf("\nInput validation file name:\t");
	scanf("%s",&valid);

	print_menu();
	int choise;
	scanf("%d", &choise);
	FILE *input;
	int mean_v, s;

	int i,measure,array[block_size],diff_array[block_size];
	int eof=0;			/*		END OF FILE     */

	input = fopen(file_input, "r");
	if(input == NULL) 
		printf("Unable to open file.");

	//printf("\nData compression program started!\n");
	//printf("----------------------------------\n\n");
	
	while(1){


		huff_heap heap;
		huff_tree h_tree;
		
		//printf("\tInitializing heap...\n");
		heap = init_heap(heap);
		//print_heap(heap);
		//printf("\tInitializing huffman tree...\n");
		h_tree = init_huff_tree(h_tree);
		//printf("\tReading training data from file...\n");
		for (i=0;i<block_size;i++){
			if((eof=fscanf(input,"%d",&measure)) != 1){
				diff_array[i] = -222;
				break;
			}
			
			array[i] = measure;			
			//printf("%d\n", array[i]);
			if(i==0)
				diff_array[i] = array[i];
			else
				diff_array[i] = array[i] - array[i-1];
			
			if(case_A == 1){
				insert_value_to_heap(diff_array[i],heap);
			}
			else if(case_A == 0){
				if(abs(diff_array[i])<=15)
					insert_value_to_heap(diff_array[i],heap);
			}

			
		}
		if(eof != 1 && i < 1)
			break;

		if (choise == RES) {
			residual_bits = 0;
			printf("\n\tResidual Mapping Table:\n");
			printf("\t-------------------------\n\n");
			residual_encode(diff_array, i);
			printf("\n");
			printf("Total written residual bits:\t%d\n", residual_bits);
			decode_residual_bitstream();
		}
		//print_heap(heap);
		
		if (choise == GOL) {
			golomb_bits = 0;
			printf("\n\tGolomb-Rice Mapping Table:\n");
			printf("\t----------------------------\n\n");
			golomb_rice_encode(diff_array, i, 4);
			printf("\n");
			//printf("End of golomb-rice encoding.\n");
			printf("Total written golomb bits:\t%d\n", golomb_bits);
			decode_golomb_bitstream(4);
		}

		if (choise == GOL_EXP) {
			exp_golomb_bits = 0;
			printf("\n\tGolomb-Rice Exponential Mapping Table:\n");
			printf("\t----------------------------------------\n\n");
			golomb_rice_exp_encode(diff_array,i);
			printf("\n");
			printf("Total written exponential golomb bits:\t%d\n", exp_golomb_bits);
			decode_golomb_exp_bitstream();
		}

		//mean_v = array_mean_value(diff_array);
		//s = array_s(diff_array, mean_v);

		//printf("************************* Counter: %d\n", heap->counter);

		//printf("\tBuilding huffman tree...\n");
		if (choise == HUFF){
			huffman_bits = 0;
			build_wide_huff_tree(h_tree, heap);	
			//print_huff_tree_preorder(h_tree->root);

			int j;
			for (j=0; j < K_BITS; j++) {
				h_tree->root->bin[j] = EMPTY_CELL;
				//printf("CELL: %d\n", h_tree->root->bin[j]);
			}

			h_tree->root->pointer = 0;
			//printf("\tUpdating huffman tree...\n");
			update_huff_tree(h_tree->root, h_tree, h_tree->root->bin, h_tree->root->pointer, BIT_0);
			printf("\n\tHuffman Mapping Table:\n");
			printf("\t------------------------\n\n");
			print_huff_tree_codes(h_tree->root);
			printf("\n");

			//printf("\tWriting the encoded file...\n");
			write_bitstream(h_tree, diff_array);
			printf("Total written huffman bits:\t%d\n", huffman_bits);

			//printf("\tDecoding procedure...\n");
			decode_bitstream(h_tree);
			//printf("\tFree huffman tree...\n");
			free_huff_tree(h_tree->root);

		}
		if(eof != 1)
			break;
	}

	printf("Data compression program successfully finished...\n\n");


	fclose(input);

	

	return 0;
}
