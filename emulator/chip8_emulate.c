// TODO: include sources

#include <stdlib.h>
#include <stdio.h>
#include "helpers_emulate.h"

#ifndef _BUF_SIZE_
#define _BUF_SIZE_ 3
#endif

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s _file_name_\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const char* fn = argv[1];
	FILE* fp = fopen(fn, "r");
	if (!fp) {
		fprintf(stderr, "Error in func main: opening the file %s caused an error\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	long initial_offset = ftell(fp);
	if (initial_offset < 0) {
		fprintf(stderr, "Error in func main: error encountered when calling ftell.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "initial_offset: %ld\n", initial_offset); 

	// seek to end of file
	int seek_res = fseek(fp, 0L, SEEK_END);
	if (seek_res < 0) {
		fprintf(stderr, "Error in func main: error encountered when calling fseek.\n");
		exit(EXIT_FAILURE);
	}

	long end_offset = ftell(fp);
	if (end_offset < 0) {
		fprintf(stderr, "Error in func main: error encountered when calling ftell.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "end_offset: %ld\n", end_offset); 

	// ============ start of emulation ===========================

	unsigned char* buf = calloc(4096, 1);
	if (!buf) {
		fprintf(stderr, "Error in func main: error encountered when calling malloc.\n");
		exit(EXIT_FAILURE);
	}

	// seek back to beginning of file before reading it
	seek_res = fseek(fp, 0L, SEEK_SET);
	if (seek_res < 0) {
		fprintf(stderr, "Error in func main: error encountered when calling fseek.\n");
		exit(EXIT_FAILURE);
	}

	size_t nread = fread(buf+0x200, 1, end_offset, fp);
	if (nread != (size_t)end_offset) {
		fprintf(stderr, "Error in func main: wrong number of bytes read from file.\n");
		goto CLEANUP;
	}

	if (ferror(fp)) {
		fprintf(stderr, "Error in func main: an error occurred while reading from the given file.\n");
		goto CLEANUP;
	}


	// TODO: include reference
	// The CHIP-8 interpreter takes up the first 512 bytes of memory.
	RegisterStructChip8 registers = {0};
	StateStructChip8 state_chip8 = {
		.regs = &registers, 
		.program_ctr = 0x200, 
		.mem = buf,
	};

	state_chip8.display = &state_chip8.mem[0xF00];
	state_chip8.stack = &state_chip8.mem[0xEA0];
	state_chip8.stack_ptr = 0;

	while (state_chip8.program_ctr < (end_offset+0x200)) {
		int res = decode_and_execute(&state_chip8);
		if (res < 0) {
			goto CLEANUP;
		}
		state_chip8.program_ctr += 2;
	}


CLEANUP:;
	free(buf);
	fclose(fp);
	EXIT_SUCCESS;
}
