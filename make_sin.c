#include "make_wav.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define S_RATE  (96000)
#define BUF_SIZE (S_RATE*0.3)

int main(int argc, char* argv[])
{
    if (argc <= 1) return 0;
    int f = atoi(argv[1]);
    short int buffer[(int) BUF_SIZE];
    float amplitude = 32000;
    for (int i = 0; i < BUF_SIZE; i++)
        buffer[i] += (int)(amplitude * sin((float)(2 * M_PI *i* f / S_RATE)));
    char* func = "sin_";
    char* format = ".wav";
    char file_name[100];
    snprintf(file_name, sizeof(file_name), "%s%d%s", func, f, format);
    write_wav(file_name, BUF_SIZE, buffer, S_RATE);
	return 0;
}