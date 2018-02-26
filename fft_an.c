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

#define SAMPLE_RATE (96000)
#define BUF_SIZE (16384)
#define DEGREE (14)
#define FREQUENCY (900)

void calcFFT(int size_array, int p2, double delta, float* in, FILE* logfile)
{
	float *c;
	float *out;

	c = calloc(size_array * 2, sizeof(float));
	out = calloc(size_array * 2, sizeof(float));

	float* p1;
	const float* p0;

	fft_make(p2, c);// функция расчёта поворотных множителей для БПФ

	fft_calc_(p2, in, out, p1, p0);
	fft_calc__(p2, c, out, p1, p0, 1);

	//fft_calc(p2, c, in, out, 1);
	
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

int main(int argc, char *argv[])
{
	float* buffer;
	buffer = calloc(BUF_SIZE * 2, sizeof(float));
	writeSinForFFT(buffer);

	FILE* logfile;
	logfile = fopen("test.txt", "w+");
	if (!logfile) {
		printf("Failed open file, error\n");
		return 0;
	}

	double delta = (double) ((SAMPLE_RATE * 1.0) / BUF_SIZE);

	printf("delta = %.5f\n", delta);
	calcFFT(BUF_SIZE, DEGREE, delta, buffer, logfile);

	fclose(logfile);
	free(buffer);
	exit(0);
}

void writeSinForFFT(float* buffer) 
{
	int amplitude = 32000;
	for (int i = 0; i < BUF_SIZE * 2; i += 2) {
		buffer[i] += (float)((amplitude * sin((float)(M_PI * i * FREQUENCY / SAMPLE_RATE))));
	}
}

void writeUsualSin() 
{
	int buffer[BUF_SIZE];
	int amplitude = 500;

	FILE* logfile;
	logfile = fopen("qwe.txt", "w+");

	if (!logfile) {
		printf("Failed open file, error\n");
		return 0;
	}

	float cur_freq = 0;
	for (int i = 0; i < BUF_SIZE; i++) {
		buffer[i] += (int)((amplitude * sin((float)(2.0 * M_PI * i * FREQUENCY / SAMPLE_RATE))));
		fprintf(logfile, "%.6f %d.0\n", cur_freq, buffer[i]);
		cur_freq += (float) 1.0 / SAMPLE_RATE;
	}
	
	fclose(logfile);
}