#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#ifndef M_PI
		#define M_PI 3.14159265358979323846
#endif
#include "fft.h"
//----------------------------------------------------------------------------
// функция расчёта поворотных множителей для БПФ
// показатель двойки (например, для БПФ на 256 точек это 8)
// результирующий массив поворотных множителей c[1 << p]
void fft_make(int p, float *c)
{
    int n, i;
    float w, f;
    n = 1 << p; // размер массива (число точек БПФ)
    w = (2. * M_PI) / (float) n;
    f = 0.;
    for (i = 0; i < n; i++)
    {
        *c++ =  cos(f);
        *c++ = -sin(f);
        f += w;
    }
}

void fft_calc_(int p, const float *in, float *out, float* p1, const float* p0) {

	int n = 1 << p;  // число точек БПФ

	// копировать элементы массива `in` в массив `out` с битовой инверсией
	for (int i = 0; i < n; i++)
	{
		int j = fft_binary_inversion(p, i) << 1;
		int k = i << 1;

		p0 = in + j;
		p1 = out + k;
		*p1++ = *p0++; // out[i] = in[j]
		*p1 = *p0;   //

		p0 = in + k;
		p1 = out + j;
		*p1++ = *p0++; // out[j] = in[i]
		*p1 = *p0;   //
	}
}

void fft_calc__(int p, const float *c, float *out, float* p1, const float* p0, int norm) {

	int n = 1 << p;  // число точек БПФ

	float* p2;
	int n2 = n >> 1; // n/2
	float re, im, re1, im1, re2, im2; // c, c1, c2
									  // выполнение бабочек
	for (int i = 0; i < p; i++)
	{
		int m = 1 << (i + 1); // через сколько эл-тов бабочка * 2
		int r = m << 1;       // размер группы * 2
		int nom = 0;          // номер группы * r
		int k = 0;            // номер эл-та в группе * 2
		int y = 0;            // индекс W * 2
		int z = 0;
		int h = 1 << (p - i); // шаг для W * 2
		int j;

		for (j = n2; j > 0; j--)
		{
			if (k >= m)
			{
				k = y = 0;
				nom += r;
				z = nom;
			}

			// c <= c[y]
			p0 = c + y;
			re = *p0++;
			im = *p0;

			// c2 <= out[z + m]
			p1 = out + (z + m);
			re2 = *p1++;
			im2 = *p1;

			// c1 <= c2 * c
			re1 = re2 * re - im2 * im;
			im1 = im2 * re + re2 * im;

			// c2 <= out[z]
			p2 = out + z;
			re2 = *p2++;
			im2 = *p2;

			// out[z]     <= c2 + c1
			// out[z + m] <= c2 - c1
			*p2-- = im2 + im1;
			*p1-- = im2 - im1;
			*p2 = re2 + re1;
			*p1 = re2 - re1;

			k += 2;
			z += 2;
			y += h;
		}
	}

	if (norm)
	{   // нормировка
		re = 1. / ((float)n);
		p1 = out;
		for (int i = n; i > 0; i--)
		{
			*p1++ *= re;
			*p1++ *= re;
		}
	}
}

int fft_binary_inversion(int p, int i)
{
	int j = 0;
	while (p-- > 0)
	{
		j <<= 1;
		j |= i & 1;
		i >>= 1;
	}
	return j;
}