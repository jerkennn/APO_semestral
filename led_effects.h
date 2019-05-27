#ifndef _LED_EFFECTS_H_
#define _LED_EFFECTS_H_

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

void led_animation(int *led, double h_1, double h_2, long int period, long int startTime);
uint32_t createRGB(int r, int g, int b);

#endif
