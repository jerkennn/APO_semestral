#ifndef _LCD_MENU_LIB_H_
#define _LCD_MENU_LIB_H_
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

#define FRAME_W 480
#define FRAME_H 320

#ifdef __cplusplusplus
extern "C"
#endif

//static int btn1=0;
int btn2;
//static int btn3=0;

long time1;
long time2;

unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

void menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3);
void down_controll_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, double*leds);

double* strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2);

#ifdef __cplusplus

#endif

#endif
