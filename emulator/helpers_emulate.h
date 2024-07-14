#ifndef HELPERS_DISASM_H
#define HELPERS_DISASM_H
#include <stdint.h>

#ifndef HEIGHT_OF_SCREEN
#define HEIGHT_OF_SCREEN 32
#endif

#ifndef WIDTH_OF_SCREEN
#define WIDTH_OF_SCREEN 64
#endif

typedef struct RegisterStructChip8 RegisterStructChip8; 
struct RegisterStructChip8 {
	unsigned char v0;
	unsigned char v1;
	unsigned char v2;
	unsigned char v3;
	unsigned char v4;
	unsigned char v5;
	unsigned char v6;
	unsigned char v7;
	unsigned char v8;
	unsigned char v9;
	unsigned char va;
	unsigned char vb;
	unsigned char vc;
	unsigned char vd;
	unsigned char ve;
	unsigned char vf;
	uint16_t I;
};

typedef struct StateStructChip8 StateStructChip8; 
struct StateStructChip8 {
	unsigned char keyboard[16];
	unsigned char delay_timer;
	unsigned char sound_timer;
	RegisterStructChip8* regs;
	long program_ctr;
	long stack_ptr;
	unsigned char* stack;
	unsigned char* display_refresh;
	unsigned char* mem;
	unsigned char screen[HEIGHT_OF_SCREEN][WIDTH_OF_SCREEN];
};

typedef struct OpcodeStruct OpcodeStruct;
struct OpcodeStruct {
	unsigned char first_byte;
	unsigned char second_byte;
	unsigned char first_nibble;
	unsigned char second_nibble;
	unsigned char third_nibble;
	unsigned char fourth_nibble;
	uint16_t addr;
};

extern int decode_and_execute(StateStructChip8* state);

#endif
