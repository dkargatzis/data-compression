#ifndef _golomb_rice_h
#define _golomb_rice_h

void golomb_rice_encode(int *array, int array_size, int divisor);

void write_golomb_bitstream(huff_heap residual_heap, int *array, int array_size);

void golomb_rice_exp_encode(int *array, int array_size);

void decode_golomb_bitstream(int divisor);

void write_exp_golomb_bitstream(huff_heap glb_heap, int *array, int array_size);

void decode_golomb_exp_bitstream();

#endif