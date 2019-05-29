/*******************************************************************
  KOREK program

  (C) Copyright 2019 by Vojtech Kozel and Martin Rektoris
      e-mail:   kozelvo1@fel.cvut.cz; rektomar@fel.cvut.cz
      license:  MIT license

 *******************************************************************/

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

void led1_animation(int *led, double h_1, double h_2, long int period, long int startTime, int on_time, int off_time);
void led1_static(int *led, int red, int green, int blue, int on_time, int off_time);

void led2_animation(int *led, double h_1, double h_2, long int period, long int startTime, int on_time, int off_time);
void led2_static(int *led, int red, int green, int blue, int on_time, int off_time);

uint32_t createRGB(int r, int g, int b);


#endif
