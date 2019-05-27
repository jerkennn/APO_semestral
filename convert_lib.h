#ifndef _CONVERT_LIB_H_
#define _CONVERT_LIB_H_
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <sys/time.h>

#ifdef __cplusplusplus
extern "C" 
#endif

#define MIN(a, b) (a <= b ? a : b)
#define MAX(a, b) (a >= b ? a : b)

double hsv[3];
double rgb[3];

double* HSV_to_RGB(double h, double s, double v);
double* RGB_to_HSV(double r, double g, double b);
uint16_t RGB_to_hex(double r, double g, double b);

long getMicrotime();

#ifdef __cplusplus


#endif

#endif
