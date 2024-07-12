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

	unsigned char first_nibble = first_byte >> 4;
	unsigned char second_nibble = first_byte & ~0xF0;
	unsigned char third_nibble = second_byte >> 4;
	unsigned char fourth_nibble = second_byte & ~0xF0;

	fprintf(stdout, "first, second byte: 0x%.02X, 0x%.02X\n", first_byte, second_byte); 

	int addr = 0;
	addr = (int)second_nibble << 8;
	addr = addr | ((int)third_nibble << 4);
	addr = addr | fourth_nibble;
	int unimplemented_op = 0;
	switch(first_nibble) {
		case 0x0A:
					fprintf(stdout, "I = 0x%.03X\n", addr);
					break;
		case 0x01:
					fprintf(stdout, "goto 0x%.03X\n", addr);
					break;
		default:
					unimplemented_op = (int)first_byte << 8;
					unimplemented_op = unimplemented_op | second_byte;
					fprintf(stdout, "Unimplemented opcode: 0x%.04X\n", unimplemented_op);
					break;
	}

	return 0;
}
