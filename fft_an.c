#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // for int16_t and int32_t
#define _USE_MATH_DEFINES
#include <math.h>
#include "fft.h"
#include "wavr.h"
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define BUF_TYPE unsigned char
#define SAMPLE_RATE (96000) //96000
#define BUF_SIZE (65536)
#define DEGREE (15) //16

void calcFFT(int size_array, int p2, double delta, BUF_TYPE* in, FILE* logfile)
{
	float *c;
	float *out;

	c = calloc(size_array * 2, sizeof(float));
	out = calloc(size_array * 2, sizeof(float));

	fft_make(p2, c);// функция расчёта поворотных множителей для БПФ
	fft_calc(p2, c, in, out, 1);
	
	double cur_freq = 0;

	double* ampl;
	ampl = calloc(size_array * 2, sizeof(double));

	double max;
	int start_max = 0;
	int pos_max = 0;
	for (int i = 0; i < size_array; i += 2) {
		double cur_ampl = sqrt(out[i] * out[i] + out[i + 1] * out[i + 1]);
		fprintf(logfile, "%.6f %f\n", cur_freq, cur_ampl);
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
	free(out);
	free(ampl);
}

void fillArray(float* in, const int* size_array, FILE* wav) {
	int16_t value;
	int i = 0;
	int j = 0;

	while (fread(&value, sizeof(value), 1, wav)) {
		in[j] = (float)value;
		j += 2;
		i++;
		if (i > *size_array) break;
	}
}

int main(int argc, char *argv[])
{
	BUF_TYPE* buffer;
	buffer = calloc(BUF_SIZE, sizeof(BUF_TYPE));
	writeSinForFFT(buffer);

	FILE* logfile;
	logfile = fopen("test.txt", "w+");
	if (!logfile) {
		printf("Failed open file, error\n");
		return 0;
	}

	double delta = (double) ((SAMPLE_RATE * 1.0) / BUF_SIZE);
	calcFFT((int) (BUF_SIZE / 2), DEGREE, delta, buffer, logfile);

	fclose(logfile);
	free(buffer);
	exit(0);
}

void writeSinForFFT(BUF_TYPE* buffer) {
	int f = 1300;
	int amplitude = 125;
	for (int i = 0; i < (int) (BUF_SIZE / 2); i++) {
		*buffer += (BUF_TYPE)((amplitude * sin((float)(2.0 * M_PI * i * f / SAMPLE_RATE))) + amplitude);
		buffer += 2;
	}
}

void writeUsualSin() {
	int f = 1300;
	BUF_TYPE buffer[(int)BUF_SIZE];
	int amplitude = 125;

	for (int i = 0; i < BUF_SIZE; i++) {
		buffer[i] += (BUF_TYPE)((amplitude * sin((float)(2.0 * M_PI * i * f / SAMPLE_RATE))) + amplitude);
	}

	FILE* logfile;
	logfile = fopen("test.txt", "w+");
	if (!logfile) {
		printf("Failed open file, error\n");
		return 0;
	}

	float cur_freq = 0;
	for (int i = 0; i < BUF_SIZE; i++) {
		fprintf(logfile, "%.6f %d.0\n", cur_freq, buffer[i]);
		cur_freq += (float) 1.0 / SAMPLE_RATE;
	}
}
