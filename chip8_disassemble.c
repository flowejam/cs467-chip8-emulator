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
	}

	unsigned char buf[_BUF_SIZE_] = {0};
	for (;;) {
		if (feof(fp)) {
			break;
		}

		size_t nread = fread(buf, 1, sizeof(buf), fp);
		if (ferror(fp)) {
			fprintf(stderr, "Error in func main: an error occurred while reading from the given file.\n");
			goto CLEANUP;
		}

		int decode_result = decode(nread, buf);
		if (decode_result < 0) {
			goto CLEANUP;
		}

	}

CLEANUP:;
	fclose(fp);
	EXIT_SUCCESS;
}
