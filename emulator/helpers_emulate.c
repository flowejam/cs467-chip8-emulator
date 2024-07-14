#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "helpers_emulate.h"

/*
 * Accepts values from 0x01-0x0F for registers v0-vf
 */
static void set_reg(StateStructChip8* state, unsigned char reg_val, unsigned char set_val) {
	RegisterStructChip8* registers = state->regs;

	switch(reg_val) {
		case 0:
			registers->v0 = set_val;
			break;
		case 1:
			registers->v1 = set_val;
			break;
		case 2:
			registers->v2 = set_val;
			break;
		case 3:
			registers->v3 = set_val;
			break;
		case 4:
			registers->v4 = set_val;
			break;
		case 5:
			registers->v5 = set_val;
			break;
		case 6:
			registers->v6 = set_val;
			break;
		case 7:
			registers->v7 = set_val;
			break;
		case 8:
			registers->v8 = set_val;
			break;
		case 9:
			registers->v9 = set_val;
			break;
		case 0x0A:
			registers->va = set_val;
			break;
		case 0x0B:
			registers->vb = set_val;
			break;
		case 0x0C:
			registers->vc = set_val;
			break;
		case 0x0D:
			registers->vd = set_val;
			break;
		case 0x0E:
			registers->ve = set_val;
			break;
		case 0x0F:
			registers->vf = set_val;
			break;
		default:
			fprintf(stderr, "Error in func get_reg: register matching value does not exist.\n");
			break;
	}
}

/*
 * Accepts values from 0x01-0x0F for registers v0-vf
 */
static unsigned char get_reg(StateStructChip8* state, unsigned char val) {
	RegisterStructChip8* registers = state->regs;

	unsigned char res = 0;

	switch(val) {
		case 0:
			res =  registers->v0;
			break;
		case 1:
			res = registers->v1;
			break;
		case 2:
			res = registers->v2;
			break;
		case 3:
			res = registers->v3;
			break;
		case 4:
			res = registers->v4;
			break;
		case 5:
			res = registers->v5;
			break;
		case 6:
			res = registers->v6;
			break;
		case 7:
			res = registers->v7;
			break;
		case 8:
			res = registers->v8;
			break;
		case 9:
			res = registers->v9;
			break;
		case 0x0A:
			res = registers->va;
			break;
		case 0x0B:
			res = registers->vb;
			break;
		case 0x0C:
			res = registers->vc;
			break;
		case 0x0D:
			res = registers->vd;
			break;
		case 0x0E:
			res = registers->ve;
			break;
		case 0x0F:
			res = registers->vf;
			break;
		default:
			fprintf(stderr, "Error in func get_reg: register matching value does not exist.\n");
			break;
	}
	return res;
}

static void push_stack(StateStructChip8* state, uint16_t val) {
	// CHIP-8 is big endian, so assume the MSB gets pushed first
	unsigned char msb = (val & 0xF00) >> 8;
	unsigned char lsb = val & 0x0FF;

	state->stack[state->stack_ptr] = msb;
	++(state->stack_ptr);
	state->stack[state->stack_ptr] = lsb;
	++(state->stack_ptr);
}

static uint16_t pop_stack(StateStructChip8* state) {
	// CHIP-8 is big endian, so assume the MSB gets pushed first
	unsigned char top_byte = state->stack[state->stack_ptr];
	--(state->stack_ptr);
	unsigned char bottom_byte = state->stack[state->stack_ptr];
	--(state->stack_ptr);

	uint16_t ret_val = bottom_byte;
	ret_val <<= 8;
	ret_val |= top_byte;
	return ret_val;
}	

static void exec_op_01(StateStructChip8* state, OpcodeStruct* opstruct) {
	fprintf(stdout, "executing: goto 0x%.03X\n", opstruct->addr);
	state->program_ctr = opstruct->addr;
}

static void exec_op_00(StateStructChip8* state, OpcodeStruct* opstruct) {
	if (opstruct->second_nibble == 0x00 && opstruct->second_byte == 0xE0) {
		fprintf(stdout, "executing: disp_clear()\n");
		for (int i = 0; i < HEIGHT_OF_SCREEN; ++i) {
			for (int j = 0; j < WIDTH_OF_SCREEN; ++j) {
				state->screen[i][j] = 0;
			}	
		}
	
	} else if (opstruct->second_nibble == 0x00 && opstruct->second_byte == 0xEE) {
		fprintf(stdout, "return;\n");
		uint16_t ret_addr = pop_stack(state);
		state->program_ctr = ret_addr;
	
	} else {
		// do nothing - this is for the RCA 1802 for COSMAC
		return;
	}
}

static void exec_op_02(StateStructChip8* state, OpcodeStruct* opstruct) {
	fprintf(stdout, "executing: *(0x%.03X)()\n", opstruct->addr);
	push_stack(state, opstruct->addr);
	state->program_ctr = opstruct->addr;
}

static void exec_op_03(StateStructChip8* state, OpcodeStruct* opstruct) {
	fprintf(stdout, "executing: if (V%.01X == 0x%.02X)\n", opstruct->second_nibble, opstruct->second_byte);
	if (get_reg(state, opstruct->second_nibble) == opstruct->second_byte) {
		state->program_ctr += 2;
	}
}

static void exec_op_04(StateStructChip8* state, OpcodeStruct* opstruct) {
	fprintf(stdout, "executing: if (V%.01X != 0x%.02X)\n", opstruct->second_nibble, opstruct->second_byte);
	if (get_reg(state, opstruct->second_nibble) != opstruct->second_byte) {
		state->program_ctr += 2;
	}
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
					exec_op_00(state, &opcode_struct);
					break;

		case 0x01:
					exec_op_01(state, &opcode_struct);
					break;

		case 0x02:
					exec_op_02(state, &opcode_struct);
					break;

		case 0x03:
					exec_op_03(state, &opcode_struct);
					break;

		case 0x04:
					exec_op_04(state, &opcode_struct);
					break;

		case 0x05:
					// TODO
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
