#include <stdlib.h>
#include <stdio.h>
#include "helpers_disasm.h"

/*
 * Given a pointer to a buffer of size 2, return the first nibble in the first
 * element, and the second in the second element.
 */ 

extern int decode(long program_ctr, unsigned char* buf) {

	unsigned char first_byte = buf[program_ctr];
	unsigned char second_byte = buf[program_ctr+1];

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
		case 0x00:
					if (second_nibble == 0x00 && second_byte == 0xE0) {
						fprintf(stdout, "disp_clear()\n");

					} else if (second_nibble == 0x00 && second_byte == 0xEE) {
						fprintf(stdout, "return;\n");

					} else {
						if (addr != 0x000) {
							fprintf(stdout, "call machine code routine at 0x%.03X\n", addr);
						}
					}
					break;

		case 0x01:
					fprintf(stdout, "goto 0x%.03X\n", addr);
					break;

		case 0x02:
					fprintf(stdout, "*(0x%.03X)()\n", addr);
					break;

		case 0x03:
					fprintf(stdout, "if (V%.01X == 0x%.02X)\n", second_nibble, second_byte);
					break;

		case 0x04:
					fprintf(stdout, "if (V%.01X != 0x%.02X)\n", second_nibble, second_byte);
					break;

		case 0x05:
					if (fourth_nibble == 0x00) {
						fprintf(stdout, "if (V%.01X == V%.01X)\n", second_nibble, third_nibble);
					}
					break;

		case 0x06:
					fprintf(stdout, "V%.01X = 0x%.02X\n", second_nibble, second_byte);
					break;

		case 0x07:
					fprintf(stdout, "V%.01X += 0x%.02X\n", second_nibble, second_byte);
					break;

		case 0x08:
					if (fourth_nibble == 0x00) {
						fprintf(stdout, "V%.01X = V%.01X\n", second_nibble, third_nibble);
					} else if (fourth_nibble == 0x01) {
						fprintf(stdout, "V%.01X |= V%.01X\n", second_nibble, third_nibble);
					} else if (fourth_nibble == 0x02) {
						fprintf(stdout, "V%.01X &= V%.01X\n", second_nibble, third_nibble);
					} else if (fourth_nibble == 0x03) {
						fprintf(stdout, "V%.01X ^= V%.01X\n", second_nibble, third_nibble);
					} else if (fourth_nibble == 0x04) {
						fprintf(stdout, "V%.01X += V%.01X\n", second_nibble, third_nibble);
					} else if (fourth_nibble == 0x05) {
						fprintf(stdout, "V%.01X -= V%.01X\n", second_nibble, third_nibble);
					} else if (fourth_nibble == 0x06) {
						fprintf(stdout, "V%.01X >>= 1\n", second_nibble);
					} else if (fourth_nibble == 0x07) {
						fprintf(stdout, "V%.01X = V%.01X - V%.01X\n", second_nibble, third_nibble, second_nibble);
					} else if (fourth_nibble == 0x0E) {
						fprintf(stdout, "V%.01X <<= 1\n", second_nibble);
					}
					break;

		case 0x09:
					if (fourth_nibble == 0x00) {
						fprintf(stdout, "if (V%.01X != V%.01X\n", second_nibble, third_nibble);
					}
					break;

		case 0x0A:
					fprintf(stdout, "I = 0x%.03X\n", addr);
					break;

		//case 0x0B:
		//			// TODO
		//			break;

		default:
					unimplemented_op = (int)first_byte << 8;
					unimplemented_op = unimplemented_op | second_byte;
					fprintf(stdout, "Unimplemented opcode: 0x%.04X\n", unimplemented_op);
					break;
	}

	return 0;
}
