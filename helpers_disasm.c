#include <stdlib.h>
#include <stdio.h>
#include "helpers_disasm.h"

/*
 * Given a pointer to a buffer of size 2, return the first nibble in the first
 * element, and the second in the second element.
 */ 

extern int decode(size_t nread, unsigned char* buf) {
	if (nread < 2) {
		fprintf(stderr, "Error in func decode: insufficient bytes read for opcode decoding.\n");
		return -1;
	}

	unsigned char first_byte = buf[0];
	unsigned char second_byte = buf[1];
	return 0;
}
