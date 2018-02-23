#include <stdlib.h>

struct wavfile {
	char    id[4];          // should always contain "RIFF"
	int32_t totallength;    // total file length minus 8
	char    wavefmt[8];     // should be "WAVEfmt "
	int32_t format;         // 16 for PCM format
	int16_t pcm;            // 1 for PCM format
	int16_t channels;       // channels
	int32_t frequency;      // sampling frequency
	int32_t bytes_per_second;
	int16_t bytes_by_capture;
	int16_t bits_per_sample;
	char    data[4];        // should always contain "data"
	int32_t bytes_in_data;
};

int is_big_endian(void);

void check(const struct wavfile* header,
	char* filename);

void print_header_info(const struct wavfile* header);