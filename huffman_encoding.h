#ifndef _huffman_encoding_h
#define _huffman_encoding_h
#include "encoding.h"
#include "laplace.h"


//#define file_input  			"COMP516coursedoc_32.txt"
#define file_output  			"bitstream.txt"
#define residual_file_output  		"residual_bitstream.txt"
#define golomb_file_output  		"golomb_bitstream.txt"
#define golomb_exp_file_output  		"golomb_exp_bitstream.txt"
#define block_size 			22094
//int block_size;
char file_input[100],valid[100];

int huffman_bits;
int residual_bits;
int golomb_bits;
int exp_golomb_bits;

#define case_A  	1

#define HUFF 1
#define RES 2
#define GOL 3
#define GOL_EXP 4

void write_bitstream(huff_tree tree, int *diff_array);


void decode_bitstream(huff_tree tree);


#endif
