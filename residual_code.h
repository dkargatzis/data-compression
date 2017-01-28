#ifndef _residual_code_h
#define _residual_code_h
#include "encoding.h"

void residual_encode(int *array, int array_size);


void write_residual_bitstream(huff_heap residual_heap, int *array, int array_size);


void decode_residual_bitstream();

#endif
