#include <stdlib.h>
#include <stdio.h>
#include "helpers_disasm.h"

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

	//unsigned char buf[_BUF_SIZE_] = {0};

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

	unsigned char* buf = malloc(end_offset + 0x200);
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

	long program_ctr = 0x200;
	while (program_ctr < (end_offset+0x200)) {
		int decode_result = decode(program_ctr, buf);
		if (decode_result < 0) {
			goto CLEANUP;
		}
		program_ctr += 2;
	}

	//for (;;) {
	//for (int i = 0; i < 10; ++i) {
	//	if (feof(fp)) {
	//		break;
	//	}

	//	size_t nread = fread(buf, 1, sizeof(buf), fp);
	//	if (ferror(fp)) {
	//		fprintf(stderr, "Error in func main: an error occurred while reading from the given file.\n");
	//		goto CLEANUP;
	//	}

	//	int decode_result = decode(nread, buf);
	//	if (decode_result < 0) {
	//		goto CLEANUP;
	//	}

	//}

CLEANUP:;
	free(buf);
	fclose(fp);
	EXIT_SUCCESS;
}
