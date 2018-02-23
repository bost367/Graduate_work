#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcmp
#include <stdint.h> // for int16_t and int32_t
#include <math.h>
#include "fft.h"
#include "wavr.h"

void caltFFT(long int* size, int* p, double* freequency, FILE* wav)
{
	//*****************************************************//
	FILE* logfile;
	logfile = fopen("test.txt", "w+");
	if (!logfile) {
		printf("Failed open file, error\n");
		return 0;
	}
	//*****************************************************//	

	long int size_array = *size;
	int p2 = *p;
	double delta = *freequency;
	//fft!!!
	float *c;
	float *in;
	float *out;

	c = calloc(size_array * 2, sizeof(float));
	in = calloc(size_array * 2, sizeof(float));
	out = calloc(size_array * 2, sizeof(float));

	fft_make(p2, c);// функция расчёта поворотных множителей для БПФ

	int16_t value;
	int i = 0;
	int j = 0;

	while (fread(&value, sizeof(value), 1, wav)) {
		in[j] = (float)value;
		j += 2;
		i++;
		if (i > size_array) break;
	}
	fft_calc(p2, c, in, out, 1);
	
	double cur_freq = 0;

	double* ampl;
	ampl = calloc(size_array * 2, sizeof(double));

	double max;
	int start_max = 0;
	int pos_max = 0;
	for (int i = 0; i < size_array; i += 2) {
		double cur_ampl = sqrt(out[i] * out[i] + out[i + 1] * out[i + 1]);
		fprintf(logfile, "%.6f %f\n", cur_freq, (cur_ampl));
		ampl[i] = cur_freq; //freq
		ampl[i + 1] = cur_ampl; //amp

		if (((ampl[i + 1] - 160) > 0) && (!start_max)) {
			start_max = 1;
			max = ampl[i + 1];
			pos_max = i;
		}

		if (start_max) {
			if ((ampl[i + 1] - 160) > 0) {
				if (ampl[i + 1] > max) {
					max = ampl[i + 1];
					pos_max = i;
				}
			}
			else {
				printf("Max Freq=%.3f , amp=%.3f\n", ampl[pos_max], ampl[pos_max + 1]);
				start_max = 0;
			}
		}
		cur_freq += delta;
	}

	free(c);
	free(in);
	free(out);
	free(ampl);
	fclose(logfile);

}

int main(int argc, char *argv[])
{
	char* filename = argv[1];
	FILE*  wav = fopen(filename, "rb");
	struct wavfile header;

	if (wav == NULL) {
		fprintf(stderr, "Can't open input file %s\n", filename);
		exit(1);
	}

	// read header
	if (fread(&header, sizeof(header), 1, wav) < 1) {
		fprintf(stderr, "Can't read input file header %s\n", filename);
		exit(1);
	}

	long int size_array;
	int p2;
	double frequency;
	check(&header, filename);

	printf("size after function = %d\nstepen' = %d, frequuency afrter function = %d", size_array, p2, frequency);
	getWavInfo(&size_array, &p2, &frequency, &header);

	caltFFT(&size_array, &p2, &frequency, wav);

	fclose(wav);
	exit(0);
}


void getWavInfo(long int* size_array, int* p, double* freequency, const struct wavfile* header)
{
	print_header_info(header);
	printf("\nchunk=%d\n", header->bytes_in_data / header->bytes_by_capture);
	//показатель двойки массива
	*p = (int) (log2(header->bytes_in_data / header->bytes_by_capture));
	printf("log2=%d\n", p);
	*size_array = 1 << (int)(log2(header->bytes_in_data / header->bytes_by_capture));
	printf("size array=%ld \n", size_array);
	*freequency = ((float) header->frequency) / (float) *size_array;
}
