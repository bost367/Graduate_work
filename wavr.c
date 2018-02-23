#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "wavr.h"

int is_big_endian(void) {
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01000000 };
	return bint.c[0] == 1;
}

void check(const struct wavfile* header, char* filename) {

	if (is_big_endian()) {
		if (memcmp(header->id, "RIFX", 4) != 0) {
			fprintf(stderr, "ERROR: %s is not a big endian wav file\n", filename);
			exit(1);
		}
	}
	else {
		if (memcmp(header->id, "RIFF", 4) != 0) {
			fprintf(stderr, "ERROR: %s is not a little endian wav file\n", filename);
			exit(1);
		}
	}
	if (memcmp(header->wavefmt, "WAVEfmt ", 8) != 0 || memcmp(header->data, "data", 4) != 0) {
		fprintf(stderr, "ERROR: Not wav format\n");
		exit(1);
	}
	if (header->format != 16) {
		fprintf(stderr, "\nERROR: not 16 bit wav format.");
		exit(1);
	}
	if (memcmp(header->data, "data", 4) != 0) {
		fprintf(stderr, "ERROR: Prrroblem?\n");
		exit(1);
	}
}

void print_header_info(const struct wavfile* header) {
	if (header->pcm == 1) {
		fprintf(stdout, " uncompressed");
	}
	else {
		fprintf(stdout, " compressed");
	}
	fprintf(stdout, " format: %d bits", header->format);
	if (header->format == 16) {
		fprintf(stdout, ", PCM");
	}
	else {
		fprintf(stdout, ", not PCM (%d)", header->format);
	}
	fprintf(stdout, ", channel %d", header->pcm);
	fprintf(stdout, ", freq %d", header->frequency);
	fprintf(stdout, ", %d bytes per sec", header->bytes_per_second);
	fprintf(stdout, ", %d bytes by capture", header->bytes_by_capture);
	fprintf(stdout, ", %d bits per sample", header->bits_per_sample);
	fprintf(stdout, ", %d bytes in data", header->bytes_in_data);
	fprintf(stdout, "\n");
}