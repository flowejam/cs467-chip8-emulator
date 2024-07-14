#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "helpers_emulate.h"

/*
 * Given a pointer to a buffer of size 2, return the first nibble in the first
 * element, and the second in the second element.
 */ 

static void exec_op_01(StateStructChip8* state, OpcodeStruct* opstruct) {
	fprintf(stdout, "executing: goto 0x%.03X\n", opstruct->addr);
	state->program_ctr = opstruct->addr;
}

extern int decode_and_execute(StateStructChip8* state) {

	unsigned char first_byte = state->mem[state->program_ctr];
	unsigned char second_byte = state->mem[(state->program_ctr)+1];

	unsigned char first_nibble = first_byte >> 4;
	unsigned char second_nibble = first_byte & ~0xF0;
	unsigned char third_nibble = second_byte >> 4;
	unsigned char fourth_nibble = second_byte & ~0xF0;

	// print the address in the program counter, followed by the 2 byte opcode
	fprintf(stdout, "0%.03lX ", state->program_ctr);
	fprintf(stdout, "0x%.02X 0x%.02X ", first_byte, second_byte); 

	int addr = 0;
	addr = (int)second_nibble << 8;
	addr = addr | ((int)third_nibble << 4);
	addr = addr | fourth_nibble;

	OpcodeStruct opcode_struct = {
		.first_byte = first_byte,
		.second_byte = second_byte,
		.first_nibble = first_nibble,
		.second_nibble = second_nibble,
		.third_nibble = third_nibble,
		.fourth_nibble = fourth_nibble,
		.addr = (uint16_t)addr
	};

	//int unimplemented_op = 0;
	switch(first_nibble) {
		case 0x00:
					if (second_nibble == 0x00 && second_byte == 0xE0) {
						fprintf(stdout, "disp_clear()\n");

					} else if (second_nibble == 0x00 && second_byte == 0xEE) {
						fprintf(stdout, "return;\n");

					} else {
						if (addr != 0x000) {
							fprintf(stdout, "call machine code routine at 0x%.03X\n", addr);
						} else {
							fprintf(stdout, "\n");
						}
					}
					break;

		case 0x01:
					exec_op_01(state, &opcode_struct);
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
					} else {
						fprintf(stdout, "\n");
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
					} else {
						fprintf(stdout, "\n");
					}
					break;

		case 0x09:
					if (fourth_nibble == 0x00) {
						fprintf(stdout, "if (V%.01X != V%.01X\n", second_nibble, third_nibble);
					} else {
						fprintf(stdout, "\n");
					}
					break;

		case 0x0A:
					fprintf(stdout, "I = 0x%.03X\n", addr);
					break;

		case 0x0B:
					fprintf(stdout, "PC = V0 + 0x%.03X\n", addr);
					break;

		case 0x0C:
					fprintf(stdout, "V%.01X = rand() & 0x%.02X\n", second_nibble, second_byte);
					break;

		case 0x0D:
					fprintf(stdout, "draw(V%.01X, V%.01X, 0x%.01X)\n", second_nibble, third_nibble, fourth_nibble);
					break;

		case 0x0E:
					if (second_byte == 0x9E) {
						fprintf(stdout, "if (key() == V%.01X)\n", second_nibble);
					} else if (second_byte == 0xA1) {
						fprintf(stdout, "if (key() != V%.01X)\n", second_nibble);
					} else {
						fprintf(stdout, "\n");
					}
					break;

		case 0x0F:
					if (second_byte == 0x07) {
						fprintf(stdout, "V%.01X = get_delay()\n", second_nibble);
					} else if (second_byte == 0x0A) {
						fprintf(stdout, "V%.01X = get_key()\n", second_nibble);
					} else if (second_byte == 0x15) {
						fprintf(stdout, "delay_timer(V%.01X)\n", second_nibble);
					} else if (second_byte == 0x18) {
						fprintf(stdout, "sound_timer(V%.01X)\n", second_nibble);
					} else if (second_byte == 0x1E) {
						fprintf(stdout, "I += V%.01X\n", second_nibble);
					} else if (second_byte == 0x29) {
						fprintf(stdout, "I = sprite_addr[V%.01X]\n", second_nibble);
					} else if (second_byte == 0x33) {
						fprintf(stdout, "set_BCD(V%.01X)\n*(I+0) = BCD(3);\n*(I+1) = BCD(2);\n*(I+2) = BCD(1);\n", second_nibble);
					} else if (second_byte == 0x55) {
						fprintf(stdout, "reg_dump(V%.01X, &I)\n", second_nibble);
					} else if (second_byte == 0x65) {
						fprintf(stdout, "reg_load(V%.01X, &I)\n", second_nibble);
					} else {
						fprintf(stdout, "\n");
					}
					break;

		default:
					//unimplemented_op = (int)first_byte << 8;
					//unimplemented_op = unimplemented_op | second_byte;
					//fprintf(stdout, "Unimplemented opcode: 0x%.04X\n", unimplemented_op);
					fprintf(stdout, "\n");
					break;
	}

	return 0;
}
