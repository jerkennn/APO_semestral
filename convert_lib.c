#define _POSIX_C_SOURCE 200112L

#include "convert_lib.h"

/* zdroj prevodniku barev
 * https://www.programmingalgorithms.com/algorithm/hsv-to-rgb?lang=C
 * */
double* HSV_to_RGB(double h, double s, double v)
{
	double r = 0, g = 0, b = 0;
	if(s==0) {rgb[0] = v; rgb[1] = v; rgb[2] = v;}
	else
	{
		int i;
		double f, p, q, t;
		if (h == 360)
			h = 0;
		else
			h = h / 60;

		i = (int)trunc(h);
		f = h - i;

		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i)
		{
			case 0:
				r = v; g = t; b = p;
				break;
			case 1:
				r = q; g = v; b = p;
				break;
			case 2:
				r = p; g = v; b = t;
				break;
			case 3:
				r = p; g = q; b = v;
				break;
			case 4:
				r = t; g = p; b = v;
				break;
			default:
				r = v; g = p; b = q;
				break;
		}
	}
	rgb[0] = r * 255; rgb[1] = g * 255; rgb[2] = b * 255;
	return rgb;
}

double* RGB_to_HSV(double r, double g, double b)
{
	double delta, min;
	double h = 0, s, v;

	min = MIN(MIN(r, g), b);
	v = MAX(MAX(r, g), b);
	delta = v - min;

	if (v == 0.0)
		s = 0;
	else
		s = delta / v;

	if (s == 0)
		h = 0.0;

	else
	{
		if (r == v)
			h = (g - b) / delta;
		else if (g == v)
			h = 2 + (b - r) / delta;
		else if (b == v)
			h = 4 + (r - g) / delta;

		h *= 60;

		if (h < 0.0)
			h = h + 360;
	}
	hsv[0] = h; hsv[1] = s; hsv[2] = v / 255;
	return hsv;
}

uint16_t RGB_to_hex(double r, double g, double b)
{
	return ((((int)(r*249+1014))>>11)<<11) + ((((int)(g*253+505))>>10)<<5) + (((int)(b*249+1014))>>11);
}

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

